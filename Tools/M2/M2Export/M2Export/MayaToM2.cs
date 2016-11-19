using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using Autodesk.Maya.OpenMaya;
using Autodesk.Maya.OpenMayaAnim;
using M2Export.types;
using M2Lib.m2;
using M2Lib.types;

namespace M2Export
{
    /// <summary>
    /// Extract from Maya to WoW.
    /// </summary>
    public static class MayaToM2
    {
        public const float Epsilon = 0.00001f;
        /// <summary>
        /// Maximum number of bones per drawcall for each view. Linked to GPU numbers of registers.
        /// </summary>
        private static readonly int[] MaxBones = {256, 64, 53, 21};

        //Invert and create vector
        public static C3Vector AxisInvert(float x, float y, float z)
        {
            return MGlobal.isYAxisUp ? new C3Vector(-1*x, z, y) : new C3Vector(x, y, z);
        }

        public static C3Vector AxisInvert(MFloatPoint point) => AxisInvert(point.x, point.y, point.z);
        public static C3Vector AxisInvert(MFloatVector point) => AxisInvert(point.x, point.y, point.z);
        public static C3Vector AxisInvert(MPoint point) => AxisInvert((float) point.x, (float) point.y, (float) point.z);

        public static void ExtractModel(M2 wowModel)
        {
            var initialTime = MAnimControl.currentTime;
            MAnimControl.currentTime = 0;

            MGlobal.displayInfo("Building model " + wowModel.Name);

            var seqList = ExtractSequences();

            var jointMap = ExtractJoints(seqList);

            var globalVertexList = new List<MayaM2Vertex>();
            var meshList = new List<MayaM2Mesh>();
            ExtractStandardMeshes(meshList, jointMap, globalVertexList);

            //To WoW proper classes
            InjectCollisionMesh(wowModel);
            MayaM2Sequence.NormalizeProbability(seqList);
            foreach (var seq in seqList)
            {
                //TODO SubAnimationId?
                wowModel.Sequences.Add(seq.ToWoW());
            }

            foreach (var bone in jointMap.Values)
            {
                bone.GlobalIndex = wowModel.Bones.Count;
                wowModel.Bones.Add(bone.ToBone());
            }

            foreach (var vert in globalVertexList)
            {
                vert.GlobalIndex = wowModel.GlobalVertexList.Count;
                wowModel.GlobalVertexList.Add(vert.ToWoW());
            }

            foreach (var lod in MaxBones)
            {
                var view = new M2SkinProfile {Bones = (uint) lod};

                var partition = MayaM2Mesh.PartitionBones(meshList, lod);
                foreach (var part in partition)
                {
                    part.startBone = wowModel.BoneLookup.Count;
                    foreach (var bone in part) wowModel.BoneLookup.Add((short) bone.GlobalIndex);
                }

                foreach (var mesh in meshList)
                {
                    var submeshes = mesh.DivideMesh(partition);
                    foreach (var submesh in submeshes)
                    {
                        var wowSubmesh = new M2SkinSection();
                        wowSubmesh.StartVertex = (ushort) view.Indices.Count;
                        var viewRelativeIndices = new Dictionary<MayaM2Vertex, int>();
                        foreach (var face in submesh.Faces)
                        {
                            foreach (var vert in face.Vertices)
                            {
                                Debug.Assert(vert.GlobalIndex >= 0, "Global index not set.");
                                if (viewRelativeIndices.ContainsKey(vert)) continue;
                                viewRelativeIndices[vert] = view.Indices.Count;
                                view.Indices.Add((ushort) vert.GlobalIndex);
                                wowSubmesh.NVertices++;
                            }
                        }
                        wowSubmesh.StartTriangle = (ushort) view.Triangles.Count;
                        foreach (var face in submesh.Faces)
                        {
                            foreach (var vert in face.Vertices)
                            {
                                view.Triangles.Add((ushort) viewRelativeIndices[vert]);
                                wowSubmesh.NTriangles++;
                            }
                        }
                        view.Submeshes.Add(wowSubmesh);
                    }
                    //TODO what's needed along with submesh (shader, tex..)
                    //TODO Batch
                }
                wowModel.Views.Add(view);
            }

            //Static models requirements
            if(wowModel.Sequences.Count == 0) wowModel.Sequences.Add(new M2Sequence());//For non-animated models, basic "Stand"
            if(wowModel.Bones.Count == 0) wowModel.Bones.Add(new M2Bone());//For jointless static models
            if(wowModel.TexUnitLookup.Count == 0) wowModel.TexUnitLookup.Add(0);
            if(wowModel.UvAnimLookup.Count == 0) wowModel.UvAnimLookup.Add(-1);

            MAnimControl.currentTime = initialTime;
        }

