using System.Collections.Generic;
using System.Linq;

namespace M2Export.types
{
    public class MayaM2Triangle
    {
        public List<MayaM2Vertex> Vertices { get; set; } = new List<MayaM2Vertex>();

        /// <summary>
        /// All bones referenced by the vertices of this triangle.
        /// </summary>
        public MayaM2BoneSet Bones
        {
            get
            {
                var bones = new MayaM2BoneSet(12);
                foreach (var weight in Vertices.SelectMany(vert => vert.Weights))
                    bones.Add(weight.Item1);
                return bones;
            }
        }
    }
}
