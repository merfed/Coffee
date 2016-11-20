Make sure you read WHOLE manual for EACH tool before using it. Mass edits like this need to be done carefully, otherwise you are on a very good way to loose a lot of work.
Backup you database and DBC files and make always really sure that you are following exactly this manual correctly, step by step.


## Loading input files:
- You can either click on Load button or enter file's path into text box. You can also edit path which is currently in text box.
- File is in fact loaded when functions are executed, not before, so you don't need to click on anything after editing path in text box.


## Overwrite output file:
- Just a small note over here, make sure this is unchecked if you're going to just add new records to already existing files, commonly DBC CSVs.


## Cleanup Listfile:
This tool copies all paths from input (listfile) from your MPQ which match filters. Useful for Noggit Import and essential for other tools here.

- Open your MPQ and extract its (listfile). Use (listfile) as input file for this function.
- Select either (All) option or certain options for file extensions and file paths.
- You can use defualt options, or add your own by using 2 text boxes and 2 buttons under them, which are under filters.
- You can use whatever you want for path prefix filter. It may be just deafault World\, but you can be more specific, like World\Custom\Release5\
- Listfile Cleanup will create a new file with all paths in original (listfile) in it, which have matched filters.


## Listfile to Gameobject:
This tool creates (or edits) GameObjectDisplayInfo.dbc.csv file and SQL batch file, which creates template gameobjects with new displayIDs.

- Use filtered Listfile as input file for this tool. Filters should be set to M2 and Root-only WMO extensions, and to either (All) or more specific file paths.
- If you want to just add new displayIDs to your already existing DBC (most likely scenario), do following (otherwise skip --s):
-- Get your GameObjectDisplayInfo.dbc and convert it to CSV.
-- Copy your CSV to folder where Listfile you will use as input file is.
-- Make sure Overwrite output files is unchecked.

- Specify from which ID (entry) should created gameobjects start.
- Specify, from which displayID should created displays start. Check your DBC for last free displayID available, if you are just adding new displays.
- Gameobjects will get its model's name as name (without path).
- Optional: specify what should be added in front of each created gameobject's name. For example, I use [AmPatch]. Names of gobs would be like [AmPatch]building.wmo
- Optional: specify what should be added behund each created gameobject's name. For example [Version1]. Names of gobs might be looking then like this: [AmPatch]building.wmo[Version1]
- Run Listfile To Gameobject. You will be asked for path and name to output files. I recommend leaving default.
- In the end, you will get CSV file of your DBC. Name it GameObjectDisplayInfo.dbc.csv, if it doesn't have that name already. Covert it to DBC and DBC is done.
- SQL batch file must be executed on world database. Use SQLYog, Navicat, or similiar software for doing so.


## Item DBC to DB
This tool inserts all items which are in DBC, but are missing in DB, into DB. Items which are already there remain untouched. Useful when you want to get all those npc-only items.
However, this tool doesn't really set most of item stats and creates just generated item names, which means that you still have a lot to do after using it. Its fine for being used
once before using Item DB to DBC tool (I highly recommend doing so!) and for just browsing NPC items for some you might like to edit, finish and use on your own by hand.

- Get CSV version of your Item.dbc. Use it as input file.
- Optional: specify prefix of names of items. I recommend this, to mark all script-created items.
- Optional: turn Slot name in name and Entry in name options on. Both are recommended as well, to prevent you from getting thousands of items after typing one lookup command.
- Optional: specify postfix of item names. May be useful for distinguishing script-created items, or different editions from each other.
- In the end, name of generated items will be as following: (Prefix)(Item slot?) (Entry?)(Postfix). For example [DBCtoDB] Bracers 123478 [Edition1].
- Most likely you would like to check Overwrite output files option here. When done, you can just run a tool.
- Execute SQL batch file on your world database, and MAKE SURE you check "Continue on error" option. There will be error for each item you already have in DB, which is normal.
- Items are made just with default stats. I have tryed to make this task as easy for you as possible, still, you need to edit names and stats of items you want to use.


## Item DB to DBC
This tool generates (or edits) Item.dbc.csv file based on item_template.csv file - it synchronizes DBC with DB. With this tool, you don't need to ever touch Item.dbc again.
I highly recommend running Item DBC to DB tool once on your database before ever using this tool. Weapons, shields and holdable items which aren't in DB, but are in DBC,
are commonly wielded by NPCs. By using this tool without ever using DBC to DB tool you would loose all those items and all those NPCs would remain empty-handed.

- Open MySQL editor capable of exporting table data as CSV. For example Navicat.
- Export world.item_template table's content into CSV (in Navicat Export Wizard, called from table's right-click menu). Important notes:
-- Export only entry, class, subclass, SoundOverrideSubclass, displayid, InventoryType, Material, sheath fields. There should be 8 of them.
-- Keep fields in this order (if you have possibility to switch them, for example even by ALTERing table, you don't even need this tool...).
-- Make sure that Text Qualifier is set to None, and that your data is being exported as numbers, not as strings. There must be no " or ' in CSV.

- Use your item_template CSV as input file. You will most likely want to check Overwrite output file here, as you are making whole DBC based on your DB. Always use Overwrite output file option.
- Tool generates new CSV for you. Name it Item.dbc.csv (or just leave default name in dialog window), convert it to DBC and you are done.

## Credits
- Amaroth's Toolkit
- v0.4a 3.3.5a (12340) TC2 compatibility, released YYYY-MM-DD
- Coded by Amaroth
- Skype striker159753
- Mail Amaroth7@seznam.cz
- PM on Modcraft or MC-Net, you can find me on both sites with nick Amaroth.