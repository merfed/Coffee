I am going to show you guys how to make custom non-animated .m2 models out of .mdx (Warcraft III) models. It is believed that converting models using MDXtoM2 is quite a complicated process but actually it is not that hard.

You may ask: "Why do we need to do it if we have OBJtoM2 and it works fine?" Well, the answer is simple. This converter provides you a lot more possibilities than OBJtoM2, such as multiple materials, textures, lights, animations and etc. At first, I will show you how to convert static .m2s but I am going to update this tutorial from time to time when I find out something new. Okay let's go.

## Tools and Resources needed

* **MDXtoM2Converter_lazy** - a tool that generates .m2 models out of .mdx files.
* **3DS Max** with MDX export plugin or Milkshape. Well, just something to convert your model to .mdx. If you already have your .mdx object, you don't need it.
* **MDLXConverter** - a tool that converts .mdx files into .mdl files which are possible to edit manually via Notepad.
* **MDLVis** - .mdx/.mdl editor.
* **War3ModelEditor**- a tool that allows you to add some additional sections to .mdx, such as lights, bones etc.
* **BLPLab** - for converting your textures to .blp (BLP2) file format which is understandable for WoW. (Warcraft textures are BLP1 and cannot be read by WoW).
* **Collision Adder** - a tool that generates collision in your m2 file.
* **Resizer.py** - a python script that allows you to resize the initial size of your model.
* **PyModelEditor** (+Python 2.6 (see PyME thread for details)) to run Resizer.py
* **AddViewDistance** for editing the render distance of the model.
* **Sweetscape 010** editor (+ **M2 template**)  - a hex-editor which we are using for fixing bugs in the model.
* **Nullmodel.mdl - .mdl** template to make your model understandable for MDLVis.

Well, there are plenty of other utilities that can make your model better but they're not essential. The list is quite huge, but don't worry, it won't take that long to get your model into game. Tools which are not linked here are either included into one of the linked packages or can be found in the resource section of Modcraft.

## Static Models

### Preparing your model

I won't explain you how to make an .mdx model. There are plenty of tutorials on this theme over the internet. 

At first, your .mdx file does not contain all the information you need for converting it to .m2. In order to fix it you need to convert your .mdx file into .mdl using MDLXConverter. I will call the result model tree.mdl.

When it is done you need to open your model in MDLVis. To make MDLVis open your file you have to make the .mdl structure understandalbe for MDLVis. So, pick up Nullmodel.mdl and tree.mdl model and open them in Notepad++. 

Find a section called Model in your tree.mdl and add missing strings into this section from Nullmodel.mdl.

**tree.mdl**

	}
	Model "pine" {
	    BlendTime 150,
	    MinimumExtent { -261.362000, -252.451996, -88.562103 },
	    MaximumExtent { 273.287994, 307.015015, 1465.869995 },
	    BoundsRadius 796.038025,
	}


**Nullmodel.mdl**
 

	}
	Model "Sword_2H_Claymore_A_01" {
	    NumGeosets 1,
	    NumGeosetAnims 1,
	    NumBones 1,
	    BlendTime 150,
	}


**Result**

	Model "pine" {
	    NumGeosets 1,
	    NumGeosetAnims 1,
	    NumBones 1,
	    BlendTime 150,
	    MinimumExtent { -261.362000, -252.451996, -88.562103 },
	    MaximumExtent { 273.287994, 307.015015, 1465.869995 },
	    BoundsRadius 796.038025,
	}

Now go to the end of both files and replace everything starting from "Bone" till the end of the file using data from Nullmodel.mdl. Now you can open your model in MDLVis.

Find string Image and write in the path to your textures. This path will be used in your MPQ files.

### Fixing the model in MDLVis
For some reason MDXToM2 does not create a usual transparency chunk in the model. In order to resolve this issue open your model in MDLVis, go to Modules>>Sequences>>Skeleton>>Switch back to Anims. Select both geosets (using check boxes) and mark them as visible.
 
Save your model as .mdx

![1](/images/cm2umdx-1.png)

