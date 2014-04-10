# Crash while launching
If you have an edit which changes part of your character, world, or DBFilesClient and you crash when launching you need the Model Edit Fix for your appropriate patch and build.

# MPQ patch not working
If the game crashes, or doesn't, but your changes are not showing up, double check the tree paths in your MPQ to make sure they are the correct ones.

# Textures not working, model is completely white
When you change the models texture, but it ends up completely white, then simply there is no texture and the client doesn't bother looking for one. Double check your MPQ for errors.

# Models appear green in game
Sometimes when importing a new texture, or using a PTR file, your model turns green and has no textures visible whatsoever.
The client detects texture data, but it cannot be found.

# Error with xxx.dbc, expected 32 Columns and got 30
Never extract a DBC file as CSV because you lose two columns. Solution: Re-extract the file in DBC format.

# Cannot save file, data corrupt
When you make a creature-creature model change, you may often experience this error, the DBC becomes corrupted and is no longer usable. I'm not sure if it happens only with DBC Editor or with all DBC editing programs. Do not delete unneeded textures. Leave them as they are.

# Editing area names, cannot launch WoW
Sometimes when editing area names in AreaPOI.dbc you can't start up WoW, even with the Model Edit Fix. Use AreaTables.dbc instead.

# WDB: Crash when renaming
When you rename X to Y, and you have a spare letter, you edit it to a dot, but you crash. Solution: Do not edit it directly, edit the code instead, and change the spare letter to 01, 00 causes crashes.

# Fall to death when switching instances
Usually when you change instance X into instance Y, you will fall to death when zoning in. Edit Offset with Cryects Tools, or fill the ADT with water.

# Model change is not solid, but transparent
We're talking about famous Campfire - X modelchanges. Sometimes your model changes might be completely transparent and simply useless. Find a better non-transparent model to change into.

# DBC Editor or WoWModelViewer won't open
Download .NET Framework 2.0.

# BLPs are coming up Corrupted or Missing parts in photoshop 
Open the corrupted image up in photoshop first, then open it in paint. It will be fine in paint, copy and past it into photoshop and do you work.