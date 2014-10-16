## Creating Non-Animated M2 with MDLVis

**Tools Used**
* MDLVis 0.40, ENG Version
* ConverterMDX_Lazy.exe
* CollisionAdder.exe (Version 2011-10-27)
* Optional GIMP or Photoshop for texture editing

MDLVis can open WoW 1.x M2 models directly. So choose a model you like from any of the 1.x patches and open it up in my MDLVis. If you want more or less a blank model, delete every geoset except one. In the last one, leave a few vertice to make later editing possible. Save the file as an mdl, open the mdl file in a text editor and then follow the tutorial to convert MDX to M2's. Open in MDLVis again, check if everything looks okay, if so save as MDX. Finally try converting the file, if everything looks good in game you're done.

Further more, the MDLVis guide features more information on how to edit the model directly. Pretty much every feature is convered as it's designed from the incredible Warcraft 3 modding community.

Included you can find [m2_base_model.zip](files/m2_base_model.zip), which includes a m2 based on NewElfRuin02, which has 3 textures linked and some standing verticies.

### Verticies and Modeling

To add vertices, you can extrude from a face or a group of faces. Copy/paste vertice or faces is also very useful. Be careful to keep your model clean, delete free vertice, don't add unnecessary faces, keep the general triangle shapes as nice and simple as possible. The "optomize" function seems to work nicely, but all ways backup before you do any drastic changes.

### Textures

To add new textures, save the file as an mdl, and add the texture path in the mdl file and link it to the geoset. Some copy/pasting will do the trick. If a face you've just created from 3 verticie looks transparent even though it has a texture (F Button), select the face (click the middle of it) and click the "-1" button to filp the normals. If the group of faces looks darker/lighter than it's closest neighbours (if can happen if you detatch a group of faces as a new geoset for example), try selecting all those faces as well as some other ones nearby with the same orientation and click the "average normals" button.

### Other

Converting models:

`ConvertMDX_Lazy.exe mymodel.mdx 0.02`

Whic is just the conversion program, your model and then the scale. The last one is a little weird and you'll have to find whats right for your model.

For collisions, just copy the M2 model you've just converted and it's skin file in the collision_adder.exe (the 2011-10-27 modified version, which changes the way bounding box normals are created) folder, drop the M2 on the program and you're done.