# Change Login Screen Background #

There are two methods of achieving the same end result. It is recommended that you takes the Lua and XML route as it's easier and has less problems in the end to fix.

## UI Folders Method ##

Start by finding and taking the background image you intend to use for your new login screen and rescaling it to 1024 x 1024 pixels. From there save it to PNG as this is a must for it to work correctly when converting to BLP. After saving it, drag it onto BLPCoverter to convert it into a BLP. Once this is completed rename the file to:

**For Wrath of the Lich King:**

    WotLK_Login_Citadel01

Once this is done you can create a new MPQ using MPQ Editor and name the patch anything from Patch-A.MPQ to Patch-Z.MPQ, 1-9 works as well. Inside the MPQ the file structure should be:

    Patch-X.MPQ/Interface/GLUES/MODELS/UI_MAINMENU/Northrend

and

    Patch-X.MPQ/Interface/GLUES/MODELS/UI_MainMenu_Northrend

Drop WotLK_Login_Citadel01.blp into both "Northrend" and "UI_MainMenu_Northrend".

Now there might be some unwanted effects with various other aspects of the login screen. For wrath there's snow, the dragon and the skybox. All of these can be made transparent and repackaged into their directories correctly in the patch you made.

## Lua and XML Method ##