# LKBC_Converter
World of Warcraft M2 model converter from version 08 (_Wrath of the Lich King_) to 07-04 (_The Burning Crusade_).
Written in ANSI C, documented with Doxygen, tested with CuTest, made with love and tears.

##What's yet to be implemented :
* Ribbons (trails like Al'ar)
* Particles (Spells, glowing effects..)
	
This is still heavy work in progress.
  
  
##Frequently Asked Questions :
###Can we convert Warlords of Draenor character models ?
Yes since 0.4. But read this carefully : 
* There are geosets supported by LK and not by BC. As a result, if you don't delete them in Blender, they will always show. Examples : "Catch-like" belt, Death Knights glowing eyes.
* Remember you don't have to decimate character models meshes if you convert to BC because there a lot of unused sets. Haircuts (those added with the barbershop in LK) for example.
* You will have to resize textures to the size of those of classic models. That's a huge loss in quality, but there is nothing we can do about it. Character texture size is hardcoded in the client. If you don't reduce them, you'll get a WoW Error or, if you're lucky, messy faces.
* Don't forget the DBC Editing. You can't use LK DBCs.

###Can we use it on creatures or only with static objects ?
This converted is aimed at converting creatures, something that had never been done previously by any LKBC conversion tool.
Not all of them are working perfectly, but the code is getting better every day.

###My model is not working !
In most cases, you're just using a model which has unsupported features, like particles or lights. 
But errors can happen elsewhere too. Report your problem in the issue section ;) !
Don't forget to add the name of your model and the nature of the problem.
  
  
##Credits :
Original idea and structures by Stan84.
Thanks to Stan84, PhilipTNG, Schlumpf and all the people who contributed to M2&M2/WotLK pages on the WoWDev wiki.
Thanks to OneSadCookie for its https://github.com/OneSadCookie/fcaseopen

##You can help me :
* If you have pure C (I don't want a line of C++ in this program !) knowledge and have some ideas to improve the code;
* If you're able to make sweet reskins to make creatures match their vanilla counterpart. Example : a spotted Siberian Tiger texture ?

## Links
* [Github](https://github.com/Koward/LKBC_Converter)

-- Koward