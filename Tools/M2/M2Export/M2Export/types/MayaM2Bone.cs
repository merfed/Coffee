using System;
using System.Collections.Generic;
using Autodesk.Maya.OpenMaya;
using M2Lib.m2;
using M2Lib.types;

namespace M2Export.types
{
    /// <summary>
    /// Allows to edit data as Maya classes (like axis inversion, rotations..) before adding it to M2Bones which converts everything to M2Lib classes.
    /// </summary>
    public class MayaM2Bone
    {
        public MayaM2Bone Parent { get; set; }
        public string Type { get; set; }
        public string OtherType { get; set; }
        public string Side { get; set; }
        public MVector BaseTranslation { get; set; }
        public List<List<Tuple<uint, MVector>>> Translation { get; } = new List<List<Tuple<uint, MVector>>>();
        public List<List<Tuple<uint, MQuaternion>>> Rotation { get; } = new List<List<Tuple<uint, MQuaternion>>>();
        public List<List<Tuple<uint, MVector>>> Scale { get; } = new List<List<Tuple<uint, MVector>>>();
        public int GlobalIndex { get; set; } = -1; //Predicted index in M2 Bone List. Should be set before converting to Bone.

        private static C3Vector ConvertVector(MVector point) => new C3Vector((float) point.x, (float) point.y, (float) point.z);
        private static C4Quaternion ConvertVector(MQuaternion point) => new C4Quaternion((float) point.x, (float) point.y, (float) point.z, (float) point.w);

        /// <summary>
        /// Create data of a WoW M2 Bone with the values of this class.
        /// </summary>
        public M2Bone ToBone()
        {
            var bone = new M2Bone
            {
                KeyBoneId = GetBoneType(),
                ParentBone = (short) (Parent?.GlobalIndex ?? -1),
                Pivot = ConvertVector(AxisInvert(GetPivot()))
            };
            foreach (var list in Translation)
            {
                var timestamps = new M2Array<uint>();
                var values = new M2Array<C3Vector>();
                foreach (var pair in list)
                {
                    timestamps.Add(pair.Item1);
                    values.Add(ConvertVector(AxisInvert(pair.Item2 - BaseTranslation)));
                }
                bone.Translation.Timestamps.Add(timestamps);
                bone.Translation.Values.Add(values);
            }
            foreach (var list in Rotation)
            {
                var timestamps = new M2Array<uint>();
                var values = new M2Array<C4Quaternion>();
                foreach (var pair in list)
                {
                    timestamps.Add(pair.Item1);
                    values.Add(ConvertVector(AxisInvert(pair.Item2)));
                }
                bone.Rotation.Timestamps.Add(timestamps);
                bone.Rotation.Values.Add(values);
            }
            foreach (var list in Scale)
            {
                var timestamps = new M2Array<uint>();
                var values = new M2Array<C3Vector>();
                foreach (var pair in list)
                {
                    timestamps.Add(pair.Item1);
                    values.Add(ConvertVector(ScaleAxisInvert(pair.Item2)));
                }
                bone.Scale.Timestamps.Add(timestamps);
                bone.Scale.Values.Add(values);
            }
            //TODO maybe bezier ?
            if(Translation.Count > 0) bone.Translation.InterpolationType = M2Track<C3Vector>.InterpolationTypes.Linear;
            if(Rotation.Count > 0) bone.Rotation.InterpolationType = M2Track<C4Quaternion>.InterpolationTypes.Linear;
            if(Scale.Count > 0) bone.Scale.InterpolationType = M2Track<C3Vector>.InterpolationTypes.Linear;
            if(Translation.Count > 0 || Rotation.Count > 0 || Scale.Count > 0) bone.Flags |= M2Bone.BoneFlags.Transformed;
            return bone;
        }

        private static MVector AxisInvert(MVector vec) => MGlobal.isYAxisUp ? new MVector(-1*vec.x, vec.z, vec.y) : vec;
        private static MVector ScaleAxisInvert(MVector vec) => MGlobal.isYAxisUp ? new MVector(vec.x, vec.z, vec.y) : vec;
        private static MQuaternion AxisInvert(MQuaternion vec) 
            => MGlobal.isYAxisUp ? new MEulerRotation(-1*vec.asEulerRotation.x, vec.asEulerRotation.z, vec.asEulerRotation.y).asQuaternion : vec;

        /// <summary>
        /// Apply a quaternion rotation on all translation data of this bone. Used to apply the jointOrient.
        /// </summary>
        /// <param name="quat"></param>
        public void RotateTranslation(MQuaternion quat)
        {
            BaseTranslation = BaseTranslation.rotateBy(quat);

            foreach (var list in Translation)
                for (var i = 0; i < list.Count; i++)
                    list[i] = new Tuple<uint, MVector>(list[i].Item1, list[i].Item2.rotateBy(quat));
        }

