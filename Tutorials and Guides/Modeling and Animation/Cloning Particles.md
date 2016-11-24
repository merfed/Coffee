In this tutorial I'll explain to you how to clone particles from a model to another one with the help of ParticleCloner, a tool made by Tigurius. This tool is old, so it doesn't work with Cata+ models since the structure of the m2's has changed. I didn't tried with spell models from Cata and up (only doodads or creatures), maybe it's different and it can work, but not sure.

Edit: It can load WoD or Legion models, but it's pretty random, if the models are similar it can work, sometimes not, just do some tests and try.

## Tools
* ParticleCloner
* 010 Editor with M2 Template

## Process

First, choose the particles you need in WowModelViewer on the model that you want.

For this tutorial I choose particles from the spell Arcane_form_precast.m2 to put on 2 models, one without bones or animations, and one with both : misc_2h_pitchfork_a_01.m2 and sheep.m2 .

![1](images/clonep-1.gif)
![2](images/clonep-2.jpg)
![3](images/clonep-3.jpg)

To make the transfer, select first the "destination" model, CTRL+ left click to select the second "source" model and drag&drop on the application. (Always do backups if you did it in the wrong way, this program overwrite the original file ! )

![4](images/clonep-4.gif)

When it's done, you'll have the particles at the same place of its original model.

![5](images/clonep-5.gif)
![6](images/clonep-6.gif)

Now, you certainly want to move the particles to the right place.

Open the m2 with 010editor and load the M2 Template. Go to the end of the file at the line " struct Particle ". You'll have all the informations you need, the attached bones, coordinates, etc...

Particles have the bone from the original model, for a simple animation like this, there's only one bone. For this fire, there's three elements (flames, smoke and sparks), so there's 3 bones. I used it for a Sargeras model.

![7](images/clonep-7.jpg)
![8](images/clonep-8.gif)
![9](images/clonep-9.jpg)

You have to set the number of the bone where you want the particles to be. To move it to the right place, go to the " struct FloatVec3 pos " line. As you can see on the picture above, I modified the values on the x axis.

![10](images/clonep-10.jpg)

You can change what you want to have the good coords. Just try and retry with the help of WMV to visualize the result. For the sheep, I set the particles on bone 3 who's attached to the head.

![11](images/clonep-11.jpg)
![12](images/clonep-12.jpg)

This tool can copy the texture path of the BLPs used by the particles.

![13](images/clonep-13.jpg)

But when you'll go ingame to see if it works, you'll have little cubes instead of the texture. WMV can load them, but not WoW.

![14](images/clonep-14.jpg)

Why ? Because the tool does some mess too. In the " struct VModelFileName " 1 & 2, you have random symbols at " struct filename texture > string value " . Delete all and reload the template, or save and close the model. Do it for each particle in your model.

![15](images/clonep-15.jpg)

And it's finally done ! Good job, you can manage particles !

![16](images/clonep-16.gif)

**PS:** for unknown reasons, my fork is invisible ingame after I added the particles on it. Maybe because the model doesn't have enough "informations" in it because it's a little model, or because there's no bones in the original model. I don't know. Maybe one of you will find the solution ?

## Credits
Met@