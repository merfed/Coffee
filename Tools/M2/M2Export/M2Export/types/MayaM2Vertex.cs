using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using Autodesk.Maya.OpenMaya;
using M2Lib.m2;
using M2Lib.types;

namespace M2Export.types
{
    public class MayaM2Vertex : IEquatable<MayaM2Vertex>
    {
        public MFloatPoint Position { get; set; } = new MFloatPoint();
        public MFloatVector Normal { get; set; } = new MFloatVector();
        public List<Tuple<float, float>> UvCoordinates { get; set; } = new List<Tuple<float, float>>();
        public List<Tuple<MayaM2Bone, double>> Weights { get; set; } = new List<Tuple<MayaM2Bone, double>>();
        public int GlobalIndex { get; set; } = -1;


        public const int MaxWeightsNumber = 4;

        public override bool Equals(object obj)
        {
            return obj != null && Equals(obj as MayaM2Vertex);
        }

        public bool Equals(MayaM2Vertex other)
        {
            if (other == null) return false;
            var equalTexCoords = true;
            if (UvCoordinates.Count != other.UvCoordinates.Count) equalTexCoords = false;
            else
            {
                for (var i = 0; i < UvCoordinates.Count; i++)
                {
                    if (!UvCoordinates[i].Equals(other.UvCoordinates[i])) equalTexCoords = false;
                }
            }
            return Position.Equals(other.Position)
                   && Normal.Equals(other.Normal)
                   && equalTexCoords;
        }

        public static bool operator ==(MayaM2Vertex left, MayaM2Vertex right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(MayaM2Vertex left, MayaM2Vertex right)
        {
            return !Equals(left, right);
        }

        [SuppressMessage("ReSharper", "NonReadonlyMemberInGetHashCode")]
        public override int GetHashCode()
        {
            var hashCode = Position?.GetHashCode() ?? 0;
            hashCode = (hashCode*397) ^ (Normal?.GetHashCode() ?? 0);
            hashCode = (hashCode*397) ^ (UvCoordinates?.GetHashCode() ?? 0);
            return hashCode;
        }

        public M2Vertex ToWoW()
        {
            var vert = new M2Vertex
            {
                Position = MayaToM2.AxisInvert(Position),
                Normal = MayaToM2.AxisInvert(Normal)
            };
            for (var i = 0; i < vert.TexCoords.Length && i < UvCoordinates.Count; i++)
                vert.TexCoords[i] = new C2Vector(UvCoordinates[i].Item1, 1 - UvCoordinates[i].Item2);

            if(Weights.Count > MaxWeightsNumber)
                MGlobal.displayWarning("This vertex is connected to more than "+MaxWeightsNumber+ " joints. Only the " + MaxWeightsNumber + " biggest will be exported.");

            var exportedWeights = Weights.OrderByDescending(p => p.Item2).Take(MaxWeightsNumber).ToList();
            Debug.Assert(exportedWeights.Count <= MaxWeightsNumber);
            //Normalize weight
            var weightSum = exportedWeights.Sum(p => p.Item2);
            if (Math.Abs(weightSum) <= MayaToM2.Epsilon)
            {
                vert.BoneWeights[0] = byte.MaxValue;
            }
            else
            {
                var availableWeight = byte.MaxValue;
                for (var j = 0; j < exportedWeights.Count; j++)
                {
                    vert.BoneIndices[j] = (byte) exportedWeights[j].Item1.GlobalIndex;
                    //Stored weight is the minimum between actual weight and remaining weight, to keep the sum == 255
                    var byteWeight = (byte) (exportedWeights[j].Item2/weightSum*byte.MaxValue);
                    vert.BoneWeights[j] = byteWeight > availableWeight ? availableWeight : byteWeight;

                    availableWeight -= vert.BoneWeights[j];
                }
                if (availableWeight > 0) // Remains
                {
                    vert.BoneWeights[Weights.Count - 1] += availableWeight;
                }
            }

            var totalWeight = vert.BoneWeights.Sum(w => w);
            Debug.Assert(totalWeight == byte.MaxValue, "Total sum of weights is not 255 but " + totalWeight);

            return vert;
        }
    }

    /// <summary>
    /// Create unique instances of vertices from given data
    /// </summary>
    public static class VertexFactory
    {
        private static Dictionary<Tuple<MFloatPoint, MFloatVector, List<Tuple<float, float>>>, MayaM2Vertex> _instances 
            = new Dictionary<Tuple<MFloatPoint, MFloatVector, List<Tuple<float, float>>>, MayaM2Vertex>();

        public static MayaM2Vertex Create(MFloatPoint pos, MFloatVector norm, List<Tuple<float, float>> uv,
            List<Tuple<MayaM2Bone, double>> weights, List<MayaM2Vertex> globalVertexList)
        {
            var compositeKey = new Tuple<MFloatPoint, MFloatVector, List<Tuple<float, float>>>(
                pos,
                norm,
                uv);
            if (_instances.ContainsKey(compositeKey)) return _instances[compositeKey];
            var vert = new MayaM2Vertex
            {
                Position = pos,
                Normal = norm,
                UvCoordinates = uv,
                Weights = weights
            };
            _instances[compositeKey] = vert;
            globalVertexList.Add(vert);
            return vert;
        }
    }
}
