# Model Editing with PyM2 #

To use PyM2 you need to include the files in your python code.

    from m2 import * 	# imports the whole m2.py file without the use of namespace
    from skin import *  # the same for skin
    import m2 			#imports m2.py, but you have to use eg m2.M2File to access the parts of the file

Now you can use the imported files.

**Example 1:** Displaying of the texture types

	#! /usr/bin/python
	from m2 import *

    # Dictionary for Texture types
    TextureTypes = {
		0 : "Hardcoded",
		1 : "Body/Clothes",
		2 : "Items",
		3 : "ArmorReflect?",
		6 : "Hair/Beard",
		8 : "Tauren fur",
		9 : "Inventory Art 1",
		10 : "quillboarpinata",
		11 : "Skin for creatures or gameobjects 1",
		12 : "Skin for creatures or gameobjects 2",
		13 : "Skin for creatures or gameobjects 3",
		14 : "Inventory Art 2"
	} 
    
    # open m2
    m2 = M2File("HumanMale.m2")

    # iterate through textures
    for i in m2.textures:
       print "Type is " + TextureTypes[i.type] #print texture type
       print i.name #print texture filename

**Example 2:** Change the Color of all particles to Green

	from m2 import *
	filename = "YourM2.m2" 
	m2 = M2File(filename ) #open YourM2.m2

	# iterate through the particles
	for i in m2.particle_emitters:

	# iterate through the elements of the Color-FakeAnimationBlock
   	for j in i.color.Keys:
      	j.x = 0 #red
      	j.y = 255 #green
      	j.z = 0 # blue
	m2.write(filename) #write the m2

**Example 3:** Display the Geoset Types

	from skin import *

	# Create Dictionary for Translating Ids
	GeosetTypes = {
		"00" : "Hairstyles",
		"01" : "Facial1",
		"02" : "Facial2",
		"03" : "Facial3",
		"04" : "Bracers",
		"05" : "Boots",
		"06" : "Unknown1",
		"07" : "Ears",
		"08" : "Wristbands",
		"09" : "Kneepads",
		"10" : "Unknown2",
		"11" : "Pants",
		"12" : "Tabard",
		"13" : "Trousers/Kilt",
		"14" : "Unknown3",
		"15" : "Cape",
		"16" : "Unknown4",
		"17" : "Eyeglows",
		"18" : "Belt"
	} 

	# Open SkinFile
	sk = SkinFile("VrykulMale00.skin")

	# Iterate through the Geosets
	for i in sk.mesh:
   		s = str(i.mesh_id)#Convert the GeosetId to a string
   		j = len(s)#Get the Length of the string
   		if (j<3):#If it's only two digits...
      		s = "00"#the Id is 00
   		elif(j<4):#if it's length is <4, then it has only one digit
      		s =  "0" + s[0]
   		else:#get the two important digits
      		s =  s[0:2]
   		print str(i.mesh_id) +" is " + GeosetTypes[s]#print mesh_id and it's translation

**Example 4:** Tilting is Tilt

	from m2 import *
	
	print "Please insert Modelname:" 
	filename = raw_input() #get input
	m2 = M2File(filename) #open m2
	print "Insert Tilting ( 0 = no tilt, 1 = x-tilt, 2 = y-tilt, 3 = xy-tilt)"
	tilt = int(raw_input()) #Get input and convert to int
	m2.hdr.modeltype |= tilt  #non-exclusive or: 001010 | 010001 = 011011
	m2.write(filename) #write m2