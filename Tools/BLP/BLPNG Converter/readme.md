While doing various WoW related projects, I was always in need of a utility that would convert BLP images to a more common format. Unfortunately, all the converters I came across would fail on at least one BLP format. Since I recently created a 3ds Max BLP import plugin, I decided to use the knowledge I gained of the format, to make my own converter. This is that converter.

I've used this personally for a few months - converting maps, UI graphics, textures, etc, and have yet to find a BLP file that it not only cannot convert, but cannot convert correctly. 

# Windows Information

BLPNG Converter (Pronounced "Bleeping Converter") Has gotten a bit of a makeover for it's second version. In order to keep things as simple and uncomplicated for the user as possible,it's left it's meager Console life behind, and has been given an actual UI. 

In addition, it can now convert from standard Windows image formats (BMP,GIF,JPG and PNG) to the BLP format as well!

Use is still nearly as simple as before - although now you will have to actually open the app first. Once it's open, drop any number of files or folders on the appropriate square, and it will dutifully convert all the appropriate files within, to the desired format. 

The addition of the UI has also brought some additional options with it. You can now choose files or a folder from the appropriate menu choices, and set the desired BLP format details. In addition, you can also select to have BLP Converter save your settings between sessions - although, this does require it to leave a settings file, in a folder, in your AppData/Application Data folder. 

Despite, and even considering this, it remains just as portable as the original version. Run it from wherever you'd like, and if you should choose to want to get rid of it, just dump it in your recycle bin and forget it. 

Oh a few other things. 

1. The UI will display a progress bar, count, percent complete, and the current file - while it's processing - but the fancy Taskbar progress bar also makes its return. So if you're doing a huge conversion job, and want to work on other things - feel free to minimize it or let it sit in the background. Well, as long as you have Windows 7+

2. Instead of the fancy Yellow progress bar on the Taskbar, if it's unable to convert some files, the "Conversion Complete" message that shows up at the end, will now have a yellow "caution" icon next to it, with a link to allow you to see what didn't get converted, and why. 

3. More of a warning: The "Conversion Complete" message will show up at the end of any action, and the green checkmark only means that it didn't encounter any errors. There's currently no special status if it didn't find anything to convert - such as if you happen to drop a folder of png files, on the "To Png" square. 

## Requirements
Yep, I'm about to say that 'N' word...

.NET 4.0 is required. 
aaaand...Visual C++ Redistributable 

Which also means that this is, indeed, a Windows application...and this time written in Managed C++, instead of C#. 

Despite the odd requirements, it has been tested, and does still work in Windows XP. 

## Supported Formats

This is for reference, and completeness only. You're not expected to, or required to know this. 

BLPNG Converter converts all known (to the author) varieties of the BLP format to PNG. This includes...

* BLPs with and without minmaps
* Indexed (256 color) BLPs with 0, 1, 4, and 8 bit alpha
* DXT Compressed BLPs with 0, 1, 4, and 8 bit alpha, as well as handling both types of 8 bit alpha (DXT3 and DXT5)
* Uncompressed BLPs with 0 or 8 bit alpha

## Issues, Problems, Bugs, Questions, Etc...
I want to hear them all. 

This should be perfectly functional - as the Conversion code has been lifted directly from the current Photoshop plugin, and polished to a sexy shine. But...stuff happens - so if you do run into an issue, please let me know and I'll correct it as soon as possible. 

oh...and, uh- sorry for taking so ungodly long on this, jeffy162. :P

# OS X Information
Like the name implies, this is the Mac OS X version of BLPNG Converter. Written in Cocoa (Objective-C) to be a native OS X application.

## About
BLPNG Converter (Pronounced "Bleeping Converter") Carries over the same form and utility from the Windows version, only reworked in design and features to meet the expectations of a Mac application.

Also like the Windows version, there's no installer. In this case, in the zip file (required by WowInterface), you'll see a standard .dmg file. 

### Features
1. Progress section appears in a slide-down sheet, and a second progress bar will appear below the dock icon.

2. Conversion complete message will appear on its own page, covering up the drop targets. This page will also include "Go Back" and "Log" buttons for navigation

3. Since OS X handles files and folders equally, the menu options under "To BLP" and "To PNG" will only have a single "Choose" option. 

4. Like the Windows version, most - if not all natively supported Mac image formats can be used to convert to BLP.

## Requirements
XCode is pretty picky about backwards compatibility with C++ - the language that the actual conversion code is written in. Because of that, OS X 10.7 Lion (or newer) is required. 

I haven't quite figured out yet, how to elevate permissions without a Developer ID ($99), so you will need to keep your images in a folder that's writable for you, without having root. Otherwise you'll get an error. 

## Issues, Problems, Bugs, Questions, Etc...
I want to hear them all. 

This is my first official application for OS X, and since I do not have a Mac, testing the application out on different versions of OS X - is a much more difficult proposition. So while I can guarantee that it runs perfectly in OS X 10.8 (Mountain Lion), if you have a different version - and it doesn't work, I'd like to know about it. 

In addition, any feature requests, or design suggestions are also welcome. 

## Links
* [WoW Interface -- Win Version](http://www.wowinterface.com/downloads/info22128-BLPNGConverter.html#info)
* [WoW Interface -- Mac Version](http://www.wowinterface.com/downloads/info23205-BLPNGConverterMac.html)


-- DigitalUtopia