While you may not directly see what changed in this update, there are a lot of things and I really recommend you to update.
Warlords of Draenor character models, after having been converter to LK of course, should now be supported.
But be careful, there are some things you should be aware :

* There are geosets supported by LK and not by BC. As a result, if you don't delete them in Blender, they will always show. Examples : "Catch-like" belt, Death Knights glowing eyes.
* Remember you don't have to decimate character models meshes if you convert to BC because there a lot of unused sets. Haircuts (those added with the barbershop in LK) for example.
* You will have to resize textures to the size of those of classic models. That's a huge loss in quality, but there is nothing we can do about it. Character texture size is hardcoded in the client. If you don't reduce them, you'll get a WoW Error or, if you're lucky, messy faces.
* Don't forget the DBC Editing. You can't use LK DBCs.

## Changelog
* "-c" option you can use in the command line to convert from LK to Classic instead of BC
* Countless bugfixes
* Now support Lights
* Better conversion for post-LK models (which still have to be converted to LK before using this tool)
* Safety checks

This update is really more stable, and I really recommend to use it. Throw away the previous ones.

## Screenshot
![1](1.png)

## Links
* [Github](https://github.com/Koward/LKBC_Converter)

-- Koward