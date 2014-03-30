-- Script to import M2 files found in the mpq files of world of warcraft
-- into GMax or 3d studio max. The script loads vertices with uv coords 
-- and submeshes as found in the M2 file. Textures are not loaded but can be 
-- assigned in a later step manually. The submeshes are created with dummy cubes 
-- as bones for skinning.
--
-- INSTALLATION
-- Place me in 3dsmax7\stdplugs\stdscripts\
-- restart 3ds max, find WOW menu on main menu bar
--
-- If menu item don't work normaly or do not exists use RunScript button from MaxScript menu and run this scipt file.
--
-- UI work and animation bug fixed by me (aka DeLazarus), April 2006.
-- Modifications was tested only on 3ds max 7
--
-- Some models imports run very slowly. Be brave :)
--
-- Injoy!
--
-- Incorporated fixes to load the WoW 2.0 models.
-- by Darjk

-- GLOBALS AND STRUCTURES


global filename = "D:\\mpq\\cauldron\\PlagueCauldronActive.m2"

-- there are no any warrnings of situation like this: LoadAnimation=true LoadBones=false, UI must work over this
-- and btw some models do not load if bones are not loaded. That is why I froze its check box
global LoadBones = true
global LoadAnimation = false

global head
global bstream
global step

global verts_read 
global views_read
global view_inds_read
global view_tris_read 
global bones_read 
global name_read
global sequences_read 
global animations_read 

fn InitLoader = 
(
	verts_read = #()
	views_read = #()
	view_inds_read = #()
	view_tris_read = #()
	bones_read = #()
	name_read = ""
	sequences_read = #()
	animations_read = #()
)

struct WOW2_Header
(
	magic,version,namelen,ofsname,
	un1,nGlobalSequences,ofsGlobalSequences,nAnimations,
	ofsAnimations,nC,ofsC,nD,ofsD,nBones,ofsBones,nF,ofsF,
	nVertices,ofsVertices,nViews,ofsViews,nG,ofsG,nTextures,
	ofsTextures,nH,ofsH,nI,ofsI,nJ,ofsJ,nK,ofsK,nX,ofsX,
	nBoneGroups,ofsBoneGroups,nTexLookup,ofsTexLookup,nL,ofsL,nM,ofsM,nN,
	ofsN,n14floats,nBoundingTriangles,ofsBoundingTriangles,
	nBoundingVertices,ofsBoundingVertices,nBoundingNormals,
	ofsBoundingNormals,nO,ofsO,nP,ofsP,nQ,ofsQ,nR,ofsR,nS,
	ofsS,nT,ofsT,nU,ofsU,nV,ofsV
	--nG Magic1 ofsG Magic2 
)

struct WOW2_Vertex
(
	pos,bw1,bw2,bw3,bw4,bi1,bi2,bi3,bi4,normal,uv,n2floats
)

struct WOW2_Views
(
	nindex,ofsnindex, ntris,ofsntris,nverts,ofsnverts,
	nsubmesh,ofsnsubmesh,ntextures,ofsntextures,unknown1,
	ind,tri -- loaded vert and tri index
)

-- nAnimations 
struct WOW2_Animations
(
	animid,startseq,endseq,speed,unknown1,unknown2,unknown3,
	unknown4,unknown5,bbox,radius,unknown6,unknown7
)

-- the actual animation info used for different animation types
struct WOW2_Anim
(
	type,flags,linetypes,linetypeofs,frames,framesofs,data,dataofs
)

-- bones including animation data
struct WOW2_Bones
(
	geosetanim,flags,unknown0,parentbone,geoset,
	trans,rot,scale,pivot,max_name
)

struct WOW2_BoneWeightIndex
(	
	cnt,bw,bi
)

-- one submesh part with its verts stored in vs, faces in fs, textures in ts 
-- and boneidexweights in bs, bones holds bone raw data
struct WOW2_Submesh
(
	id,ofsvert,nverts,oftri,tris,unkown1,unknown2,
	unknown3,unknown4,n3floats,
	vs,fs,ts,bs,bones
)

struct WOW2_Texture
(
	flags,submesh1,submesh2,color,texpass,texunit,unknown1,
	lookupindex,texunit2,lightemitt
)

struct WOW2_Texturefile
(
	-- type:
	--0  Texture given in filename  
	--1  Body + clothes  
	--2  Cape  
	--6  Hair, beard  
	--8  Tauren fur  
	--11  Skin for creatures  

	type,flags,namelen,ofsname  
)

fn echo msg =
(
	format "%\n" (msg) to:listener
)

