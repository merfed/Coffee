## Convert MDX to M2

**Needed Tools:**
* BLPLab (Warcraft 3)
* MDLxConv (Warcraft 3)
* BLPConverter (WoW)
* Converter MDX (WoW)

### BLP
To convert the BLP from Version 1.0 (WC3) to 2.0 (WoW), open the BLP1 with BLPLab and save it as PNG, then convert the PNG with BLPConverter to BLP2.

### Model
If your model is missing important parts (like bones, transparency, etc), convert the MDX with MDLxConv to MDL and open the MDL in Notepad.

**Textures**

If you need to add them by hand:

	Textures 1 {
	   Bitmap {
	      Image "CustomItem\dmT.blp",
	   }
	}

**Transparency**

For every geoset you need:

	GeosetAnim {
	   Alpha 1 {
	      DontInterp,
	      0: 1.000000,
	   }
	   GeosetId 0,
	}

**Sequences/Animations**

You'll need at least one:

	Sequences 1 {
	   Anim "Stand" {
	      Interval { 0, 3333 },
	                MinimumExtent { -0.656940, -2.999150, -2.449310 },
	                MaximumExtent { 0.550530, 21.822701, 1.284070 },
	                BoundsRadius 12.463241,
	   }
	}

**Materials**

Should also be included (renderflags):

	Materials 1 {
	   Material {
	      Layer {
	         FilterMode None,
	         static TextureID 0,
	      }
	   }
	}

**Bones**

At least one Bone must be in the model:

	Bone "objHandle" {
	   ObjectId 0,
	   GeosetId 0,
	   GeosetAnimId None,
	}

Now convert it to MDX again (or try if it works with MDL, sometimes it doesn't) and open up a command prompt. Then type:

`ConverterMDX <MDX> [optional: scale]`

Then the converter will ask you some questions like AnimationIDs. The AnimationIDs can be found in AnimationData.dbc. NextAnimation can be -1 or the next Animation(...). subAnimID is which one of a row of animations this is.

Camera Types are: 0 - Portrait, 1 - CharInfo, -1 - Else.

**Attachements**

       typedef enum <uint32>{
    	   Mountpoint_or_LeftWrist =0,
    	   Right_palm,
    	   Left_palm,
    	   Right_elbow,
    	   Left_elbow,
    	   Right_Shoulder,
    	   Left_Shoulder,
    	   Right_Knee,
    	   Left_knee,
    	   unk1,
    	   unk2,
    	   Helmet,
    	   Back,
    	   unk3,
    	   unk4,
    	   Bust,
    	   Bust2,
    	   Face,
    	   Above_Char,
    	   Ground,
    	   Top_of_head,
    	   Left_Palm2,
    	   Right_Palm2,
    	   unk5,
    	   unk6,
    	   unk7,
    	   Right_Back_Sheath,
    	   Left_Back_Sheath,
    	   Middle_Back_Sheath,
    	   Belly,
    	   Left_Back,
    	   Right_Back,
    	   Left_Hip_Sheath,
    	   Right_Hip_Sheath,
    	   Bust3,
    	   Right_Palm3,
    	   unk8,
    	   demolishervehicle1,
    	   demolishervehicle2,
    	   Vehicle_Seat1,
    	   Vehicle_Seat2,
    	   Vehicle_Seat3,
    	   Vehicle_Seat4,
    	   unk9,
    	   unk10,
    	   unk11,
    	   unk12,
    	   unk13
    	}ATTACHMENT_ID;

Where the first one is 0, the second 1 etc. Now just place all the files in the correct paths and you're done.