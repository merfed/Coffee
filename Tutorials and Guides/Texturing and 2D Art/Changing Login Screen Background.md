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

Firstly you need to extract the patched WoW.exe and interface folders into your wow directory. These are the basic steps, and must be completed before we can contine.

To set the background you just have to create a 1024x1024 PNG which will have to be atleast 75% of the 1024 pixels (768px height) to fill the entire screen. For an HD background make an image thats 2^x size in both dimensions. If you want to make a background with higher resolution just make one with: 1024x1024 = 2^10 x 2^10. 2^11 x 2^11 which will be: 2048x2048. Once you have finished editing your file save it into this directory:

	Interface\LoginScreen\

And name it *Background.png*. Now all you have to do is drag and drop the PNG onto BLPConverter and remove the PNG from the directory. If BLPConverter can't compress the BLP file, use BLP Lab v0.5.0, save as BLP2 and use the DXTC compression.

To change the background music, put the music file (.WAV) into this directory:

	Interface\LoginMusic\

Rename the file to *LoginScreen.wav*, after doing that open the file in any music player you had and write down the legenth of the song in seconds: ie. 2:25 is 145 seconds. With this number, open up *xv_vars_init.lua* located in *Interface/GlueXML/*, search for:

	VX_MUSICTIMER = GetTime() + <SECONDS>;

Remove the <SECONDS> part with your song length.

For the buttons you have to create 5 files:

	Glues-BigButton-Up 		    -> the normal button 
	Glues-BigButton-Down 	    -> the button pressed
	Glues-BigButton-Highlight   -> the button while your mouse is over it
	Glues-BigButton-Glow  	    -> the glow around the button
	Glues-BigButton-Rays  	    -> the pulsing rays when your mouse is over it

These files will be saved as .PNGs into:

	Interface/GLUES/COMMON/

With the filenames above, and then drop them all onto the BLP Converter, and remove the PNGs from the folder.

The positioning of the buttons can be changed in the *AccountLogin.XML* in *Interface/GlueXML/*.

	name="AccountLoginAccountEdit"          -> editbox for entering your accountname
	name="AccountLoginPasswordEdit"         -> editbox for entering your password
	name="AccountLoginLoginButton"          -> Login button
	name="AccountLoginExitButton"           -> Quit button
	name="OptionsButton"                    -> Options button
	name="AccountLoginManageAccountButton"  -> Manage Account button
	name="AccountLoginCommunityButton"      -> Official Site button
	name="AccountLoginSaveAccountNameText"  -> Save Account name checkbox text + checkbox

Now to move these you have to look out for these:

	<Anchor point="BOTTOM">
	       <Offset>
	       <AbsDimension x="..." y="..."/>
	       </Offset>
	</Anchor>

With the anchor point you can set the anchor to a specific destination ie. TOP, BOTTOMLEFT, RIGHT, etc. And with AbsDimension x and y you can set the exact coords of the destination.

Models can be added aswell. Open up *vx_vars_init.lua*. Lines 4-9 are the definitions of the model. The model gets added to the modelframe at the last 3 lines. In these lines you can change the lines 5-9 with the description behind (SetHeight and SetWidth only if you want to stretch your model, or cut of a piece of the model). If you want more than one model on your loginscreen, you have to copy the lines 4 - 9 and change every Mod1 in your copied block to Mod2 (if more than two then Mod3, ...) also you have to add a new "table.insert" behind (with a space and ; between) or under the first "table.insert"
in there you have to write: table.insert (M , Mod<YOUR MODEL NUMBER!>);
eg.: table.insert(M, Mod2); if you don't add the model to the table "M", the model will reset it's position on logout and returning to the AccountLogin screen.

Now to set the model you have to start your WoWModelViewer and find a model with only 1 skin (else they are white) then enter your full path of the model into the 2nd last line with double \\ and .m2 at the end

Final notes, try to set the position of the model in Z+1, most of the time it helps to see the model. Keep in mind that every model has different usage of the SetPosition command. eg.: some use the x and y axes to move in a 2D field and some use the y and z axes to move in a 2D field