        private static List<MayaM2Sequence> ExtractSequences()
        {
            // Sequences
            List<MayaM2Sequence> seqList = new List<MayaM2Sequence>();
            var editorNames = new MStringArray();
            MGlobal.executeCommand("ls -type m2Editor", editorNames);
            var editorName = editorNames[0];
            MGlobal.displayInfo("Searching editor node... Result : "+editorName);
            if (!string.IsNullOrEmpty(editorName))
            {
                MGlobal.displayInfo("Editor data found.");
                int numberOfClips;
                MGlobal.executeCommand("getAttr -size " + editorName + ".animClips", out numberOfClips);
                MGlobal.displayInfo("\tExtracting "+numberOfClips+" clips.");
                for (var i = 0; i < numberOfClips; i++)
                {
                    int start;
                    int end;
                    int type;
                    bool looping;
                    bool lowPriority;
                    int repMin;
                    int repMax;
                    bool blending;
                    int blendTimeStart;
                    int blendTimeEnd;
                    double rarity;
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipStart", out start);
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipEnd", out end);
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipType", out type);
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipLooping", out looping);
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipLowPriority", out lowPriority);
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipRepMin", out repMin);
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipRepMax", out repMax);
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipBlending", out blending);
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipBlendTimeStart", out blendTimeStart);
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipBlendTimeEnd", out blendTimeEnd);
                    M2EditorNode.GetClipAttribute(editorName, i, "animClipRarity", out rarity);
                    var mayaSeq = new MayaM2Sequence
                    {
                        Start = start,
                        End = end,
                        Type = type,
                        IsLoop = looping,
                        IsLowPriority = lowPriority,
                        MinimumRepetitions = repMin,
                        MaximumRepetitions = repMax,
                        IsBlending = blending,
                        BlendTimeStart = blendTimeStart,
                        BlendTimeEnd = blendTimeEnd,
                        Probability = (short) (rarity * short.MaxValue)
                    };
                    seqList.Add(mayaSeq);
                }
            }

            //Default when no animation clip, tries to get slider range.
            if (seqList.Count != 0)
            {
                var mayaSeq = new MayaM2Sequence
                {
                    Start = (int) MAnimControl.minTime.asUnits(MTime.Unit.kMilliseconds),
                    End = (int) MAnimControl.maxTime.asUnits(MTime.Unit.kMilliseconds),
                };
                if (mayaSeq.Start != mayaSeq.End) mayaSeq.IsLoop = true;
                seqList.Add(mayaSeq);
            }
            return seqList;
        }