### Solving possible problems
In most cases now you can just drag'n'drop your tree.mdx to the converter and it will turn it to m2. Try doing it. If there is a crash open your .mdx in War3ModelEditor, go to Windows>>Node editor>>Right click on the bone>>Add light. Add 4-5 lights and try converting the model with MDXtoM2_lazy. If it does not work, add more lights. Okay, you should get your .m2 model now.

![2](/images/cm2umdx-2.png)

### Fixing  different converter issues
Open Resizer.py with a Notepad. (should be placed inside a PyModelEditor folder). Edit paths to your model, set the size to 0.02. We need to decrease the scale of your model in 50 times. Save the file.

Run it by double clicking. If it works you will see a black command prompt window for a second.

Open your model in the 010 editor. Run a template on the file. 

Go to "**struct TheFloats floats**" and divide each value there by 50. For doing this operation faster open "Hex operations" tab, "Divide". Use the following settings: Treat Data As: Float, Operand: 50, Decimal, and press Okay. Now make sure that BoundingBox size is equal to VertexBox size. If not, copy values from bounding box to vertex box. Go to "**struct sAnimations _Animations**" and make sure if the bounding box values are divided by 50 too.

![3](/images/cm2umdx-3.png)

Go to "**struct RenderFlags _RenderFlags**" and set the blending mode of every geoset to **RM_AlphaTesting (1)** (for usual models) and set the Render Flag to the one you need. I used double sided on my leaves geoset to make it visible from both sides. You can find the flag explanation on the WoW Dev Wiki.

If you added lights with War3ModelEditor you have to go to "**uint Lights**", "**ofsLights**" and set the values to 0. Then go to "**struct Lights**", select the section and press Shift + Delete on the selected bytes. 

### Adding collision
If you need your collision to be 100% of the size of the model, just drag'n'drop your .m2 on CollisionAdder.exe and you're done. If you need to make some parts of the model non-collideable, you need to create another .m2 model which contains only the geometry you need to keep the collision on. You can even add more geometry if you need to make some invisible collision walls in your model or something else. For example, for making tree collision for the second model you leave only bark without leaves. Generate the collision for this model and open both ones in the 010 editor. 

Copy the values from "**uint BoundingTriangles**",  "**uint BoundingVertices**" and "**uint BoundingNormals**" to your model. Go to the end of the file and add 5-6 empty strings containing only zeros. 

![4](/images/cm2umdx-4.jpg)

Pick up the offset of the last empty string and write it to "**uint ofsBoundingTriangles**". Copy the entire bounding triangles section to the empty space. Finish the string with zeros. Pick up the offset of the new string and do the same for the rest two sections. 

![5](/images/cm2umdx-5.jpg)

You're done. Don't forget to rescale the collision model before doing that using Resizer.py.

## Animated models
I won't explain you how to convert animated models because there already a good tutorial by Phucko1 (see Converting Animated Models guide) which should explain you everything you need to know about getting animated models to work.

