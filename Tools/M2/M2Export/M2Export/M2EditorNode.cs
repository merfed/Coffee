using System.Text;
using Autodesk.Maya.OpenMaya;
using M2Export;
using M2Lib.m2;

[assembly: MPxNodeClass(typeof(M2EditorNode), "m2Editor", 0x0003FFF0)]

namespace M2Export
{
    public class M2EditorNode : MPxNode
    {
        // Animation clips
        public static MObject Name;
        public static MObject Start;
        public static MObject End;
        public static MObject Type;
        public static MObject LoopingFlag;
        public static MObject LowPriorityFlag;
        public static MObject RepetitionsMin;
        public static MObject RepetitionsMax;
        public static MObject BlendingFlag;
        public static MObject BlendTimeStart;
        public static MObject BlendTimeEnd;
        public static MObject Rarity;
        public static MObject Export;

        public static MObject AnimClips;

        [MPxNodeInitializer]
        public static bool Initialize()
        {
            InitializeAnimClips();
            return true;
        }

        public static void InitializeAnimClips()
        {
            var tAttr = new MFnTypedAttribute();
            var nAttr = new MFnNumericAttribute();
            var cAttr = new MFnCompoundAttribute();
            var eAttr = new MFnEnumAttribute();

            Name = tAttr.create("animClipName", "acn", MFnData.Type.kString);
            addAttribute(Name);
            Start = nAttr.create("animClipStart", "acs", MFnNumericData.Type.kInt);
                nAttr.setMin(0);
            addAttribute(Start);
            End = nAttr.create("animClipEnd", "ace", MFnNumericData.Type.kInt);
                nAttr.setMin(0);
            addAttribute(End);
            Type = eAttr.create("animClipType", "act");
                foreach(var entry in AnimationData.NameToId) eAttr.addField(entry.Key, (short) entry.Value);//TODO check performance
                eAttr.setDefault(0);
            addAttribute(Type);
            LoopingFlag = nAttr.create("animClipLooping", "acl", MFnNumericData.Type.kBoolean);
                nAttr.setDefault(true);
            addAttribute(LoopingFlag);
            LowPriorityFlag = nAttr.create("animClipLowPriority", "aclp", MFnNumericData.Type.kBoolean);
                nAttr.setDefault(false);
            addAttribute(LowPriorityFlag);
            RepetitionsMin = nAttr.create("animClipRepMin", "acrepmin", MFnNumericData.Type.kFloat);
                nAttr.setDefault((float) 0);
                nAttr.setMin(0);
            addAttribute(RepetitionsMin);
            RepetitionsMax = nAttr.create("animClipRepMax", "acrepmax", MFnNumericData.Type.kFloat);
                nAttr.setDefault((float) 0);
                nAttr.setMin(0);
            addAttribute(RepetitionsMax);
            BlendingFlag = nAttr.create("animClipBlending", "acb", MFnNumericData.Type.kBoolean);
                nAttr.setDefault(true);
            addAttribute(BlendingFlag);
            BlendTimeStart = nAttr.create("animClipBlendTimeStart", "acbts", MFnNumericData.Type.kInt);
                nAttr.setDefault(150);
                nAttr.setMin(0);
            addAttribute(BlendTimeStart);
            BlendTimeEnd = nAttr.create("animClipBlendTimeEnd", "acbte", MFnNumericData.Type.kInt);
                nAttr.setDefault(150);
                nAttr.setMin(0);
            addAttribute(BlendTimeEnd);
            Rarity = nAttr.create("animClipRarity", "acra", MFnNumericData.Type.kFloat);
                nAttr.setDefault((float) 100);
                nAttr.setMin(0);
                nAttr.setMax(100);
            addAttribute(Rarity);
            Export = nAttr.create("exportAnimClip", "eac", MFnNumericData.Type.kBoolean);
                nAttr.setDefault(true);
            addAttribute(Export);

            AnimClips = cAttr.create("animClips", "clips");
            cAttr.addChild(Name);
            cAttr.addChild(Start);
            cAttr.addChild(End);
            cAttr.addChild(Type);
            cAttr.addChild(LoopingFlag);
            cAttr.addChild(LowPriorityFlag);
            cAttr.addChild(RepetitionsMin);
            cAttr.addChild(RepetitionsMax);
            cAttr.addChild(BlendingFlag);
            cAttr.addChild(BlendTimeStart);
            cAttr.addChild(BlendTimeEnd);
            cAttr.addChild(Rarity);
            cAttr.addChild(Export);
            cAttr.isArray = true;
            addAttribute(AnimClips);
        }


        private static string ClipGetAttrCommand(string editorName, int index, string attribute)
        {
            var builder = new StringBuilder();
            builder.Append("getAttr ")
            .Append(editorName)
            .Append(".animClips[")
            .Append(index)
            .Append("].")
            .Append(attribute);
            return builder.ToString();
        }
        public static void GetClipAttribute(string editorName, int index, string attribute, out string result)
            => MGlobal.executeCommand(ClipGetAttrCommand(editorName, index, attribute), out result);
        public static void GetClipAttribute(string editorName, int index, string attribute, out int result)
            => MGlobal.executeCommand(ClipGetAttrCommand(editorName, index, attribute), out result);
        public static void GetClipAttribute(string editorName, int index, string attribute, out double result)
            => MGlobal.executeCommand(ClipGetAttrCommand(editorName, index, attribute), out result);

        public static void GetClipAttribute(string editorName, int index, string attribute, out bool result)
        {
            int intermediate;
            GetClipAttribute(editorName, index, attribute, out intermediate);
            result = intermediate != 0;
        }
    }
}