        private static Dictionary<string, MayaM2Bone> ExtractJoints(List<MayaM2Sequence> seqList)
        {
            var jointMap = new Dictionary<string, MayaM2Bone>();

            //Goal of iteration : Extract raw joint data and store it in intermediate object, MayaM2Bone
            var processedJoints = new HashSet<string>();
            for(var jointIter = new MItDag(MItDag.TraversalType.kDepthFirst, MFn.Type.kJoint); !jointIter.isDone; jointIter.next())
            {
                var jointPath = new MDagPath();
                jointIter.getPath(jointPath);
                if (processedJoints.Contains(jointPath.fullPathName)) continue;
                MGlobal.displayInfo("Extracting raw data of "+jointPath.fullPathName);
                var joint = new MFnIkJoint(jointPath);
                var mayaBone = new MayaM2Bone();
                jointMap[jointPath.fullPathName] = mayaBone;

                // Hierarchy
                var isRoot = joint.parentCount == 0 || !joint.parent(0).hasFn(MFn.Type.kJoint);
                if (!isRoot) {
                    var parentPath = new MDagPath();
                    MDagPath.getAPathTo(joint.parent(0), parentPath);
                    if(!jointMap.ContainsKey(parentPath.fullPathName))
                        MGlobal.displayError("\tParent is not referenced. Crash incoming. Path : "+parentPath.fullPathName);
                    jointMap[jointPath.fullPathName].Parent = jointMap[parentPath.fullPathName];
                }
                //Note : M2Bone.submesh_id is wrong in the wiki. Do not try to compute it.
                // Label
                jointMap[jointPath.fullPathName].Type = MGlobal.executeCommandStringResult("getAttr -asString " + joint.fullPathName + ".type");
                jointMap[jointPath.fullPathName].OtherType = MGlobal.executeCommandStringResult("getAttr -asString " + joint.fullPathName + ".otherType");
                jointMap[jointPath.fullPathName].Side = MGlobal.executeCommandStringResult("getAttr -asString " + joint.fullPathName + ".side");

                // Base translation is used to compute position
                MAnimControl.currentTime = 0;
                jointMap[jointPath.fullPathName].BaseTranslation = joint.getTranslation(MSpace.Space.kTransform);

                foreach (var seq in seqList)
                {
                    var transData = new List<Tuple<uint, MVector>>();
                    var rotData = new List<Tuple<uint, MQuaternion>>();
                    var scaleData = new List<Tuple<uint, MVector>>();
                    for (var i = seq.Start; i < seq.End; i += 33) //TODO FIXME What if not multiple of 33 ?
                    {
                        //Get data for this joint for this frame
                        MAnimControl.currentTime = new MTime(i, MTime.Unit.kMilliseconds);

                        var translation = joint.getTranslation(MSpace.Space.kTransform);
                        var rotation = new MQuaternion();
                        joint.getRotation(rotation, MSpace.Space.kTransform);
                        var scaleArray = new double[3];
                        joint.getScale(scaleArray);
                        var scale = new MVector(scaleArray);

                        if (!translation.isEquivalent(MVector.zero, Epsilon))
                        {
                            var previousIsTheSame = transData.Count > 0 &&
                                                    transData.Last().Item2.isEquivalent(translation, Epsilon);
                            if (!previousIsTheSame)
                                transData.Add(new Tuple<uint, MVector>((uint) (i - seq.Start), translation));
                        }
                        if (!rotation.isEquivalent(MQuaternion.identity, Epsilon))
                        {
                            var previousIsTheSame = rotData.Count > 0 &&
                                                    rotData.Last().Item2.isEquivalent(rotation, Epsilon);
                            if (!previousIsTheSame)
                                rotData.Add(new Tuple<uint, MQuaternion>((uint) (i - seq.Start), rotation));
                        }
                        if (!scale.isEquivalent(MVector.one, Epsilon))
                        {
                            var previousIsTheSame = scaleData.Count > 0 &&
                                                    scaleData.Last().Item2.isEquivalent(scale, Epsilon);
                            if (!previousIsTheSame)
                                scaleData.Add(new Tuple<uint, MVector>((uint) (i - seq.Start), scale));
                        }
                    }
                    if (transData.Count > 0) jointMap[joint.fullPathName].Translation.Add(transData);
                    if (rotData.Count > 0) jointMap[joint.fullPathName].Rotation.Add(rotData);
                    if (scaleData.Count > 0) jointMap[joint.fullPathName].Scale.Add(scaleData);
                }


                processedJoints.Add(jointPath.fullPathName);
            }

            //Goal of iteration : apply transformations to joint data & their children
            processedJoints.Clear();
            for (var jointIter = new MItDag(MItDag.TraversalType.kBreadthFirst, MFn.Type.kJoint);
                !jointIter.isDone;
                jointIter.next())
            {
                var jointPath = new MDagPath();
                jointIter.getPath(jointPath);
                if (processedJoints.Contains(jointPath.fullPathName)) continue;
                MGlobal.displayInfo("Applying joint orient of "+jointPath.fullPathName);
                var joint = new MFnIkJoint(jointPath);
                var jointOrient = new MQuaternion();
                joint.getOrientation(jointOrient);

                for (uint i = 0; i < joint.childCount; i++)
                {
                    if (!joint.child(i).hasFn(MFn.Type.kJoint)) continue;
                    var childFn = new MFnIkJoint(joint.child(i));
                    MGlobal.displayInfo("\tto "+ childFn.fullPathName+";");
                    jointMap[childFn.fullPathName].RotateTranslation(jointOrient);
                }

                processedJoints.Add(jointPath.fullPathName);
            }
            return jointMap;
        }

