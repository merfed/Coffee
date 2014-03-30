# Loading Screen #

This is how to modify the loading screen to an image of your choice. Start by creating a new image (1280x1024) in photoshop. Keep in mind there will be a loading bar added to your image in-game. Reize your image to 1024x1024. Save it as a PNG and convert it to a BLP and rename it to:

	LoadScreeenYourMapNameHere.blp

Open up patch-enUS-3.MPQ (enUS depends on your version of WoW and localization) and extract the Map.dbc and LoadingScreend.dbc from the DBFilesClient folder.

Open up the LoadingScreens.dbc with Taliis and go to the strings table. There you add two new strings; one with the name of your map and one with a path like this:

	Interface\Glues\LoadingScreens\LoadScreenYourMapNameHere.blp

Write down the two new IDs you got and head on to the Table table. Right click on the last line and click clone selected row. A new row will pop up; change #0 to the previous row ID+1 and write this value down, it's the most important one. In #1 you enter the first string number you created, in #2 the second one, for the path. Ignore the last column and save your file.

Opening up the Map.dbc in Taliis and find the row of the level you want to change the loading screen. It can be a good help to at first find the map ID in game or on list elsewhere. Now scroll to column #57 and chanfge the number that is there to the number sued as the LoadingScreen ID (remember, the important step from above). Create a new folder like this: Interface\Glues\LoadingScreens\ and put your BLP into there. Then make another folder named DBCFilesClient and put Map.dbc and LoadingScreen.dbc into this one. Pack everything up together into patch-enUS-3.mpq and enjoy your brand new loading screen.