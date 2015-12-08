It must've been around the 500th time or so, of converting various BLP files back and forth, when I thought..

Why all this converting nonsense? Why can't I just load up BLP files right into Photoshop, edit them, and save them back to BLP? And then I remembered - because there's no Photoshop plugin for that format.

So I figured I'd try to change that, and I have.

##Supported Formats
###Windows
* x64
	* CS4 CS5 CS6 CC CC 2014 
* x86 (32-bit)
	* 6.0 7.0 CS CS2 CS3 CS4 CS5 CS6 CC CC 2014

### Mac OS X
Mac OS X plugin requires OS X 10.6 (Lion)+

* x64
	* CS5 CS6 CC CC2014

##Installation
###Windows

1. Find your Photoshop Folder
	1. eg1: (x64) C:\Program Files\Adobe\Adobe Photoshop CS6 (64 Bit)\
	2. eg2: (32-bit) C:\Program Files\Adobe\Adobe Photoshop CS6\
2. Drop the Appropriate "Plug-ins" folder for your build of Photoshop in this folder
3. Accept any merge folder prompts.
4. Run Photoshop

### OS X
1. Find your Photoshop Folder
	1. eg: /Applications/Photoshop CS6/
2. From that folder drop the "Plug-ins" folder for Mac into that location.
3. Accept any folder merge prompts.
4. Run Photoshop

## Supported Variants
It was my goal to support every variant of the BLP format that Blizzard uses in WoW - for both opening and saving. The end result is a total of 11 different combinations of compression and transparency. Below is a listing of these.

**Alpha Bit Depths** This is the amount of detail allowed in transparency of your image.

* 0-bit - No transparency at all
* 1-bit - simple transparency, similar to GIFs. It's either transparent or not.
* 4-bit - slightly better range of transparency, but will turn smooth fades into visible "steps"
* 8-bit - This is the same level of transparency you'll ordinarily see in PNG/TGA formats, with 256 levels of transparency.

**Compression Types**

* Indexed (256 Color) - Very similar to a GIF, as it uses a palette that can fit up to 256 colors, but BLP files have indipendent transparency. Indexed BLPs can make use of all 4 types of alpha.
* DXT - This is the defacto compression type used in modern games, with the most popular format that uses this, being .dds. DXT compression gives you a better color range, with a slightly larger size, than indexed. It also supports all 4 levels of transparency.
* DXT-5- Would normally be lumped in with the above category, however it's the second DXT type that supports 8-bit alpha. The result is a bit better color, and a slightly larger file size than the 8-bit alpha under the DXT category.
* Uncompressed- Very much like a Bitmap, with no compression to speak of. It can support either 0-bit or 8-bit alpha. This version is only used by Blizzard for sky textures, where every bit of color detail is necessary.

## Questions, Comments, and Other Feedback
Please comment below, especially if you find a bug not already mentioned above. Whether that be a problem loading a WoW BLP file, or saving your own, let me know. If it's a WoW file, let me know the name, and if it's one of your own - I would appreciate linking me (either in the comments or PM) the original image you're trying to convert. If you don't feel comfortable with sending me a PSD file, that's ok too, but whatever the problem is, might get lost in the conversion.

Mac Users, Thank you for your patience!

I never intended on "Soon" rivaling Blizzard's definition, but life has a tendency of interfering with intentions. I hope you find it useful, and if you run into any problems - please let me know.

Interested in Batch Conversions?

The Photoshop plugin is great for loading, editing and saving single BLP images; but if your main goal is simply conversion, BLPNG Converter might be more your style. Whether it be a single file, or an entire folder tree, BLPNG Converter will quickly, and easily convert any (WoW era) BLP file to PNG, or popular image formats (bmp,gif,jpg and png) to BLP!

## Works With
* **6.2.0** - 1.0 (Repack)
* **6.0.2** - 1.0, 0.X3a, 0.X3
* **5.3.0** - 0.X2, 0.X

## Changelog
### 1.0 (repack)
Changed the folder structure around in the zip file, so each build is in its own "Plug-ins/File Formats" folder, to make installing easier.

### 1.0
The limitation that prevented saving when a "flattened" Background layer was present in the document, has been solved and subsequently removed.
Now that the plugin is "feature-complete", it has shed it's beta...ness.

### 0.X3a
OS X Initial Release

### 0.X3
Fixed an issue where Photoshop would hang and/or give a "Disk Error" message when attempting to open a BLP file without mipmaps.

In older versions of Photoshop, the Compression Drop-down list in the save options, wouldn't expand all the way. This has been fixed.

### 0.X2
Fixed issue that was preventing saved BLP files from being displayed in game
Plugin will now throw an error if the image you are attempting to save, does not have dimensions that are a power of 2

### 0.X
Initial Release

## Links
* [WoW CurseForge](http://wow.curseforge.com/addons/photoshop-blp-format-plugin/)

-- DigitalUtopia