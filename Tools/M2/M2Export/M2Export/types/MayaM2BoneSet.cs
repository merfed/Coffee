using System;
using System.Collections;
using System.Collections.Generic;

namespace M2Export.types
{
    public class MayaM2BoneSet : IEnumerable<MayaM2Bone>
    {
        private readonly HashSet<MayaM2Bone> _bones = new HashSet<MayaM2Bone>();
        private readonly int _maxBones;
        public int startBone { get; set; } = -1;
        public int ExtraRoom => _maxBones - _bones.Count;

        public MayaM2BoneSet(int maxBones)
        {
            _maxBones = maxBones;
        }

        public void Add(MayaM2Bone bone)
        {
            if(ExtraRoom == 0) throw new Exception("Bone set is full.");
            _bones.Add(bone);
        }

        public bool IsSubsetOf(IEnumerable<MayaM2Bone> set) => _bones.IsSubsetOf(set);

        public bool Contains(MayaM2Bone bone) => _bones.Contains(bone);
        public IEnumerator<MayaM2Bone> GetEnumerator()
        {
            return _bones.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