        /// <summary>
        /// Get the local indices from mesh relative triangle indices obtained with MItMeshPolygon.getTriangle().
        /// Original C++ script by Bruno 'Beosil' Heidelberger and Thomas Cowell.
        /// </summary>
        /// <param name="polyMeshRelative"></param>
        /// <param name="triMeshRelative"></param>
        /// <returns>A list of 3 face-relative indices each.</returns>
        private static MIntArray GetLocalTriangle(MIntArray polyMeshRelative, MIntArray triMeshRelative)
        {
            Debug.Assert(triMeshRelative.Count == 3);
            var result = new MIntArray();
            foreach (var vertMeshIndex in triMeshRelative)
            {
                for (var gv = 0; gv < polyMeshRelative.Count; gv++)
                {
                    if (polyMeshRelative[gv] != vertMeshIndex) continue;
                    result.Add(gv);
                    break;
                }
            }
            return result;
        } 

        /// <summary>
        /// Faces and vertices UVs, positions and normals.
        /// </summary>
        private static void ExtractMeshGeometry(MayaM2Mesh mesh, MDagPath meshPath, Dictionary<string, MayaM2Bone> jointMap, List<MayaM2Vertex> globalVertexList)
        {
            // ***Data Tables***
            // UV Sets
            var uvsets = new MStringArray();
            var meshFunctions = new MFnMesh(meshPath);
            meshFunctions.getUVSetNames(uvsets);

            //Bone Weights
            var vertexWeights = new List<MDoubleArray>();
            var influenceObjects = new MDagPathArray();
            GetMeshWeightData(vertexWeights, influenceObjects, meshPath);

            //Positions
            var positions = new MFloatPointArray();
            meshFunctions.getPoints(positions, MSpace.Space.kWorld);

            //Normals
            var normals = new MFloatVectorArray();
            meshFunctions.getVertexNormals(false, normals, MSpace.Space.kWorld);

            var polygonIter = new MItMeshPolygon(meshPath);
            while (!polygonIter.isDone)
            {
                //Divide face into triangles
                var polyMeshRelative = new MIntArray();
                polygonIter.getVertices(polyMeshRelative);
                int numTriangles;
                polygonIter.numTriangles(out numTriangles);
                for (var i = 0; i < numTriangles; i++)
                {
                    var triangle = new MayaM2Triangle();
                    var triangleMeshRelative = new MIntArray();
                    polygonIter.getTriangle(i, new MPointArray(), triangleMeshRelative);
                    var triangleFaceRelative = GetLocalTriangle(polyMeshRelative, triangleMeshRelative);
                    for(var v = 0; v < 3; v++)
                    {
                        var meshIndex = triangleMeshRelative[v];
                        var faceIndex = triangleFaceRelative[v];

                        //Bone weights
                        var weights = new List<Tuple<MayaM2Bone, double>>();
                        for (var b = 0; b < influenceObjects.length; b++) //for each joint
                        {
                            var kJointPath = influenceObjects[b];
                            if (!kJointPath.hasFn(MFn.Type.kJoint)) continue;
                            Debug.Assert(b < vertexWeights[meshIndex].Count,
                                "vertexWeights size : " + vertexWeights.Count + " " +
                                "\njointWeights for this vertex : " + vertexWeights[meshIndex].Count);
                            //Here are a joint&weight for this vertex
                            if (vertexWeights[meshIndex][b] > Epsilon)
                            {
                                weights.Add(new Tuple<MayaM2Bone, double>(jointMap[kJointPath.fullPathName],
                                    vertexWeights[meshIndex][b]));
                            }
                        }

                        //Position & normals
                        var position = positions[(int) polygonIter.vertexIndex(faceIndex)];
                        var normal = normals[(int) polygonIter.normalIndex(faceIndex)];

                        //UV coordinates
                        var uvCoordinates = new List<Tuple<float, float>>();
                        if (uvsets.length > 0 && meshFunctions.numUVs(uvsets[0]) > 0)
                        {
                            foreach (var uvset in uvsets)
                            {
                                var uvCoords = new float[2];
                                polygonIter.getUV(faceIndex, uvCoords, uvset);
                                uvCoordinates.Add(new Tuple<float, float>(uvCoords[0], uvCoords[1]));
                            }
                        }
                        var vert = VertexFactory.Create(position, normal, uvCoordinates, weights, globalVertexList);
                        triangle.Vertices.Add(vert);
                    }
                    mesh.Faces.Add(triangle);
                }
                polygonIter.next();
            }
        }