However, I am pretty sure that after doing everything written there most of your model animations will be glitchy. How? This video is the only example I found over the internet, but it looks exactly like this [video](http://www.youtube.com/watch?v=TUtNSHAPML4).

Well, as you can see, some models glitch on some parts of animation. If you reduce the speed of changing frames, you will notice that it looks like the animations start going back at some point. So, it is correct in general, but goes in wrong backwards direction instead of looping. Yeah, that's the mistake of MDXtoM2 converter. I don't know what exactly it is caused by, but I know quite a simple solution for this problem.

### Gathering data
Go to model viewer and open your model. Start playing animations in slow motion mode and make a list of the bugged ones. Now, comes the hardest part. By moving the frames switcher in your WMV you change animation frames. Though, not all of these frames are real. Actual KeyFrames are written to M2, and there are not so much of theme. All other hundreds of frames are just interpolation between them. So, you need to slowly move this switcher and stop it when the animation starts behaving weirdly (most likely goes back or loops where it is not supposed to).  Pick up this numbers and add them to your list under your animation. Do it for every animation if necessary.Keep in mind that these frames value are always approximate, and later you will need to find the closest keyframe value. Though, it is not hard as it probably seems to you now. By opening your model in MDLvis (mdx/mdl source model, not m2), you can pickup the bone numbers of the bugged animation parts. Just look in WMV what parts of the body swing badly, find the corresponding bone numbers in MDLVis and add them to the list for each bugged animation.

### Fixing bugs
Now open your model in pymodeleditor. Show Node Tree>>Pick up the bone

You will now see the Bone Editor. Here you have two buttons: Edit Translation and Edit Rotation. One of them, or both can contain bugged data. Fortunately, it is quite easy to track.

When you open either of them, you get the animation KeyFrames cords. For fixing animations, you need to pick up the animation ID of the animation you need to fix in the dropdown menu marked as 4 in the screenshot. Now, speaking about the data itself. 

![6](/images/cm2umdx-6.png)

Let's now see what is wrong here. This the original block of data.

	0:{ -32759 , -32317 , -32287 , -7 }
	1:{ -32758 , -32317 , -32287 , -7 }
	105:{ -31914 , 31173 , -31813 , -64 }
	243:{ -29948 , 26571 , -31292 , -749 }
	313:{ -29284 , 25977 , -31367 , -932 }
	382:{ -28844 , 28110 , -31543 , -595 }
	521:{ -29063 , -29421 , -32425 , -385 }
	590:{ -29558 , -27700 , 32598 , -555 }
	694:{ -30218 , -29822 , 32081 , -240 }
	868:{ -31503 , 28143 , 31892 , -365 }
	973:{ -32071 , 25472 , 32029 , -839 }
	1077:{ -32320 , 26522 , 32240 , -609 }
	1251:{ -32396 , 31488 , 32626 , -28 }
	1424:{ -32577 , -32500 , -32588 , -3 }
	1667:{ -32758 , -32317 , -32287 , -7 }
	1668:{ -32759 , -32317 , -32287 , -7 }

The number of the each string before ":" is a KeyFrame number. So, the error should be in some numbers close to our previous WMV records. If you look carefully, some values does not follow the common pattern in the middle of the structure. The errors can be only related to negative/positive value differences. You never need to change the numbers. In most cases you mostly need to remove some minuses in from of the first value. But I picked up the hardest example I could find for you here

	521:{ -29063 , -29421 , -32425 , -385 }
	590:{ -29558 , -27700 , 32598 , -555 }
	694:{ -30218 , -29822 , 32081 , -240 }

These 3 keyframes look weird for me. And they surprisingly almost exactly match my WMV animation bug records.

Let's see why. Keep a look at the second column.  These 3 values are negative while everything around them is positive. Here our animation is most likely to go backwards. Let's change them to positive.

	0:{ -32759 , -32317 , -32287 , -7 }
	1:{ -32758 , -32317 , -32287 , -7 }
	105:{ -31914 , 31173 , -31813 , -64 }
	243:{ -29948 , 26571 , -31292 , -749 }
	313:{ -29284 , 25977 , -31367 , -932 }
	382:{ -28844 , 28110 , -31543 , -595 }
	521:{ -29063 , 29421 , -32425 , -385 }
	590:{ -29558 , 27700 , 32598 , -555 }
	694:{ -30218 , 29822 , 32081 , -240 }
	868:{ -31503 , 28143 , 31892 , -365 }
	973:{ -32071 , 25472 , 32029 , -839 }
	1077:{ -32320 , 26522 , 32240 , -609 }
	1251:{ -32396 , 31488 , 32626 , -28 }
	1424:{ -32577 , -32500 , -32588 , -3 }
	1667:{ -32758 , -32317 , -32287 , -7 }
	1668:{ -32759 , -32317 , -32287 , -7 }

And it works like that for Translations and Rotations. Just save your files more often to avoid errors and make backups. It takes approximately and hour for every model. But I am pretty sure if you do it often, you will get a faster skill. 

That's all for now. Big thanks to Vel, Malice and other people who helped me and to the authors of all these tools I used here. I will add some pictures soon enough to make this tutorial more understandable for newbies. If you don't understand something (my English sucks) or have some problems/tips feel free to ask me. Check this thread from time to time for getting recent updates. Good luck with your first model ;).