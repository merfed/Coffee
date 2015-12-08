This is a ridiculously down and dirty program that I pieced together in order to find and view BLPs from my current wow's MPQ libraries. The issue I always ran into when developing addon's is that the blizzard blps were all spread out over multiple MPQs. This on top of identical image paths existing in multiple MPQs made it hard to find/remember where images were.

What this does is finds all of the MPQs that belong to your current wow build and gathers all of the BLP paths in them. All of these are compiled down into a single tree-view allowing you to navigate the BLPs with the file-path hierarchy you would use when referencing these files in LUA.

It also provides a viewing pane that will allow you to view either a single image, or an entire folder's images all at once.

Clicking on an image will provide additional information and allow you to copy the image's path to the clipboard or save the image to disk as a png.

There are some bugs here and there and viewing a folder with a ridiculous number of images in it may take some time to load.

This has not been properly tested. I do not intend on spending a large amount of time fixing bugs or making updates for this tool. As stated before, this is a very, very, down and dirty utility I made to help me out. I am only posting this here for anyone else that finds this to be useful "as-is". I did in-fact code this finished-product software, but it contains code and assemblies that I did not create. Therefore use this at your own risk. I make no claim, warranted, or guarantee to the stability or safety of this software.

Enjoy!

## Works With
* 3.2.0

## Links
* [WoW Curse Forge](http://wow.curseforge.com/addons/quickblp/)

-- DataDink