        /// <summary>
        /// Get for each vertex the weights for all influence objects, including zero weights.
        /// </summary>
        /// <param name="vertexWeights"></param>
        /// <param name="influenceObjects"></param>
        /// <param name="meshPath"></param>
        private static void GetMeshWeightData(List<MDoubleArray> vertexWeights, MDagPathArray influenceObjects, MDagPath meshPath)
        {
            var fnMesh = new MFnMesh(meshPath);

            // Get any attached skin cluster
            var hasSkinCluster = false;

            // Search the skin cluster affecting this geometry
            var kDepNodeIt = new MItDependencyNodes(MFn.Type.kSkinClusterFilter);

            // Go through each skin cluster in the scene until we find the one connected to this mesh
            while (!kDepNodeIt.isDone && !hasSkinCluster)
            {
                MGlobal.displayInfo("Processing skin cluster...");
                var kObject = kDepNodeIt.thisNode;
                var kSkinClusterFn = new MFnSkinCluster(kObject);
                var uiNumGeometries = kSkinClusterFn.numOutputConnections;
                kSkinClusterFn.influenceObjects(influenceObjects);
                MGlobal.displayInfo("\t uiNumGeometries : " + uiNumGeometries);
                MGlobal.displayInfo("\t influenceOBjects number : " + influenceObjects.Count);

                // Go through each connection on the skin cluster until we get the one connecting to this mesh
                MGlobal.displayInfo("Mesh we are looking for : " + fnMesh.fullPathName);
                for(uint uiGeometry = 0; uiGeometry < uiNumGeometries && !hasSkinCluster; uiGeometry++)
                {
                    var uiIndex = kSkinClusterFn.indexForOutputConnection(uiGeometry);
                    var kInputObject = kSkinClusterFn.inputShapeAtIndex(uiIndex);
                    var kOutputObject = kSkinClusterFn.outputShapeAtIndex(uiIndex);
                    if (!kOutputObject.hasFn(MFn.Type.kMesh)) continue;
                    var fnOutput = new MFnMesh(MDagPath.getAPathTo(kOutputObject));
                    MGlobal.displayInfo("Output object : " + fnOutput.fullPathName);

                    if (fnOutput.fullPathName != fnMesh.fullPathName) continue;

                    hasSkinCluster = true;
                    MGlobal.displayInfo("\t==> A connected skin cluster has been found.");

                    // Go through each vertex (== each component) and save the weights for each one
                    var kGeometryIt = new MItGeometry(kInputObject);
                    while (!kGeometryIt.isDone)
                    {
                        var kComponent = kGeometryIt.currentItem;
                        var kWeightArray = new MDoubleArray();
                        uint uiNumInfluences = 0;
                        kSkinClusterFn.getWeights(meshPath, kComponent, kWeightArray, ref uiNumInfluences);
                        vertexWeights.Add(kWeightArray);

                        kGeometryIt.next();
                    }
                }
                kDepNodeIt.next();
            }
        }

