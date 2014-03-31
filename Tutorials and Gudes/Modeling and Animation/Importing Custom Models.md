# Importing Custom Models #

## 3DSMax ##

Somethings you'll need:

1. CollisionAdder
2. 3DSMax
3. Soulburn's ObjectDetacher
4. Glitchy's MES - change OBJ to M2.
5. BLPConverter

So lets get started. Make sure your model is finished, go into vertex mode, and select all your vertices. Go into edit vertices and select break. Click objectDetacher. This should detach every face from the current model. Doing this keeps our model texture from glitching in game.

![ScreenShot](/screenshots/latest.png)

**Note:** if the model has about 10k+ verts when you break them, detach a few at a time, otherwise 3DSMax might crash. This is a pretty intensive process and might take some time. Now your model is detached, put them all into a group and name it whatever you like. Now export to OBJ and put it into the same folder as your texture. Make sure your export options are:

**Geometry:**

- [X] Flip YZ-axis (Poser-like) 
- [ ] Shapes/Lines
- [ ] Hidden Objects
- Faces: Triangles
- [X] Texture coordinates
- [X] Normals
- [ ] Smoothing Groups
- Scale: 1.0

Material:
- [ ] Export material
- [ ] Create mat-library
- [ ] Force black ambient

Output:
- Target: PC/Win
- [ ] Relative Numbers
- Precision: 6

Optimize:
- [ ] vertex
- [ ] normals
- [ ] texture-coord

- [ ] Write log to Export-folder (when using #noPrompt in scripts)
- Preset: <NONE>

Then open up Glitchy's MES, open up your OBJ and BLP files in Glitchy's MES. Max bound is the size you want the model to be in game. Once you've done that click convert and save in the M2 section. You should have the M2 and Skin in your folder. If you want collision on your mode just drag and drop over CollisionAdder, then go back into MES and create an MPQ. For M2 path, find the models path and want to change, once you have done all that click make MPQ and test it out in game,