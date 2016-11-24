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