        private static void ExtractStandardMeshes(List<MayaM2Mesh> meshList, Dictionary<string, MayaM2Bone> jointMap, List<MayaM2Vertex> globalVertexList)
        {
            for(var meshIter = new MItDag(MItDag.TraversalType.kDepthFirst, MFn.Type.kMesh);
            !meshIter.isDone; meshIter.next())
            {
                var meshPath = new MDagPath();
                meshIter.getPath(meshPath);

                var meshFn = new MFnMesh(meshPath);
                // only want non-history items
                if (meshFn.isIntermediateObject) continue;
                var name = meshFn.name;
                if (name == "Collision") continue;//TODO use custom attribute
                var mesh = new MayaM2Mesh();
                ExtractMeshGeometry(mesh, meshPath, jointMap, globalVertexList);
                ExtractMeshShaders(mesh, meshPath);
                meshList.Add(mesh);
            }
        }

        /// <summary>
        /// Extract the vertices, normals and triangles of a mesh into the M2 collision data fields.
        /// </summary>
        /// <param name="wowModel"></param>
        private static void InjectCollisionMesh(M2 wowModel)
        {
            var collisionFound = false;
            for(var meshIter = new MItDag(MItDag.TraversalType.kDepthFirst, MFn.Type.kMesh);
            !meshIter.isDone; meshIter.next())
            {
                var meshPath = new MDagPath();
                meshIter.getPath(meshPath);

                var meshFn = new MFnMesh(meshPath);
                // only want non-history items
                if (meshFn.isIntermediateObject) continue;
                var name = meshFn.name;
                if (name != "Collision") continue;//TODO use custom attribute
                if (collisionFound)
                    throw new Exception("More than one collision box has been found. One supported.");
                MGlobal.displayInfo("\t Collision mesh detected.");

                wowModel.CollisionBox = new CAaBox(AxisInvert(meshFn.boundingBox.min),
                    AxisInvert(meshFn.boundingBox.max));
                wowModel.CollisionSphereRadius =
                    (float) Math.Max(meshFn.boundingBox.depth/2, meshFn.boundingBox.width/2);

                //TODO fixme better iterate through faces
                var collisionPoints = new MFloatPointArray();
                meshFn.getPoints(collisionPoints, MSpace.Space.kWorld);
                var collisionNormals = new MFloatVectorArray();
                meshFn.getNormals(collisionNormals, MSpace.Space.kWorld);
                var collisionTriangles = new MIntArray();
                meshFn.getTriangles(new MIntArray(), collisionTriangles);
                for (var i = 0; i < collisionPoints.Count; i++)
                {
                    wowModel.CollisionVertices.Add(AxisInvert(collisionPoints[i]));
                    wowModel.CollisionNormals.Add(AxisInvert(collisionNormals[i]));
                }
                foreach(var vertIndex in collisionTriangles)
                    wowModel.CollisionTriangles.Add((ushort) vertIndex);

                collisionFound = true;
            }
        }

        /// <summary>
        /// Originally written in C++ by RobTheBloke. 
        /// See https://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/research/maya/mfnmesh.htm
        /// </summary>
        /// <param name="shadingEngine"></param>
        /// <returns>The shader name.</returns>
        private static MObjectArray GetMaterials(MObject shadingEngine)
        {
            // attach a function set to the shading engine
            var fn = new MFnDependencyNode(shadingEngine);

            // get access to the surfaceShader attribute. This will be connected to
            // lambert , phong nodes etc.
            var sShader = fn.findPlug("surfaceShader");

            // will hold the connections to the surfaceShader attribute
            var materials = new MPlugArray();

            // get the material connected to the surface shader
            sShader.connectedTo(materials, true, false);
            var materialsObjects = new MObjectArray();

            if (materials.Count <= 0) return materialsObjects;
            // if we found a material
            foreach (var plug in materials)
            {
                materialsObjects.Add(plug.node);
            }
            return materialsObjects;
        }