        /// <summary>
        /// The pivot is the world space coordinate of the bone. 
        /// It is the sum of all translations (starting from root). The affected objects rotate around this point.
        /// </summary>
        /// <returns></returns>
        private MVector GetPivot()
        {
            if (Parent != null) return Parent.GetPivot() + BaseTranslation;
            return BaseTranslation;
        }

        /// <summary>
        /// Get bone type from joint labelling.
        /// </summary>
        /// <returns>A WoW bone label.</returns>
        private M2Bone.KeyBone GetBoneType()
        {
            switch (Type)
            {
                // As many Maya labels as possible are supported
                case "Root": return M2Bone.KeyBone.Root;
                case "Hip": return M2Bone.KeyBone.Waist;
                case "Spine": return M2Bone.KeyBone.SpineLow;
                case "Head": return M2Bone.KeyBone.Head;
                case "Shoulder": return Side == "Left" ? M2Bone.KeyBone.ShoulderL : M2Bone.KeyBone.ShoulderR;
                case "Index Finger": return Side == "Left" ? M2Bone.KeyBone.IndexFingerL : M2Bone.KeyBone.IndexFingerR;
                case "Middle Finger": return Side == "Left" ? M2Bone.KeyBone.MiddleFingerL : M2Bone.KeyBone.MiddleFingerR;
                case "Ring Finger": return Side == "Left" ? M2Bone.KeyBone.RingFingerL : M2Bone.KeyBone.RingFingerR;
                case "Pinky Finger": return Side == "Left" ? M2Bone.KeyBone.PinkyFingerL : M2Bone.KeyBone.PinkyFingerR;
                case "Thumb": return Side == "Left" ? M2Bone.KeyBone.ThumbL : M2Bone.KeyBone.ThumbR;
                case "Other":
                    switch (OtherType)
                    {
                        // All WoW bone labels are recognized
                        case "ArmL": return M2Bone.KeyBone.ArmL;
                        case "ArmR": return M2Bone.KeyBone.ArmR;
                        case "ShoulderL": return M2Bone.KeyBone.ShoulderL;
                        case "ShoulderR": return M2Bone.KeyBone.ShoulderR;
                        case "SpineLow": return M2Bone.KeyBone.SpineLow;
                        case "Waist": return M2Bone.KeyBone.Waist;
                        case "Head": return M2Bone.KeyBone.Head;
                        case "Jaw": return M2Bone.KeyBone.Jaw;
                        case "IndexFingerR": return M2Bone.KeyBone.IndexFingerR;
                        case "MiddleFingerR": return M2Bone.KeyBone.MiddleFingerR;
                        case "PinkyFingerR": return M2Bone.KeyBone.PinkyFingerR;
                        case "RingFingerR": return M2Bone.KeyBone.RingFingerR;
                        case "ThumbR": return M2Bone.KeyBone.ThumbR;
                        case "IndexFingerL": return M2Bone.KeyBone.IndexFingerL;
                        case "MiddleFingerL": return M2Bone.KeyBone.MiddleFingerL;
                        case "PinkyFingerL": return M2Bone.KeyBone.PinkyFingerL;
                        case "RingFingerL": return M2Bone.KeyBone.RingFingerL;
                        case "ThumbL": return M2Bone.KeyBone.ThumbL;
                        case "$BTH": return M2Bone.KeyBone.Bth;
                        case "$CSR": return M2Bone.KeyBone.Csr;
                        case "$CSL": return M2Bone.KeyBone.Csl;
                        case "Breath": return M2Bone.KeyBone.Breath;
                        case "Name": return M2Bone.KeyBone.Name;
                        case "NameMount": return M2Bone.KeyBone.NameMount;
                        case "$CHD": return M2Bone.KeyBone.Chd;
                        case "$CCH": return M2Bone.KeyBone.Cch;
                        case "Root": return M2Bone.KeyBone.Root;
                        case "Wheel1": return M2Bone.KeyBone.Wheel1;
                        case "Wheel2": return M2Bone.KeyBone.Wheel2;
                        case "Wheel3": return M2Bone.KeyBone.Wheel3;
                        case "Wheel4": return M2Bone.KeyBone.Wheel4;
                        case "Wheel5": return M2Bone.KeyBone.Wheel5;
                        case "Wheel6": return M2Bone.KeyBone.Wheel6;
                        case "Wheel7": return M2Bone.KeyBone.Wheel7;
                        case "Wheel8": return M2Bone.KeyBone.Wheel8;
                        default: return M2Bone.KeyBone.Other;
                    }
                default: return M2Bone.KeyBone.Other;
            }
        }
    }
}
