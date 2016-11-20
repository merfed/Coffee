First of all, why the heck do I even bother with something what seems to be so basic? Well, apparently it is not, because there are a few tips and tricks which aren't very commonly known, at least beginners don't know them and there's no tutorial really describing them. I will also link my simple WIP tool which can be very usefull for you here. I will start with things you probably do already know, if that bothers you, just skip to the last heading.

First of all, how can we get models into our map in Noggit? A short list of possibilities:

- Ctrl+c and ctrl+v. No magic hidden here.
- Import from MV.
- Import from a text file (Import.txt).

## Import from MV
MV is a model viewer. WoW Model Viewer is an application which enables you to browse WoW models. Great thing, but here comes a problem - current Noggit's import functions aren't compatible with quite a lot of WMV's versions. As far as I know, importing WMOs in general tends to work. In order to import models from WMV, simply change ImportPath in noggit.conf, so it points to log.txt file in userSettings folder in your WMV's folder. An example follows.:

    ImportFile = D:\Modding\Projekty\Arathor\WMV\userSettings\log.txt

When you have this set, you can simply open a WMV, select a model, then go to Noggit and shift+c to import M2, alt+c to import WMO. There were slightly bugged versions of Noggit where both shortcuts actually imported M2, keep this on mind please. The newest version should have this fixed by now.

If your import functions don't work (mostly M2 tends to not), you can try a different version of WMV. Version which is definitely compatible with Noggit's import function for M2s can be downloaded on Modcraft (see WoWModelViewer 7 - Noggit Version).

The problem is, that this version is really an ancient one, and tends to crash frequently, especially when it comes to retro-ported stuff.

## Import from Text File
Import.txt is a file located in Noggit's installation folder. It contains paths to models separated by line breaks. You can delete models from here simply by opening a file and deleting them. To reload a file, just deselect any model you may have selected in Noggit and go to Assist menu, choose To Text File function, and here you go, Import file is reloaded. If you have any model selected while clicking on To Text File button, its path will be written at the end of the file. This can be very useful when you need to copy a model from different map, or when you need to copy multiple models. To access list of models in your Import.txt, simply go to Assist menu and click on From Text File. Then you can just start clicking on model names and models will appear on your mouse cursor or in model you have currently selected, or on selected coordinates, if Auto select mode in Edit menu is turned off (does anyone even use this thing?).

## So, why this tutorial? Where are those secret tips?
Yep, its a little bit strange to write them at the end of tutorial which was supposed to be dedicated to this, but I needed to explain stuff above. Import.txt is actually quite a powerful thing to use, as long as you can get list of models you want to import into Noggit, separated by line breaks, and filter them. Such lists do exist in WoW - they are called listfiles. In each MPQ, you can find (listfile) (as long as it wasn't made as listfile-free MPQ). (listfile) is a list of all file paths to all files in MPQ. And we will make a good use of this. First of all, download my Toolkit. Its a WIP application and poorly written, I've got to admit, but it works for what we are going to do with it absolutely fine. You can also use Amaroth's Toolkit.

Open your WMV. Go to File menu and select Export textures... option. In new panel which appears, make sure that Path preserved option is checked. Then just browse M2s. Everytime you find some you want to use, just click on Export all button in panel which have appeared. In the end, you will have model files in Export folder in WMV's directory, and they will be all in paths into which they belong.

Now, you have basically two options. You can either place cleanup.bat script into Export folder and run it. It will delete all .skin and .blp files in all directories in Export folder. If you want to change list of extensions which should be deleted, simply right-click that cleanup.bat, click on Edit and you surely will figure out how to change it on your own. Then create a new, empty MPQ and place all folders in Export directory into it. The second option is skipping whole cleanup.bat part and continuing to another paragraph of this tutorial.

When you have any sort of MPQ with (listfile) which contains files you don't want to use in Import.txt (like BLPs, SKINs or maybe even more stuff, it may be your server's patch you are giving to players or whatever else), you need to clean that (listfile) from all things you don't want to have there. To do this, just extract (listfile) from that MPQ and place it whenever you like. Then run my Toolkit application and set extracted (listfile) as input file. For this purpouse, you will want to use cleanup filter, which is that big panel on the left. Select extensions you want to keep in listfile (most likely .M2 and Root-only .WMO). You can also select path prefixes to keep only models from certain directory, for example, you can keep only files from World folders. You can also modify filters by using those 2 buttons and text input under filters, so you can add filter to World\Expansion02\ directory, for example. For more info, follow a Manual. When all your settings are done, just click on Cleanup Listfile button and save cleaned listfile whenever you want. If you have made a MPQ which already contains only M2 files, its (listfile) will, obviously, contain also only M2s (and (listfile) and (attributes) most likely), so you can basically use that (listfile)'s content without even filtering it by my Toolkit.

Now just open that cleaned and filtered listfile, press ctrl+a (or select just some particular models you wanted to use), ctrl+c, open Import.txt, ctrl+v, ctrl+s, use To Text File function in Noggit to reload import file and you are done, you have your models in Noggit.

This all can seem a little bit complicated, but when you learn how to do this and won't need to look any manual up for that, you will actually be able to quickly get lists of even hundreds or thousands file paths and filter them by using my Toolbox to get files you wanted, the only problem is exporting model files via WMV, if you need to do that (so, if you need to select just a few certain models rather then just use whole folder for example).

One final tip. You can also create a (listfile) of all file paths in WoW and then just always get only models from some certain directories by using my Toolbox. Lets say you want all World\Expansion02\Coldarra\ models. If you already have prepared merged (listfile) of all WoW files, my Toolkit is actually very fast and easy way of getting all those models directly into Import.txt. Just open Toolkit, choose merged (listfile) as an input file, write World\Expansion02\Coldarra\ under filters, click on Add path prefix button, make that filter checked, click on Cleanup Listfile and... done.

And one now really final tip. Extensions filter may be actually used also for ends of file names (like keg.m2, instead of just .m2). Path prefix filter can, on the other hand, be also used for name prefixes, like World/Custom/am_. Sadly, its (for now) limited for use on only one directory. This will get all am_ files, but only in World/Custom/ folder. Both extension and prefix filters aren't case-sensitive.

### Side Notes
In order to get listfile containing only M2 paths, you can take one shortcut which is faster than my cleaner in my toolkit. Go to your WMV's Export folder, open a notepad and copy following:

    del /s /q *.blp
    del /s /q *.skin

and save file as .bat file into Export folder. Then create a new, empty MPQ file and move it to that Export folder as well. Now, every single time you want to get some models into Noggit's Import file, export all those models from WVM like you normally would, but instead of using my cleaner just run .bat file. It will delete all blps and skins from Export directories (and following subdirectories) and thus leave only M2s, at least as long as you don't have any other garbage there. Then just place all remaining stuff into MPQ, export its lisfile and open it in Notepad. And you are done, because now you can just copy all M2 paths. So, basically, you need to use my toolkit only when you want to filter models or stuff like that.

The newest beta release of Noggit (and following ones) support WMV versions which place "," symbol directly behind file extension, which means that they are now compatible with most of WMV versions (at least with those WotLK ones).