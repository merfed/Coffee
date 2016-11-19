using System.Collections.Generic;
using System.Linq;

namespace M2Export.types
{
    public class MayaM2Mesh
    {
        public List<MayaM2Triangle> Faces { get; set; } = new List<MayaM2Triangle>();
        public List<MayaM2Texture> Textures { get; set; } = new List<MayaM2Texture>();


        /// <summary>
        /// Divides itself in submeshes according to bone partitions computed from LOD value.
        /// </summary>
        public List<MayaM2Mesh> DivideMesh(List<MayaM2BoneSet> partition)
        {
            var submeshes = partition.Select(part => new MayaM2Mesh()).ToList();
            foreach (var tri in Faces)
            {
                var bones = tri.Bones;
                for (var i = 0; i < partition.Count; i++)
                {
                    if (!bones.IsSubsetOf(partition[i])) continue;
                    submeshes[i].Faces.Add(tri);
                    break;
                }
            }
            return submeshes;
        } 

        public static List<MayaM2BoneSet> PartitionBones(List<MayaM2Mesh> meshes, int lod)
        {
            var partition = new List<MayaM2BoneSet>();
            foreach (var mesh in meshes)
            {
                foreach (var tri in mesh.Faces)
                {
                    var bones = tri.Bones;
                    var alreadyContained = partition.Any(part => bones.IsSubsetOf(part));
                    if (alreadyContained) continue;
                    MayaM2BoneSet bestPart = null;
                    var bestPartRoom = int.MaxValue;
                    foreach(var part in partition)
                    {
                        var extraBones = bones.Count(b => !part.Contains(b));
                        var extraRoom = part.ExtraRoom;
                        if (extraRoom < extraBones || extraRoom >= bestPartRoom) continue;
                        bestPart = part;
                        bestPartRoom = extraRoom;
                    }
                    if (bestPart == null)
                    {
                        bestPart = new MayaM2BoneSet(lod);
                        partition.Add(bestPart);
                    }
                    foreach (var bone in bones.Where(b => !bestPart.Contains(b)))
                    {
                        bestPart.Add(bone);
                    }
                }
            }
            return partition;
        } 
    }
}