        /// <summary>
        /// Extract a material and its linked texture, if any.
        /// </summary>
        private static void ExtractMaterial(MayaM2Mesh mesh, MObject material)
        {
            var fnShader = new MFnLambertShader(material);
            // Get lambert out of the shader
            var strName = fnShader.name;
            MGlobal.displayInfo("Detected material : "+strName+" of type "+ material.apiTypeStr);
            //TODO use this type

            // ReSharper disable once UnusedVariable
            var clrDiffuse = fnShader.color;
            // ReSharper disable once UnusedVariable
            var clrAmbient = fnShader.ambientColor;
            if (material.hasFn(MFn.Type.kReflect))
            {
                var fnReflectShader = new MFnReflectShader(material);
                // ReSharper disable once UnusedVariable
                var clrSpec = fnReflectShader.specularColor;
            }

            // Look for textures at the color plug
            var colorPlug = fnShader.findPlug("color");
            var fileTextureIter = new MItDependencyGraph(colorPlug.node, MFn.Type.kFileTexture,
                MItDependencyGraph.Direction.kUpstream, MItDependencyGraph.Traversal.kDepthFirst,
                MItDependencyGraph.Level.kNodeLevel);
            while (!fileTextureIter.isDone)
            {
                //TODO hardcoded textures
                var nodeFn = new MFnDependencyNode(fileTextureIter.thisNode());

                var fileNamePlug = nodeFn.findPlug("fileTextureName");
                string textureFileName;
                fileNamePlug.getValue(out textureFileName);
                MGlobal.displayInfo("\t Texture found : " + textureFileName);

                var wrapUPlug = nodeFn.findPlug("wrapU");
                var wrapU = false;
                wrapUPlug.getValue(ref wrapU);
                var wrapVPlug = nodeFn.findPlug("wrapV");
                var wrapV = false;
                wrapVPlug.getValue(ref wrapV);
                var tex = new MayaM2Texture
                {
                    WrapU = wrapU,
                    WrapV = wrapV
                };
                mesh.Textures.Add(tex);
                fileTextureIter.next();//maybe now the loop is fixed
            }
        }

        /// <summary>
        /// Extract all shaders (M2Batch) linked to a mesh.
        /// </summary>
        private static void ExtractMeshShaders(MayaM2Mesh mesh, MDagPath meshPath)
        {
            MGlobal.displayInfo("Looking for shaders in mesh "+meshPath.fullPathName);
            var meshFn = new MFnMesh(meshPath);

            // get the number of instances
            var numInstances = meshFn.parentCount;
            MGlobal.displayInfo("\t"+numInstances+" instances.");

            // loop through each instance of the mesh
            for (uint i = 0; i < numInstances; ++i)
            {
                // attach a function set to this instances parent transform
                //var fn = new MFnDependencyNode(fnMesh.parent(i));

                // this will hold references to the shaders used on the meshes
                var shaderEngines = new MObjectArray();

                // this is used to hold indices to the materials returned in the object array
                var faceIndices = new MIntArray();

                // get the shaders used by the i'th mesh instance
                meshFn.getConnectedShaders(i, shaderEngines, faceIndices);

                switch (shaderEngines.length)
                {
                    // if no shader applied to the mesh instance
                    case 0:
                        break;
                    // if all faces use the same material
                    case 1:
                        var materials = GetMaterials(shaderEngines[0]);
                        MGlobal.displayInfo("\t\tIn shaderEngine[0], found "+materials.length+" materials.");

                        //TODO Extract Material data
                        ExtractMaterial(mesh, materials[0]);
                        //TODO Extract Transparency data
                        //TODO Shader
                        break;
                    //Multiple materials, each applied only on some faces. 
                    default:
                        throw new NotImplementedException("Cannot handle more than one shaderEngine per mesh.");
                }
            }
        }
    }
}