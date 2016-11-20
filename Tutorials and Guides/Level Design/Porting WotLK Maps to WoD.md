## Requirements
* MyDBCEditor
* Pagdzin & Mjollná ADT converter
* PhilipTNG's ADT LK to MoP/WoD Converter
* Rangorn's WdtSupplies
* A WoD Server

## Modify DBC Files
Go to your server's folder and copy the "Map.dbc" and "MapDifficulty.dbc" to your DBFilesClient in your WoD Folder. Now right click the Azeoth entry, and select "Copy line to". Now write in the Map ID you want to use. I am gonna use the map id 3000. Once copied, it should show you the new map id you have added. Now we need to modify it to your needs.

The first column says "Azeroth" This is the internal name aka what you want the map folder to be called. I am gonna call it "SuperCoolLand". The next name you will see is "Eastern Kingdoms" This is the ingame name, choose whatever you want here. There is alot of different columns, but for now let's save it and open up MapDifficulty.dbc

Now this may look complicated, but it's actually pretty easy. Right click the ID column and click "Insert Line". Now write an unique id, im gonna pick 5000. 
You should now be at your new line. The second column is gonna show a "0". You will need to change that to your map id. So im gonna write 3000. 

Save the file and copy the your modified dbc files to the server's dbc folder. Now you should have your dbc files both in the client and the server.

## Converting the ADTs
There are two ways of converting the adt's. Pagdzin & Mjollná ADT converter or PhilipTNG's ADT LK to MoP/WoD Converter.

Pagdzin & Mjollná's is the fastest one. But it only works with blizzard ADTs or older noggit modified ADTs

PhilipTNG's works on all ADTs, but is quite slow at converting them.

If you are unsure which one to use, Philip's is usually the best one. I'm gonna tell how to use both.

### Using Pagdzin & Mjollná ADT converter 
Download the tools and extract the folder to wherever you want. Open the "adtconverter.conf" and modify the "directory=" to where you want the converted adt's to be written.

Save the file and open up "ADT Converter.jar". You can now choose if you want "Retail" or "World building". I'm gonna select the second option.

Now click file, open and navigate to the directory with your WotLK adt's. Once you have found it, click "Open" and let the converter do it's thing. Once it's done, you should be able to find your freshly made adt's in the folder to choosed above.

Make sure that for every adt that there is an:

    _tex0.adt
    _tex1.adt
    _obj0.adt
    _obj1.adt

If the tex1 and obj1 is missing, just copy the tex0 and obj0 and rename them to tex1 and obj1.  Copy the adt's to your WoD folder's "World\Maps\Whatever your continent is called"

### Using PhilipTNG's ADT LK to MoP/WoD Converter
Once you have downloaded the tools, extract the folder to the root of your C: drive (this is important!). The path should be like this:

    C:\PTNG_ADT_Converter

Now, copy your ADT files to the "ToBeVerted" folder. Once they have been copied, run the "WC_ADT_Convert.bat" file.

This takes longer depending on how many adt's you have. 
While it's running, don't copy or paste anything, or you might break something. Once it's done it's thing. You should now have some new adt's in the "Finished" folder. Make sure that for 

every adt that there is an:

    _tex0.adt
    _tex1.adt
    _obj0.adt
    _obj1.adt
    
If the tex1 and obj1 is missing, just copy the tex0 and obj0 and rename them to tex1 and obj1. Copy the adt's to your WoD folder's "World\Maps\Whatever your continent is called"

## Creating other needed stuff
Now we need to use WdtSupplies to create some other needed files for WoD.

Extract WdtSupplies and move your wdt file to the same folder. Open a new command prompt at the same location (Shift+Right click and select "New command prompt here") and write the following:

    java -jar WdtSupplies.jar NameOfWdt.wdt

The Program will now generate the following files:

    *.wdl
    *.tex
    *_occ.wdt
    *_lgt.wdt

It might take a few minutes, so be patient. 
Once done, copy the new files to your continent folder.
Now launch up your server, teleport to your continent and it should work!

## FAQ
**"I Used Pagdzin & Mjollná ADT converter but there is no adt's in the output folder?"**

Make sure that the directory is the same, as you wrote in adtconverter.conf. If that's correct, try use the other option (Retail). If it still doesn't work, try use the other converter.

**"Terrain is green in the distance, but when i get closer it gets it right texture."**

This is caused by the *.tex file, it's for low resolution texture. The only way to fix this, is to take an tex file from another map, that uses the same textures as your own.

**"How do i port my WotLK maps to Cata or MoP?"**

For Cata and MoP, you don't need to modify MapDifficulty.dbc and you don't need the _occ and _lgt file. Everything else is mostly the same.