fn ReadFixedString bstream fixedLen=
(
	local str = ""
	for i = 1 to fixedLen do
	(
		str += bit.intAsChar (ReadByte bstream #unsigned)
	)
	str
)

fn SkipBytes bstream count=
(
	local unknown
	case count of
	(
		2: unknown = ReadShort bstream #unsigned
		4: unknown = ReadLong bstream #unsigned
		default:
		(
			for i = 1 to count do
			(
				unknown = ReadByte bstream #unsigned
			)
		)
	)
)
	
fn LongToString num=
(
	local str = ""
	for i = 1 to 4 do
	(
		str += bit.intAsChar (bit.and num 0xff)
		-- num = bit.shift num -8
		num /= 256
	)
	str
)

fn ShortToFloat s16 =
(	
	if s16 < 0 then
		f32 = (s16 + 32768.0)
	else
		f32 = (s16 - 32767.0)
	
	f32 /= 32767.0
	return f32
)


fn WOW2_Open fname =
(
		bstream = fopen fname "rb"
		head = WOW2_Header ()
 )	 
	
fn WOW2_Close =
(
	step = "Close"
	fclose bstream
)
	
	
	fn WOW2_Read_Header =
	(
		if head == undefined or bstream == undefined then
			echo "Nothing to read"

		format "reading\n" to:listener
		step = "Read header"
		head.magic = ReadLong bstream #unsigned
		echo ("4cc: "+(LongToString head.magic))
		head.version = ReadLong bstream #unsigned
		echo ("Version: "+(LongToString head.version))
		head.namelen = ReadLong bstream #unsigned
		--echo ("Name len:"+ (head.namelen as string))
		head.ofsname = ReadLong bstream #unsigned
		--echo ("Name offs:"+ head.ofsname as string)
		head.un1 = ReadLong bstream #unsigned
		--echo (LongToString head.un1)
		--echo ("before glob ftell: "+(ftell bstream) as string)
		head.nGlobalSequences = ReadLong bstream #unsigned
		--echo ("Global seq:"+head.nGlobalSequences as string)
		head.ofsGlobalSequences = ReadLong bstream #unsigned
		--echo ("Global seq ofs:"+ head.ofsGlobalSequences as string)
		--echo ("before anims: "+(ftell bstream) as string)
		head.nAnimations = ReadLong bstream #unsigned
		--echo ("Anims :"+ head.nAnimations as string)
		head.ofsAnimations = ReadLong bstream #unsigned
		--echo ("Anims ofs:" + head.ofsAnimations as string)
		head.nC = ReadLong bstream #unsigned
		--echo (LongToString head.nC)
		head.ofsC = ReadLong bstream #unsigned
		--echo ("ofsC:" + head.ofsC as string)
		head.nD = ReadLong bstream #unsigned
		--echo (LongToString head.nD)
		head.ofsD = ReadLong bstream #unsigned
		--echo ("ofsD:" + head.ofsD as string)
		head.nBones = ReadLong bstream
		--echo ("Bones:"+head.nBones as string)
		head.ofsBones = ReadLong bstream #unsigned
		--echo ("Bones ofs:"+head.ofsBones as string)
		head.nF = ReadLong bstream #unsigned
		--echo ("nF:" + head.nF as string)
		head.ofsF = ReadLong bstream #unsigned
		--echo ("ofsF:" + head.ofsF as string)
		--echo ("before verts ftell: "+(ftell bstream) as string)
		head.nVertices = ReadLong bstream #unsigned
		--echo ("Vertices:"+head.nVertices as string)
		head.ofsVertices = ReadLong bstream #unsigned
		--echo ("Vertices of:"+head.ofsVertices as string)
		head.nViews = ReadLong bstream #unsigned
		--echo ("Views:"+head.nViews as string)
		head.ofsViews = ReadLong bstream #unsigned
		--echo ("ofsViews:"+head.ofsViews as string)
		head.nG = ReadLong bstream #unsigned
		head.ofsG = ReadLong bstream #unsigned
		--echo ("ofsG:"+head.ofsG as string)
		head.nTextures = ReadLong bstream #unsigned
		--echo ("Textures:"+head.nTextures as string)
		head.ofsTextures = ReadLong bstream #unsigned
		--echo ("ofsTex:"+head.ofsTextures as string)
		head.nH = ReadLong bstream #unsigned
		head.ofsH = ReadLong bstream #unsigned
		--echo ("ofsH:"+head.ofsH as string)
		head.nI = ReadLong bstream #unsigned
		head.ofsI = ReadLong bstream #unsigned
		--echo ("ofsI:"+head.ofsI as string)
		head.nJ = ReadLong bstream #unsigned
		head.ofsJ = ReadLong bstream #unsigned
		--echo ("ofsJ:"+head.ofsJ as string)
		head.nK = ReadLong bstream #unsigned
		head.ofsK = ReadLong bstream #unsigned
		--echo ("ofsK:"+head.ofsK as string)
		head.nX = ReadLong bstream #unsigned
		head.ofsX = ReadLong bstream #unsigned
		--echo ("ofsX:"+head.ofsX as string)
		head.nBoneGroups = ReadLong bstream #unsigned
		head.ofsBoneGroups = ReadLong bstream #unsigned
		echo ("BoneGroups:"+head.nBoneGroups as string)
		head.nTexLookup = ReadLong bstream #unsigned
		--echo ("TexLookup:"+head.nTexLookup as string)
		head.ofsTexLookup = ReadLong bstream #unsigned
		--echo ("ofsTexlkup:"+head.ofsTexLookup as string)
		head.nL = ReadLong bstream #unsigned
		head.ofsL = ReadLong bstream #unsigned
		--echo ("ofsL:"+head.ofsL as string)
		head.nM = ReadLong bstream #unsigned
		head.ofsM = ReadLong bstream #unsigned
		--echo ("ofsM:"+head.ofsM as string)
		head.nN = ReadLong bstream #unsigned
		head.ofsN = ReadLong bstream #unsigned
		---echo ("ofsN:"+head.ofsN as string)
		head.n14floats = undefined
		for i = 1 to 14 do
		( 
			undef = ReadFloat bstream
		)
		head.nBoundingTriangles = ReadLong bstream #unsigned
		head.ofsBoundingTriangles = ReadLong bstream #unsigned
		--echo ("ofsBoundTri:"+head.ofsBoundingTriangles as string)
		head.nBoundingVertices = ReadLong bstream #unsigned
		head.ofsBoundingVertices = ReadLong bstream #unsigned
		--echo ("ofsBoundingVerts:"+head.ofsBoundingVertices as string)
		head.nBoundingNormals = ReadLong bstream #unsigned
		head.ofsBoundingNormals = ReadLong bstream #unsigned
		--echo ("ofsBoundNormals:"+head.ofsBoundingNormals as string)
		head.nO = ReadLong bstream #unsigned
		head.ofsO = ReadLong bstream #unsigned
		--echo ("ofsO:"+head.ofsO as string)
		head.nP = ReadLong bstream #unsigned
		head.ofsP = ReadLong bstream #unsigned
		--echo ("ofsP:"+head.ofsP as string)
		head.nQ = ReadLong bstream #unsigned
		head.ofsQ = ReadLong bstream #unsigned
		--echo ("ofsQ:"+head.ofsQ as string)
		head.nR = ReadLong bstream #unsigned
		head.ofsR = ReadLong bstream #unsigned
		--echo ("ofsR:"+head.ofsR as string)
		head.nS = ReadLong bstream #unsigned
		head.ofsS = ReadLong bstream #unsigned
		--echo ("ofsS:"+head.ofsS as string)
		head.nT = ReadLong bstream #unsigned
		head.ofsT = ReadLong bstream #unsigned
		--echo ("ofsT:"+head.ofsT as string)
		head.nU = ReadLong bstream #unsigned
		head.ofsU = ReadLong bstream #unsigned
		--echo ("ofsU:"+head.ofsU as string)
		head.nV = ReadLong bstream #unsigned
		head.ofsV   = ReadLong bstream #unsigned
		--echo ("ofsV:"+head.ofsV as string)
		echo "---- Header finished ----"
		ok
	)


fn WOW2_Read_Name =
(
	step = "Read Name"
	if (fseek bstream head.ofsname #seek_set ) then
	(
		name_read = (ReadFixedString bstream head.namelen)
		echo name_read	
	)
	else
		echo "unable to read name"
)

fn WOW2_Read_Verts =
(
	step = "Read Verts"
	if (fseek bstream head.ofsVertices #seek_set ) then
	(
		step = "Read verts prep"
		
		for i=1 to head.nVertices  do
		(
			local vert = WOW2_Vertex ()
			local v4 = [0.0,0.0,0.0]
			local v3 = [0.0,0.0,0.0]
			local v2 = [0.0,0.0,0.0]
			local v1 = [0.0,0.0]
			
			v4.x = ReadFloat bstream
			v4.y = ReadFloat bstream
			v4.z = ReadFloat bstream
			vert.pos = v4
			vert.bw1 = ReadByte bstream #unsigned
			vert.bw2 = ReadByte bstream #unsigned
			vert.bw3 = ReadByte bstream #unsigned
			vert.bw4 = ReadByte bstream #unsigned
			vert.bi1 = ReadByte bstream #unsigned
			vert.bi2 = ReadByte bstream #unsigned
			vert.bi3 = ReadByte bstream #unsigned
			vert.bi4 = ReadByte bstream #unsigned
			v3.x = ReadFloat bstream
			v3.y = ReadFloat bstream
			v3.z = ReadFloat bstream
			vert.normal = v3
			v2.x = ReadFloat bstream
			v2.y = ReadFloat bstream
			v2.z = 0.0 --is empty (0.0)
			vert.uv = v2
			v1.x = ReadFloat bstream
			v1.y = ReadFloat bstream
			vert.n2floats = v1

			append verts_read vert
		)	
		
--		echo ("Verts read: " + verts_read.count as string )
	)
	else
		echo "unable to read vertices"
)

fn WOW2_Read_Views =
(
	step = "Read Views"
	if (fseek bstream head.ofsViews #seek_set ) then
	(
		for i=1 to head.nViews  do
		(
			local view = WOW2_Views ()
			view.nindex = ReadLong bstream #unsigned
			view.ofsnindex = ReadLong bstream #unsigned
			view.ntris = ReadLong bstream #unsigned
			view.ofsntris = ReadLong bstream #unsigned
			view.nverts = ReadLong bstream #unsigned
			view.ofsnverts = ReadLong bstream #unsigned 			
			view.nsubmesh = ReadLong bstream #unsigned
			view.ofsnsubmesh = ReadLong bstream #unsigned
			view.ntextures = ReadLong bstream #unsigned
			view.ofsntextures = ReadLong bstream #unsigned
			view.unknown1 = ReadLong bstream #unsigned
			
			append views_read view
			step = "Read Views Mod 3"
			if((mod (view.ntris) 3) != 0.0 )then
				echo ("  View["+i as string+"].ntris is not a multiple of 3!")
			
			if false then --i == 1 then
			(	echo ("\nView nind  : "+view.nindex as string)
				echo ("View ofsind : "+view.ofsnindex as string)
				echo ("View ntris : "+view.ntris as string)
				echo ("View ofsntris : "+view.ofsntris as string)
				echo ("View nverts: "+view.nverts as string)
				echo ("View ofsnverts : "+view.ofsnverts as string)
				echo ("View nsubm : "+view.nsubmesh as string)
				echo ("View ntex  : "+view.ntextures as string)
			)
		)
	)
	else
		echo "unable to read views"
		
	step = "Read View index lists"
	--load tri and vert index lists
	for i=1 to views_read.count do
	(
		local ind = #()
		local tri = #()
		step = "Read View index"
		if (fseek bstream (views_read[i].ofsnindex) #seek_set ) then
		(
			for j = 1 to views_read[i].nindex do
			(
				local x = ReadShort bstream #unsigned
				append ind x
			)
			views_read[i].ind = ind
		)
		else
			echo "unable to read view indices"
		
		step = "Read View faces"
		if (fseek bstream views_read[i].ofsntris #seek_set ) then
		(
			for j= 1 to views_read[i].ntris do
			(
				local x = ReadShort bstream #unsigned
				append tri x
			)
			views_read[i].tri = tri
		)
		else
			echo "unable to read view faces"
	)
)

/*
fn WOW2_Read_Bone_Groups =
(
--nBoneGroups,ofsBoneGroups
	bonegroups = #()
--echo ("bonegroups:"+head.nBoneGroups as string)
	if (fseek bstream head.ofsBoneGroups #seek_set ) then
	(
		for i = 1 to head.nBoneGroups do
		(
			local id = (ReadShort bstream)
--echo ("bonegroupID:"+id as string)
			append bonegroups id
		)
	)
	
	return bonegroups
)
*/
fn WOW2_Create_Bones bonearr =
(
	step = "Bone creation"
	-- previous bone must have been created
	for i = 1 to bonearr.count do
	(
		Dummy position:(bonearr[i].pivot) isSelected:on name:(bonearr[i].max_name)
		$.boxsize = [0.1,0.1,0.1]
		--$.pos.controller = TCB_position ()
		$.rotation.controller = Local_Euler_XYZ ()
		--$.scale.controller = TCB_scale ()
		if (bonearr[i].parentbone >= 0) then
		(
			$.parent = getNodeByName \
				(bonearr[(bonearr[i].parentbone + 1)].max_name) \
				exact:true
		)
	)
)
-- read bone animation data frames
-- bone arrays should be already filled
-- @param filled WOW2_Bones array 
-- @TODO finish this function
fn WOW2_CreateBoneScaleAnim ba =
(
	--echo(ba.max_name)
	
	if ba.scale.frames > 0 then
	(
		step = "Framesoffset reading"
		if (fseek bstream ba.scale.framesofs #seek_set ) then
		(
			local t = #()
			local maxx = 0
			local x = 0
			currobj = getNodeByName (ba.max_name) exact:true
			step = "Reading Translation frames"
			for j = 1 to ba.scale.frames do
			(
				append t (ReadLong bstream )
if t[j] > maxx then
 maxx = t[j]				
			)
			step = "Dataoffset reading"
			if (fseek bstream ba.scale.dataofs #seek_set ) then
			(
--echo ("max Anim ="+maxx as string)
				step = "Reading Translation data"
				animationRange = interval 10 maxx
				with animate on
				(
					for j = 1 to ba.scale.data do
					(
--step = "1"				
						local v3 = [0.0,0.0,0.0]
						v3.x = (ReadFloat bstream )
						v3.y = (ReadFloat bstream )
						v3.z = (ReadFloat bstream )
--step = "2"


						if( j > 1 ) then -- let him to create start keys himself
						(
							addNewKey currobj.scale.controller t[j] 
							currobj.scale.controller.keys[j+1].value = currobj.scale.controller.keys[1].value
						)

						in coordsys local
							at time (t[j]) 
								currobj.scale = v3
					)
				)
			)
			else
				echo "Failed to read bstream start for bone translation frames"
		)
		else
			echo "Failed to read bstream start for bone translation data"	
	)
)

-- read bone animation data frames
-- bone arrays should be already filled
-- @param filled WOW2_Bones array 
-- @TODO finish this function
fn WOW2_CreateBoneTransAnim ba =
(
	if ba.trans.frames > 0 then
	(
		step = "Framesoffset reading"
		if (fseek bstream ba.trans.framesofs #seek_set ) then
		(
			local t = #()
			local maxx = 0
			local x = 0
			currobj = getNodeByName (ba.max_name) exact:true
			step = "Reading Translation frames"
			for j = 1 to ba.trans.frames do
			(
				append t (ReadLong bstream )
if t[j] > maxx then
 maxx = t[j]				
			)
			step = "Dataoffset reading"
			if (fseek bstream ba.trans.dataofs #seek_set ) then
			(
--echo ("max Anim ="+maxx as string)
				step = "Reading Translation data"
				animationRange = interval 10 maxx
				with animate on
				(
					for j = 1 to ba.trans.data do
					(
--step = "1"				
						local v3 = [0.0,0.0,0.0]
						v3.x = (ReadFloat bstream )
						v3.y = (ReadFloat bstream )
						v3.z = (ReadFloat bstream )
--step = "2"

						if( j > 1 ) then
						(
							addNewKey currobj.pos.controller.X_position.controller t[j] 
							addNewKey currobj.pos.controller.Y_position.controller t[j] 
							addNewKey currobj.pos.controller.Z_position.controller t[j] 
							currobj.pos.controller.X_position.keys[j+1].value = currobj.pos.controller.X_position.keys[1].value
							currobj.pos.controller.Y_position.keys[j+1].value = currobj.pos.controller.Y_position.keys[1].value
							currobj.pos.controller.Z_position.keys[j+1].value = currobj.pos.controller.Z_position.keys[1].value
						)

						in coordsys local
							at time (t[j]) 
								currobj.pos =  v3
					)
				)
			)
			else
				echo "Failed to read bstream start for bone translation frames"
		)
		else
			echo "Failed to read bstream start for bone translation data"	
	)
)

-- read bone animation data frames
-- bone arrays should be already filled
-- @param filled WOW2_Bones array 
-- @TODO finish this function
fn WOW2_CreateBoneRotateAnim ba =
(
	if ba.rot.frames > 0 then
	(
		step = "Framesoffset reading"
		if (fseek bstream ba.rot.framesofs #seek_set ) then
		(
			local t = #()
			local maxx = 0
			local x = 0
			currobj = getNodeByName (ba.max_name) exact:true
			step = "Reading Rotation frames"
			
			for j = 1 to ba.rot.frames do
			(
				append t (ReadLong bstream )
				if (t[j] > maxx) then
					maxx = t[j]				
			)
			
			step = "Dataoffset reading"
			if (fseek bstream ba.rot.dataofs #seek_set ) then
			(
				--echo ("max Anim ="+maxx as string)

				step = "Reading Rotation data"
				
				animationRange = interval 10 maxx
				with animate on
				(
					
					for j = 1 to ba.rot.data do
					(
						--step = "1"		
						--step = "Reading Rotation data - 1"

						local q = quat 0.0 0.0 0.0 0.0
						
						local a = ReadShort bstream
						local b = ReadShort bstream
						local c = ReadShort bstream
						local d = ReadShort bstream
						
						q.x = ShortToFloat a
						q.y = ShortToFloat b 
						q.z = ShortToFloat c 
						q.w = ShortToFloat d 
						
						--step = "2"
						--step = "Reading Rotation data - 2"
						local b = (quatToEuler q) 
						
						if ((b.x > 360) or (b.y > 360) or (b.z > 360)) then
							echo ("oversized:"+q as string + " = "+ (quatToEuler q) as string)

						if( j > 1 ) then
						(
							try
							(
								--step = "3"
								--step = "Reading Rotation data - 3"
								addNewKey currobj.rotation.controller.local_x_rotation.controller t[j] 
								addNewKey currobj.rotation.controller.local_y_rotation.controller t[j] 
								addNewKey currobj.rotation.controller.local_z_rotation.controller t[j] 
								
								--echo (t[j] as string)
								
								-- No idea what this is suppose to do
								--echo (currobj.rotation.controller.local_x_rotation.keys[j].value as string + " : " + currobj.rotation.controller.local_y_rotation.keys[j].value as string + " : " + currobj.rotation.controller.local_z_rotation.keys[j].value as string)
								--echo (currobj.rotation.controller.local_x_rotation.keys.count as string)
								if (currobj.rotation.controller.local_x_rotation.keys.count >= (j+1)) then
								(
									currobj.rotation.controller.local_x_rotation.keys[j+1].value = currobj.rotation.controller.local_x_rotation.keys[1].value
									currobj.rotation.controller.local_y_rotation.keys[j+1].value = currobj.rotation.controller.local_y_rotation.keys[1].value
									currobj.rotation.controller.local_z_rotation.keys[j+1].value = currobj.rotation.controller.local_z_rotation.keys[1].value
								)
							)
							catch
							(
								echo (getCurrentException())
								throw ()
							)
						)

						in coordsys local
							at time (t[j]) 
								currobj.rotation = q
					)
				)
--echo "-----------#########------------------"
			)
			else
				echo "Failed to read bstream start for bone rotation frames"
		)
		else
			echo "Failed to read bstream start for bone rotation data"	
	)
)


fn WOW2_Create_BoneAnimations ba =
(
	for i = 1 to ba.count do
	(
		step = "Compare of Data and Frame counts"
		if ba[i].trans.frames != ba[i].trans.data then
			echo ("Translate frames not even on bone #" + i as string)

		WOW2_CreateBoneTransAnim (ba[i])

		if ba[i].rot.frames != ba[i].rot.data then
			echo ("Rotate frames not even on bone #" + i as string)

		WOW2_CreateBoneRotateAnim (ba[i])
		
		if ba[i].scale.frames != ba[i].scale.data then
			echo ("Scale frames not even on bone #" + i as string)
		if ba[i].scale.frames > 0 then
			echo ("bone scaling found on bone "+ ba[i].max_name)
		WOW2_CreateBoneScaleAnim (ba[i])
	)
)

-- read bone animation information
-- ofsets and types for the animations
-- identical for scale, rotate, translate
-- the final data is found starting at dataofs
-- expects the bstream pointer to be at the correct position
-- @see WOW2_ReadBones
-- @return A filled WOW2_Anim structure
fn WOW2_Read_BoneAnim =
(
	local a = WOW2_Anim ()
	a.type = (ReadShort bstream )
	a.flags = (ReadShort bstream )
	a.linetypes = (ReadLong bstream )
	a.linetypeofs = (ReadLong bstream )
	a.frames = (ReadLong bstream )
	a.framesofs = (ReadLong bstream )
	a.data = (ReadLong bstream )
	a.dataofs = (ReadLong bstream )

	return a
)

fn WOW2_Read_Bones =
(
	step = "Read Views"
	if ( head.nBones <= 0 ) then  -- static models do not have bones
	(
		echo "--model without bones !"
		return true
	)
	
	local bonearr = #()
	if (fseek bstream head.ofsBones #seek_set ) then
	(
		for i = 1 to head.nBones do
		( 
			step = ("Reading Bone #"+ i as string)
			local bone = WOW2_Bones ()
			local piv = [0.0,0.0,0.0]
			
			bone.geosetanim = (ReadLong bstream ) 
			bone.flags = (ReadShort bstream )
			bone.unknown0 = (ReadShort bstream )
			bone.parentbone = (ReadShort bstream ) -- -1 or bone # (0 is first)
			bone.geoset = (ReadShort bstream )
			
			-- WoW 2.0 Change
			SkipBytes bstream 4
			
			step = ("Reading Bone #"+ i as string + " anims")
			bone.trans = WOW2_Read_BoneAnim ()
			bone.rot = WOW2_Read_BoneAnim ()
			bone.scale = WOW2_Read_BoneAnim ()
			piv.x = (ReadFloat bstream )
			piv.y = (ReadFloat bstream )
			piv.z = (ReadFloat bstream )
			bone.pivot = piv
			bone.max_name = ("bone_"+ i as string + "_" + bone.flags as string)
			step = ("Appending Bone #"+ i as string)
			append bonearr bone
		)
		

	)
	else
		echo "unable to read bones"
	
	join bones_read bonearr -- add bones to global bones array
	--WOW2_Read_Bone_Groups ()  -- not needed at the moment
	WOW2_Create_Bones bonearr
		
	return bonearr		
)

fn WOW2_Create_Faces sm view =
(
	step = "Init"
	sm.fs = #() -- faces
	sm.vs = #() -- verts
	sm.ts = #() -- uvs
	sm.bs = #() -- boneindexweight per vertex
	--step = "Create Faces"
	sm.bones = #() -- bones
	--echo ("t ofs "+(sm.oftri as string)+" tot tris: "+(sm.tris as string))
	--if (fseek bstream (view.ofsntris + sm.oftri) #seek_set ) then
	--(
		--local done = 0
		local up = sm.tris as integer -1
		--echo ("/3 check :"+((up+1)/3) as string + " mod "+(mod (up+1) 3)as string)
		
		if ((mod (up+1) 3) != 0.0 ) then
		(
			echo "#ERROR sm.tris not a multiple of 3!"
		)
	--	else
	--		echo "#INFO sm.tris check passed!"
	
		local ofs = sm.oftri as integer
--echo ("oftri = "+ ofs as string)
		for i=1 to (up) by 3 do
		(
		try
		(
		step = "Create Faces" +(i as string)
			local a = view.tri[(ofs+ i)] + 1
			local b = view.tri[(ofs+ i +1)] + 1
			local c = view.tri[(ofs+ i +2)] + 1
		step = "Create Faces erg lookup " + a as string + \
			" " + b as string + " "+ c as string + " " 
--			echo ([a,b,c])
			local erg = ([(view.ind[a] + 1),(view.ind[b] + 1),\
					(view.ind[c] + 1)] as point3 )
--echo "-----------------------------------------------"
			--echo (erg as string)
		step = "Create Faces f append"
			append sm.fs [(i),(i+1),(i+2)] --erg
--			echo ([(i),(i+1),(i+2)] as string)
		step = "Create Faces v append"
			append sm.vs verts_read[erg.x].pos
			append sm.vs verts_read[erg.y].pos
			append sm.vs verts_read[erg.z].pos
		step = "Create Faces t append"
			append sm.ts verts_read[erg.x].uv
			append sm.ts verts_read[erg.y].uv
			append sm.ts verts_read[erg.z].uv 
		step = "Collect bones"
			local belms = #() -- bone id's used by this submesh
			-- @TODO create a copy & paste less solution for boneweights and index reading
			local xw = WOW2_BoneWeightIndex ()
			local yw = WOW2_BoneWeightIndex ()
			local zw = WOW2_BoneWeightIndex ()
			xw.bw = #()
			xw.bi = #()
			xw.cnt = 0
			yw.bw = #()
			yw.bi = #()
			yw.cnt = 0
			zw.bw = #()
			zw.bi = #()
			zw.cnt = 0
			if (verts_read[erg.x].bi1 >= 0 and verts_read[erg.x].bw1 > 0) then
			(	
				-- count from 0 so add 1 for max
				append belms (verts_read[erg.x].bi1 + 1)
				append xw.bi (verts_read[erg.x].bi1 + 1)
				append xw.bw verts_read[erg.x].bw1
				xw.cnt = xw.cnt +1
			)
			if (verts_read[erg.x].bi2 >= 0 and verts_read[erg.x].bw2 > 0) then
			(
				append belms (verts_read[erg.x].bi2 + 1)
				append xw.bi (verts_read[erg.x].bi2 + 1)
				append xw.bw verts_read[erg.x].bw2
				xw.cnt = xw.cnt +1
				
			)
			if (verts_read[erg.x].bi3 >= 0 and verts_read[erg.x].bw3 > 0) then
			(
				append belms (verts_read[erg.x].bi3 + 1)
				append xw.bi (verts_read[erg.x].bi3 + 1)
				append xw.bw verts_read[erg.x].bw3
				xw.cnt = xw.cnt +1
				
			)
			if (verts_read[erg.x].bi4 >= 0 and verts_read[erg.x].bw4 > 0) then
			(
				append belms (verts_read[erg.x].bi4 + 1)
				append xw.bi (verts_read[erg.x].bi4 + 1)
				append xw.bw verts_read[erg.x].bw4
				xw.cnt = xw.cnt +1
				
			)
			append sm.bs xw
			if (verts_read[erg.y].bi1 >= 0 and verts_read[erg.y].bw1 > 0) then
			(
				append belms (verts_read[erg.y].bi1 + 1)
				append yw.bi (verts_read[erg.y].bi1 + 1)
				append yw.bw verts_read[erg.y].bw1
				yw.cnt = yw.cnt +1
				
			)
			if (verts_read[erg.y].bi2 >= 0 and verts_read[erg.y].bw2 > 0) then
			(
				append belms (verts_read[erg.y].bi2 + 1)
				append yw.bi (verts_read[erg.y].bi2 + 1)
				append yw.bw verts_read[erg.y].bw2
				yw.cnt = yw.cnt +1
				
			)
			if (verts_read[erg.y].bi3 >= 0 and verts_read[erg.y].bw3 > 0) then
			(
				append belms (verts_read[erg.y].bi3 + 1)
				append yw.bi (verts_read[erg.y].bi3 + 1)
				append yw.bw verts_read[erg.y].bw3
				yw.cnt = yw.cnt +1
				
			)
			if (verts_read[erg.y].bi4 >= 0 and verts_read[erg.y].bw4 > 0) then
			(
				append belms (verts_read[erg.y].bi4 + 1)
				append yw.bi (verts_read[erg.y].bi4 + 1)
				append yw.bw verts_read[erg.y].bw4
				yw.cnt = yw.cnt +1
				
			)
			append sm.bs yw
			if (verts_read[erg.z].bi1 >= 0 and verts_read[erg.z].bw1 > 0) then
			(
				append belms (verts_read[erg.z].bi1 + 1)
				append zw.bi (verts_read[erg.z].bi1 + 1)
				append zw.bw verts_read[erg.z].bw1
				zw.cnt = zw.cnt +1
				
			)
			if (verts_read[erg.z].bi2 >= 0 and verts_read[erg.z].bw2 > 0) then
			(
				append belms (verts_read[erg.z].bi2 + 1)
				append zw.bi (verts_read[erg.z].bi2 + 1)
				append zw.bw verts_read[erg.z].bw2
				zw.cnt = zw.cnt +1
				
			)
			if (verts_read[erg.z].bi3 >= 0 and verts_read[erg.z].bw3 > 0) then
			(
				append belms (verts_read[erg.z].bi3 + 1)
				append zw.bi (verts_read[erg.z].bi3 + 1)
				append zw.bw verts_read[erg.z].bw3
				zw.cnt = zw.cnt +1
			)
			if (verts_read[erg.z].bi4 >= 0 and verts_read[erg.z].bw4 > 0) then
			(
				append belms (verts_read[erg.z].bi4 + 1)
				append zw.bi (verts_read[erg.z].bi4 + 1)
				append zw.bw verts_read[erg.z].bw4
				zw.cnt = zw.cnt +1
			)
			append sm.bs zw
		step = "unique bones"
--echo ("belms:" + belms.count as string)
			for elm = 1 to belms.count do
			( 
				if ((finditem sm.bones belms[elm]) == 0) then
				(
					append sm.bones belms[elm]
				)
			)
		)
		catch
		(
			echo (getCurrentException())
			throw ()
		)	
						
		)
--		echo "done"
	--)
	-- only sort this one :-)
	sort sm.bones
step = "new"

	-- set sm.bs[i].bi (bone index) to the index of the bone in sm.bones
	-- for all boneindex/weight pairs in this submesh
	for i = 1 to sm.bs.count do
	(	
		-- for all bone indices for this vertex
		for j = 1 to sm.bs[i].bi.count do
		(
			-- assign the index by lookup of the global index 
			-- number in the local bones index array
			sm.bs[i].bi[j] = findItem sm.bones (sm.bs[i].bi[j])
			-- max uses weights from 0.0 to 1.0 WOW uses 0 to 255
			sm.bs[i].bw[j] = (sm.bs[i].bw[j] / 255.0)
		)
	)
	--echo ("MAX WEIGHT IS:"+maxbW as string)
 	--sm.vs = v
	--sm.fs = f
	--sm.ts = t
	--sm.bones = bones
	sm --f
)

fn WOW2_Create_Submeshes view =
(
	step = ("Create Submesh "+ view as string )
	if (fseek bstream views_read[view].ofsnsubmesh #seek_set ) then
	(
		for i=1 to views_read[view].nsubmesh do
		(
			local sm = WOW2_Submesh ()
			sm.id = ReadLong bstream #unsigned
--			echo ("sm.id "+ sm.id as string)
			sm.ofsvert = ReadShort bstream #unsigned
--			echo ("sm.ofsvert "+ sm.ofsvert as string)
			sm.nverts = ReadShort bstream #unsigned
--			echo ("sm.nverts "+ sm.nverts as string)
			sm.oftri = ReadShort bstream #unsigned
--			echo ("sm.oftri "+ sm.oftri as string)
			sm.tris = ReadShort bstream #unsigned
--			echo ("sm.tris "+ sm.tris as string)
			sm.unkown1 = ReadShort bstream #unsigned
			sm.unknown2 = ReadShort bstream #unsigned
			sm.unknown3 = ReadShort bstream #unsigned
			sm.unknown4 = ReadShort bstream #unsigned
			local p = [0.0,0.0,0.0]
			p.x = ReadFloat bstream
			p.y = ReadFloat bstream
			p.z = ReadFloat bstream
			sm.n3floats = p
			
			-- WoW 2.0 change
			SkipBytes bstream 16
			
	--continue	

if true then --sm.id != 0 then
(			
--			local vs = #()
			--vs = WOW2_Create_Verts sm views_read[view]
--			local ts = #()
			--ts = WOW2_Create_Faces sm views_read[view]
			WOW2_Create_Faces sm views_read[view]		

--		local uvs = #()
			--uvs = WOW2_Create_UV sm views_read[view]
--step = "out"
--echo ("bones count on sm#"+ i as string +" is "+ sm.bones.count as string)
--local outstr = "bones: "
--for bcnt = 1 to sm.bones.count do
--	outstr = outstr + (sm.bones[bcnt] as string) + " "

--echo (outstr)
--echo (sm.vs.count)
--echo (sm.bs.count)
--echo (sm.ts.count)
--for i= 1 to uvs.count do
--	echo (uvs[i] as string)
--for i= 1 to ts.count do
--	echo (ts[i] as string)
 step = "mat"
local skinMaterial = standardMaterial name:(name_read +"_"+view as string +"_"+i as string)
step = ("Mesh init ")
local theMesh = undefined
try
(
			theMesh = mesh vertices:(sm.vs) faces:(sm.fs) \
				name:(name_read +"_"+view as string +"_"+i as string) \
				material:skinMaterial tverts:(sm.ts) 
)
catch
(
echo (getCurrentException())
throw ()
)

step = "Build tv's"
try
(
			--Set texcoord faces (so user just needs to load a converted skin PCX file) 
			buildTVFaces theMesh false
			for i = 1 to sm.fs.count do
			(
				--local tcVert = sm.fs[i]
--				echo ((sm.ts[(sm.fs[i].x)]) as string)
--				echo ((sm.ts[(sm.fs[i].y)]) as string)
--				echo ((sm.ts[(sm.fs[i].z)]) as string)
--
--				echo (sm.fs[i] as string)
				setTVFace theMesh i (sm.fs[i])
			)
			update theMesh
)
catch
(
echo (getCurrentException())
throw ()
)

step = "Build Skin"
if (head.nBones > 0 and theMesh != undefined and sm.bones.count > 0) then
(
	try
	( 		step = "Skin prepare"
		max modify mode
		select theMesh
		subObjectLevel = 0
		local newskin = Skin()
		addModifier theMesh newskin
		local mysk = theMesh.modifiers[#Skin]
		subobjectLevel = 1
modPanel.setCurrentObject theMesh.modifiers[#Skin]
subobjectLevel = 1
		step = "Add "+ sm.bones.count as string + " bones to submesh"
		for i = 1 to sm.bones.count do
		(
			step = ("AddBone " + i as string) 
			local bone = (getNodeByName bones_read[sm.bones[i]].max_name exact:true )
			--skinOps.addBone newskin bone 0
			skinOps.addBone theMesh.modifiers[#Skin] bone -1
		)
update theMesh		

		step = ("Set Vertex weights for " + theMesh.name)
		-- NEVER delete this, the lines will update the vertex cache of the skin object it seems
		-- if this lines are omitted the vertex count will be 0 and an exception will be thrown
		-- on adding data to the first vertex 
		throwaway = theMesh.numverts 
		for i = 1 to sm.bs.count do
		(
			if sm.bs[i].cnt > 0 then
			(
				skinOps.setVertexWeights theMesh.modifiers[#Skin] \
					i (sm.bs[i].bi) (sm.bs[i].bw)	
			)
		)
		subObjectLevel = 0
		deselect theMesh
	)
	catch
	(
		echo (getCurrentException())
		throw ()
	)
)
			update theMesh
			--sm = undefined
			gc()
--return 0
)-- if sm.id != 0
		)
	)
)


fn WOW2_Read_Meshes =
(
	step = "Read Meshes"
	for viewcnt = 1 to  1 do --views_read.count do
	(
		--read indices to verts and tris of view
		--view_inds_read
		--view_triss_read
		
		WOW2_Create_Submeshes 1
	)
)

-- read nGlobalSequences into sequences_read
-- @sideeffect global array sequences_read is filled
fn WOW2_Read_GlobalSequences =
(
echo ("Sequences:"+head.nGlobalSequences as string)
	if (fseek bstream head.ofsGlobalSequences #seek_set ) then
	(
		for i = 1 to head.nGlobalSequences do
		(
			append sequences_read (ReadLong bstream )			
		)
	)
)

-- read nAnimations into animations_read
-- @sideeffect global array animations_read is filled
fn WOW2_Read_Animations =
(
	if (fseek bstream head.ofsAnimations #seek_set ) then
	(
		for i = 1 to head.nAnimations do--
		(
			local anim = WOW2_Animations ()
			anim.bbox = #()
			anim.animid = (ReadLong bstream )
			anim.startseq = (ReadLong bstream )
			anim.endseq = (ReadLong bstream )
			anim.speed = (ReadFloat bstream )
			anim.unknown1 = (ReadLong bstream )
			anim.unknown2 = (ReadLong bstream )
			anim.unknown3 = (ReadLong bstream )
			anim.unknown4 = (ReadLong bstream )
			anim.unknown5 = (ReadLong bstream )
			for j = 1 to 6 do
			(
				append anim.bbox  (ReadFloat bstream )
			)
			anim.radius = (ReadFloat bstream )
			anim.unknown6 = (ReadShort bstream )
			anim.unknown7 = (ReadShort bstream )
			
			append animations_read anim
		)
	)
)

-----------------------------------------------------------------
-- MAIN
-----------------------------------------------------------------
fn MainFunc =
(
ECHO("START")
InitLoader()
ECHO("INITIALIZATION COMPLETE")
try
(
if objects.count > 0 then
(
echo "--Cleaning Scene--"
select objects
delete $
)
--if false then
--(
echo ("--Start--")
	WOW2_Open(filename)
	WOW2_Read_Header()
echo ("Trying read Name")
	WOW2_Read_Name()

	if( LoadBones ) then 
	(
		echo ("Trying read bones")
		WOW2_Read_Bones()
	)

	echo ("Trying read verts"	)
	WOW2_Read_Verts()
	
	echo ("Trying read views")
	WOW2_Read_Views()

	echo ("Trying read Meshes")
	WOW2_Read_Meshes()
--echo "--Sequences--"
--	WOW2_Read_GlobalSequences()
--echo "--Animations--"

	if( LoadAnimation ) then 
	(
		WOW2_Read_Animations()
		WOW2_Create_BoneAnimations bones_read
	)
	WOW2_Close()
echo "--Model loaded--"
--deselect *
--)
	gc()
	true
)
catch
(
	format "-- Failed in \n" 
	echo(  step )
-- to:listener
	if bstream != undefined then WOW2_Close()
	gc()
	false
)

)

macroScript OpenWow2ImportFloater category:"WOW"
(

	rollout WoW2_Import_Roll "WoW2_Import" width:250 
	(
		button theButton "Open file dialog..." width:240
		Label lbl "...or enter name manualy:"
		edittext  edrit  "" labelOnTop:true fieldwidth:240 height:36
		label tl1 "\n"
		checkbox cbLoadBones "Load bones" checked:false triState: 0 enabled:false
		checkbox cbLoadAnimation "Load animation" checked:false triState: 0
		label tl21 "\n"

		fn CheckCB ch = 
		(
			if( LoadBones ==  false ) then
			(
				LoadAnimation =false
				cbLoadAnimation.checked=false
				cbLoadAnimation.enabled=false
			)
			else
			(
				cbLoadAnimation.enabled=true
			)
		)
		
		on cbLoadBones changed ch do
		(
			LoadBones = ch
			CheckCB ch
		)
		
		on cbLoadAnimation changed ch do
		(
			LoadAnimation = ch
		)
		
		label tl2 ""
		button gogo "Import it NOW"
		
		fn CheckFile fname = 
		(
			local fs = getFileSize fname
		
			if( fs > 0 ) then
			(
				gogo.enabled = true
				tl2.text = ""
			)
			else
			(
				tl2.text = "File name is Incorrect"
				gogo.enabled = false
			)
		)
		
		on WoW2_Import_Roll open do
		(
			edrit.text = (filename  as string)			
			CheckFile filename

			cbLoadAnimation.checked = LoadAnimation 
			cbLoadBones.checked = LoadBones 
			CheckCB ch
		) 
		
		on theButton pressed do 
		(
			local f = getOpenFileName types:"Wow model(*.m2)|*.m2|All|*.*|" caption:"Open A Test File:" filename:edrit.text
			
			try
			(
				filename = f + ""			
				edrit.text = filename
				CheckFile filename
			)
			catch()			
		)
		
		
		on edrit changed newtext do
		(
			CheckFile newtext
			if( gogo.enabled ) then ( filename = newtext )
		)
		
		on gogo pressed do 
		(
			CheckFile filename
			if( gogo.enabled ) then 
			(
				if( MainFunc ()	== false ) then
				(
					messagebox "IMPORT FILED. Look MAXScript listener for details"
				)
			)
		)
 	)
	
	on isChecked do WoW2_Import_Roll.open --return true if rollout is open
	

	on execute do createDialog WoW2_Import_Roll

	
	on closeDialogs do destroyDialog WoW2_Import_Roll

)
	


 /*
WowMenu = mainMenuBar.findMenu "WOW"
if( WoWMenu != undefined) then
(
	menuMan.unRegisterMenu WowMenu
	WowMenu = undefined
)

*/

if menuMan.registerMenuContext 0x1aa76d7d then
(
	-- Get the main menu bar
	local mainMenuBar = menuMan.getMainMenuBar()
	-- Create a new menu
	local subMenu = menuMan.createMenu "WOW"
	
	-- create a menu item that calls the sample macroScript
	local testItem = menuMan.createActionItem "OpenWow2ImportFloater" "WOW" 
	
	testitem.setUseCustomTitle true
	
	testitem.setTitle  "Open importer"
	
	-- Add the item to the menu
	subMenu.addItem testItem -1
	
	-- Create a new menu item with the menu as it's sub-menu
	local subMenuItem = menuMan.createSubMenuItem "WOW" subMenu
	
	-- compute the index of the next-to-last menu item in the main menu bar
	local subMenuIndex = mainMenuBar.numItems() - 1
	
	-- Add the sub-menu just at the second to last slot
	mainMenuBar.addItem subMenuItem subMenuIndex
	
	-- redraw the menu bar with the new item
	menuMan.updateMenuBar()
)
