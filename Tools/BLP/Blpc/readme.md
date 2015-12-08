Blp2 texture viewer and converter with a graphical interface. Full source included (C#).

##Features
* Supports a variety of formats for viewing and saving:
	* Blp2 textures
		* Dxt1, 3 & 5
		* Raw1 (indexec colors)
		* Raw3 (32bpp)
	* GDI+ formats
		* Png
		* Jpeg
		* Gif
		* Bitmaps, Icons
		* Emf, Wmf
		* Tiff
* Mipmap creation
* Decent color quantization for Raw1 files using NeuQuant and Floyd-Steinberg dithering
* Automatic resizing for Blp conversion when necessary
* Batch conversion
* Optional file type association
* Short guideline included to shed some light on the various texture types
* Open source (full C# code included in download)
	* Simple class integration in your C# projects
	* MIT license

##Prerequisites
Requires Microsofts .Net Framework 4.0, Windows Installer 3.1 and Libsquish (included).


## Known issues
This is not Photoshop. Converting huge textures (e.g. 2048x2048) can take some time depending on your texture type. Textures of more common dimensions should be converted almost instantly though.
Jpeg encoded Blp textures NYI. Tell me if you find an example within the WoW data files.

## Using cBlp2 in your projects
Handling blp files with the Blp2 class is quite simple:

    using MDS.cBlp2;
    using System.Drawing;
    
    ...
    
    Blp2 myBlp = Blp2.FromFile(@"C:\test.blp");
    Bitmap myBmp = (Bitmap)myBlp;
    // Do stuff


    myBlp = Blp2.FromFile(@"C:\test.png");
    myBlp.Save(@"C:\test.blp", LibSquish.DxtFormat.Dxt3, Blp2.ResizeMethod.Extend, true);
    myBlp.Save(@"C:\test2.jpeg", System.Drawing.Imaging.ImageFormat.Jpeg);

For more information rummage through the hopefully well documented Blp2 class or take a look at the implementation in Blpc.

# Links
* [Wowinterface](http://www.wowinterface.com/downloads/info18810-Blpc.html)
* [WoW Curseforge](http://wow.curseforge.com/addons/blpc/)


-- hawkthetank AKA H4nk