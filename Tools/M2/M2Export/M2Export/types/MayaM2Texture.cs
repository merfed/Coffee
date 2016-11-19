using M2Lib.m2;

namespace M2Export.types
{
    public class MayaM2Texture
    {
        public bool WrapU { get; set; } = false;
        public bool WrapV { get; set; } = false;

        public M2Texture ToWoW()
        {
            var wowTexture = new M2Texture {Type = M2Texture.TextureType.Monster1}; //TODO type with name
            if (WrapU) wowTexture.Flags |= M2Texture.TextureFlags.WrapX;
            if (WrapV) wowTexture.Flags |= M2Texture.TextureFlags.WrapY;
            return wowTexture;
        }
    }
}
