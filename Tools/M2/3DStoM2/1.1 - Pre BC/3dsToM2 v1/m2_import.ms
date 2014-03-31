--
-- m2 -> 3ds max importer
-- Original script found at 3d-inferno.com
-- http://3d-inferno.com/Diverse/index.php?v0=Diverse&v1=Wow%20Importer&v2=0
-- updated with textures, bones and animation by ufoz
--
-- requires python 2.4 and world of warcraft to extract textures
--

--
-- Script to import m2 files as included in the mpq files of world of warcraft into GMax
--

--
-- GLOBALS AND STRUCTURES
--

-- this is where this script, and the extract directory should be located  (overwritten by UI)
global basePath = "c:\\3dsmax6\\scripts"

-- model filename (overwritten by UI)
global filename = "C:\\Documents and Settings\\Chris\\Desktop\\XXX\\Creature\\raptor\\Raptor.m2"
--global filename = "World\\Generic\\NightElf\\Passive doodads\\Statues\\StatueNEPriestess.m2"
--global filename = "Creature\\Cat\\Cat.m2"
--global filename = "Character\\Human\\Female\\HumanFemale.m2"
--global filename = "World\\Generic\\Human\\Passive Doodads\\LogMachines\\LogMachine02.m2"
--global filename = "World\\KhazModan\\Ironforge\\PassiveDoodads\\LavaSteam\\IronforgeWheel.m2"
--global filename = "World\\KhazModan\\Ironforge\\PassiveDoodads\\LavaSteam\\IronforgeBellow.m2"
--global filename = "World\\KhazModan\\Ironforge\\PassiveDoodads\\Statues\\IronforgeStatue_01.m2"

-- some options (overwritten by UI)
global doCreateMesh = true		-- create model geometry
global doCreateBones = true		-- create bones
global doTransformBones = true	-- convert animations (requires doCreateBones)
global doCreateSkin = true		-- apply bone weights to vertices (requires doCreateMesh and doCreateBones)
global doCreateTextures = true	-- extract textures from wow
global useFPS = 25		-- fps to rescale animations to
global viewNum = 1    -- 1 to 4
global setMatNum = 1   -- material slot number in the material editor
global flipuv_y = true  -- change this if textures appear upside down

-- globals
global head
global bstream
global step
global verts_read = #()
global views_read = #()
global view_inds_read = #()
global view_tris_read = #()
global name_read
global bones_read = #()
global cbones = #()
global cmesh
global cmat
global sm_read = #()
global tex_read = #()
global tex_lut = #()
global flags_read = #()

global replace_tex = #("","","","","","","","","","","","","","","","")

-- code
fn WOW2_Convert_Time t =
(
	t / (1000 / useFPS)
)

struct WOW2_Header
(
	magic,version,namelen,ofsname,
	un1,
	nGlobalSequences,ofsGlobalSequences,
	nAnimations,ofsAnimations,
	nC,ofsC,nD,ofsD,
	nBones,ofsBones,
	nF,ofsF,
	nVertices,ofsVertices,
	nViews,ofsViews,
	nColors,ofsColors,
	nTextures,ofsTextures,
	nTransparency,ofsTransparency,
	nI,ofsI,
	nTexAnims,ofsTexAnims,
	nK,ofsK,
	nRenderFlags,ofsRenderFlags,
	nY,ofsY,
	nTexLookup,ofsTexLookup,
	nTexUnits,ofsTexUnits,
	nTransLookup,ofsTransLookup,
	nTexAnimLookup,ofsTexAnimLookup,
	n14floats,
	nBoundingTriangles,ofsBoundingTriangles,
	nBoundingVertices,ofsBoundingVertices,
	nBoundingNormals,ofsBoundingNormals,
	nAttachments,ofsAttachments,
	nAttLookup,ofsAttLookup,
	nQ,ofsQ,
	nLights,ofsLights,
	nCameras,ofsCameras,
	nCameraLookup,ofsCameraLookup,
	nRibbonEmitters,ofsRibbonEmitters,
	nParticleEmitters,ofsParticleEmitters
)

struct WOW2_Vertex
(
	pos,bw1,bw2,bw3,bw4,bi1,bi2,bi3,bi4,normal,uv,n2floats
)

struct WOW2_Animation
(
	aid,start,end
)

struct WOW2_AnimBlock
(
	type,seq,nRanges,ofsRanges,nTimes,ofsTimes,nKeys,ofsKeys,
	ranges, times, keys
)

struct WOW2_Bone
(
	findex, flags, par, unk, btrans, brot, bscale, pos
)

struct WOW2_View
(
	nindex,ofsnindex, ntris,ofsntris,nverts,ofsnverts,
	nsubmesh,ofsnsubmesh,ntextures,ofsntextures,lodlevel,
	ind,tri, -- loaded vert and tri index
	texunits -- loaded tex units
)

struct WOW2_Submesh
(
	id,ofsvert,nverts,oftri,tris,unkown1,unknown2,
	unknown3,unknown4,n3floats,
	vs,fs,ts
)

struct WOW2_Textureunit
(
	flags,order,submesh1,submesh2,col,rflags,texunit,unknown1,
	texlookupindex,texunit2,trans,texanim
)

struct WOW2_Texture
(
	-- type:
	--0  Texture given in filename
	--1  Body + clothes
	--2  Cape
	--6  Hair, beard
	--8  Tauren fur
	--11  Skin for creatures
	--12  Skin for creatures #2
	--13  Skin for creatures #3

	type,flags,namelen,ofsname,
	filename, convfilename
)

struct WOW2_RenderFlags
(
	blending, flags
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

--
-- HELPERS

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

--
-- WOW M2 FUNCTIONS

fn WOW2_Open fname =
(
	-- check to see if this is a real path
	ss = substring fname 2 2
	if (ss != ":\\") then (
		local dest = (basePath + "\\data\\" + fname)
		ff = fopen dest "rb"
		if ff==undefined then
		(
			echo ("Extracting " + fname)
			echo ("To " + dest)
			DOSCommand (basePath+"\\extract\\extract.py \"" + fname + "\"")
		) else (
			echo ("Found " + dest)
		)
		fname = dest
	)

	bstream = fopen fname "rb"
	if bstream==undefined then (
		echo "File not found!"
		throw "File not found"
	)
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
	echo ("Name len:"+ (head.namelen as string))
	head.ofsname = ReadLong bstream #unsigned
	echo ("Name offs:"+ head.ofsname as string)
	head.un1 = ReadLong bstream #unsigned
	head.nGlobalSequences = ReadLong bstream #unsigned
	head.ofsGlobalSequences = ReadLong bstream #unsigned
	head.nAnimations = ReadLong bstream #unsigned
	head.ofsAnimations = ReadLong bstream #unsigned
	head.nC = ReadLong bstream #unsigned
	head.ofsC = ReadLong bstream #unsigned
	head.nD = ReadLong bstream #unsigned
	head.ofsD = ReadLong bstream #unsigned
	head.nBones = ReadLong bstream #unsigned
	head.ofsBones = ReadLong bstream #unsigned
	head.nF = ReadLong bstream #unsigned
	head.ofsF = ReadLong bstream #unsigned
	head.nVertices = ReadLong bstream #unsigned
	echo ("Vertices:"+head.nVertices as string)
	head.ofsVertices = ReadLong bstream #unsigned
	echo ("Vertices of:"+head.ofsVertices as string)
	head.nViews = ReadLong bstream #unsigned
	echo ("Views:"+head.nViews as string)
	head.ofsViews = ReadLong bstream #unsigned
	head.nColors = ReadLong bstream #unsigned
	head.ofsColors = ReadLong bstream #unsigned
	head.nTextures = ReadLong bstream #unsigned
	--echo ("Textures:"+head.nTextures as string)
	head.ofsTextures = ReadLong bstream #unsigned
	head.nTransparency = ReadLong bstream #unsigned
	head.ofsTransparency = ReadLong bstream #unsigned
	head.nI = ReadLong bstream #unsigned
	head.ofsI = ReadLong bstream #unsigned
	head.nTexAnims = ReadLong bstream #unsigned
	head.ofsTexAnims = ReadLong bstream #unsigned
	head.nK = ReadLong bstream #unsigned
	head.ofsK = ReadLong bstream #unsigned
	head.nRenderFlags = ReadLong bstream #unsigned
	head.ofsRenderFlags = ReadLong bstream #unsigned
	head.nY = ReadLong bstream #unsigned
	head.ofsY = ReadLong bstream #unsigned
	head.nTexLookup = ReadLong bstream #unsigned
	--echo ("TexLookup:"+head.nTexLookup as string)
	head.ofsTexLookup = ReadLong bstream #unsigned
	head.nTexUnits = ReadLong bstream #unsigned
	head.ofsTexUnits = ReadLong bstream #unsigned
	head.nTransLookup = ReadLong bstream #unsigned
	head.ofsTransLookup = ReadLong bstream #unsigned
	head.nTexAnimLookup = ReadLong bstream #unsigned
	head.ofsTexAnimLookup = ReadLong bstream #unsigned
	head.n14floats = undefined
	for i = 1 to 14 do
	(
		undef = ReadFloat bstream
	)
	head.nBoundingTriangles = ReadLong bstream #unsigned
	head.ofsBoundingTriangles = ReadLong bstream #unsigned
	head.nBoundingVertices = ReadLong bstream #unsigned
	head.ofsBoundingVertices = ReadLong bstream #unsigned
	head.nBoundingNormals = ReadLong bstream #unsigned
	head.ofsBoundingNormals = ReadLong bstream #unsigned
	head.nAttachments = ReadLong bstream #unsigned
	head.ofsAttachments = ReadLong bstream #unsigned
	head.nAttLookup = ReadLong bstream #unsigned
	head.ofsAttLookup = ReadLong bstream #unsigned
	head.nQ = ReadLong bstream #unsigned
	head.ofsQ = ReadLong bstream #unsigned
	head.nLights = ReadLong bstream #unsigned
	head.ofsLights = ReadLong bstream #unsigned
	head.nCameras = ReadLong bstream #unsigned
	head.ofsCameras = ReadLong bstream #unsigned
	head.nCameraLookup = ReadLong bstream #unsigned
	head.ofsCameraLookup = ReadLong bstream #unsigned
	head.nRibbonEmitters = ReadLong bstream #unsigned
	head.ofsRibbonEmitters = ReadLong bstream #unsigned
	head.nParticleEmitters = ReadLong bstream #unsigned
	head.ofsParticleEmitters   = ReadLong bstream #unsigned
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

fn WOW2_Read_Anims =
(
	step = "Read Anims"
	if (fseek bstream head.ofsAnimations #seek_set ) then
	(	
		maxend = 0
		for i=1 to head.nAnimations  do
		(
			local a = WOW2_Animation()
			a.aid = ReadLong bstream
			a.start = ReadLong bstream
			a.end = ReadLong bstream
			myend = WOW2_Convert_Time a.end
			if myend > maxend then maxend = myend
			SkipBytes bstream (68-12)
		)
		animationRange = interval 0 maxend
		frameRate = useFPS
	)
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
			if flipuv_y then vert.uv = [v2.x,1.0-v2.y,0.0]
			else vert.uv = v2
			v1.x = ReadFloat bstream
			v1.y = ReadFloat bstream
			vert.n2floats = v1

			append verts_read vert
		)	
		
		echo ("Verts read: " + verts_read.count as string )
	)
	else
		echo "unable to read vertices"
)

fn WOW2_Read_AnimBlock bl =
(
	bl.type = ReadShort bstream
	bl.seq = ReadShort bstream
	bl.nRanges = ReadLong bstream #unsigned
	bl.ofsRanges = ReadLong bstream #unsigned
	bl.nTimes = ReadLong bstream #unsigned
	bl.ofsTimes = ReadLong bstream #unsigned
	bl.nKeys = ReadLong bstream #unsigned
	bl.ofsKeys = ReadLong bstream #unsigned
)

fn WOW2_Fill_AnimBlock bl dim =
(
	bl.ranges = #()
	if bl.nRanges>0 then
	(
		fseek bstream bl.ofsRanges #seek_set
		for i=1 to bl.nRanges do
		(
			a = ReadLong bstream
			b = ReadLong bstream
			r = [a,b]
			append bl.ranges r
		)
	)
	bl.times = #()
	if bl.nTimes>0 then
	(
		fseek bstream bl.ofsTimes #seek_set
		for i=1 to bl.nTimes do
		(
			t = ReadLong bstream
			append bl.times t
		)
	)
	bl.keys = #()
	if bl.nKeys>0 then
	(
		fseek bstream bl.ofsKeys #seek_set
		for i=1 to bl.nKeys do
		(
			if dim==1 then
			(
				local kv = ReadFloat bstream
				append bl.keys kv
			)
			else if dim==3 then
			(
				local kv = [0.0,0.0,0.0]
				kv.x = ReadFloat bstream
				kv.y = ReadFloat bstream
				kv.z = ReadFloat bstream
				append bl.keys kv
			)
			else if dim==4 then
			(
				local kv = [0.0,0.0,0.0,0.0]
				kv.x = ReadFloat bstream
				kv.y = ReadFloat bstream
				kv.z = ReadFloat bstream
				kv.w = ReadFloat bstream
				append bl.keys kv
			)
		)
	)
)

fn WOW2_Read_Views =
(
	step = "Read Views"
	if (fseek bstream head.ofsViews #seek_set ) then
	(
		for i=1 to head.nViews  do
		(
			local view = WOW2_View ()
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
			view.lodlevel = ReadLong bstream #unsigned
			
			append views_read view
			step = "Read Views Mod 3"
			if((mod (view.ntris) 3) != 0.0 )then
				echo ("View["+i as string +"].ntris is not a multiple of 3!\n")
			
			if i == 1 then
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

	step = "Read View tex units"
	for i=1 to views_read.count do
	(
		views_read[i].texunits = #()
		
		if (fseek bstream views_read[i].ofsntextures #seek_set ) then
		(
			for j=1 to views_read[i].ntextures do
			(
				tu = WOW2_Textureunit()
				tu.flags = ReadShort bstream #unsigned
				tu.order = ReadShort bstream
				tu.submesh1 = ReadShort bstream #unsigned
				tu.submesh2 = ReadShort bstream #unsigned
				tu.col = ReadShort bstream
				tu.rflags = ReadShort bstream #unsigned
				tu.texunit = ReadShort bstream #unsigned
				tu.unknown1 = ReadShort bstream #unsigned
				tu.texlookupindex = ReadShort bstream #unsigned
				tu.texunit2 = ReadShort bstream #unsigned
				tu.trans = ReadShort bstream #unsigned
				tu.texanim = ReadShort bstream

				append views_read[i].texunits tu
			)
		)
	)
)

fn WOW2_Read_Bones =
(
	step = "Read Bones"
	if (fseek bstream head.ofsBones #seek_set ) then
	(
		--echo ("nbones " + head.nBones as string)
		for i=1 to head.nBones do
		(
			--echo ("reading bone " + i as string)
			local b = WOW2_Bone()
			b.findex = ReadLong bstream
			b.flags = ReadLong bstream
			b.par = ReadShort bstream
			b.unk = ReadShort bstream
			b.btrans = WOW2_AnimBlock(); WOW2_Read_AnimBlock(b.btrans)
			b.brot = WOW2_AnimBlock(); WOW2_Read_AnimBlock(b.brot)
			b.bscale = WOW2_AnimBlock(); WOW2_Read_AnimBlock(b.bscale)
			local vp = [0.0,0.0,0.0]
			vp.x = ReadFloat bstream; vp.y = ReadFloat bstream; vp.z = ReadFloat bstream
			b.pos = vp
			append bones_read b
		)
	)
)

-- from the maxscript reference
fn setNodeWorldRotation theNode theRot = 
( 
  in coordsys (transmatrix theNode.transform.pos) 
    theNode.rotation = theRot 
) 

fn WOW2_Bone_Depth b =
(
	if b.par == -1 then return 0
	else return ( 1 + WOW2_Bone_Depth bones_read[b.par + 1] )
)

fn WOW2_Create_Bones =
(
	---------------------- bones
	step = "Create Bones"
	echo "Creating bones..."
	for i=1 to head.nBones do
	(
		local startpos = bones_read[i].pos
		--local endpos = bones_read[i].pos -- worked
		local endpos = bones_read[i].pos + [10,0,0]
		local bname = ("Bone_" + name_read + "_" + (i as string))
		echo ("Creating bone " + i as string)

		--cb = BoneSys.createBone startpos endpos [0,0,1] -- worked
		cb = BoneSys.createBone startpos endpos [0,0,0.1]
		cb.name = bname
		cb.pivot = bones_read[i].pos

		--cb.boneScaleType = #scale
		cb.boneScaleType = #none

		-- for some reason the bones come out as big and ugly
		hide cb
	
		append cbones cb
	)
	max views redraw
	echo "Setting up bone hierarchy"
	for i=1 to head.nBones do
	(
		if bones_read[i].par!=-1 then
		(
			--echo ("Setting parent of " + i as string + " to " + (bones_read[i].par+1) as string)
			cbones[i].parent = cbones[bones_read[i].par+1]
		)
	)
	max views redraw
	---------------------- animation
	if doTransformBones then (
		-- sort bones by depth
		local bd = #()
		for i=1 to head.nBones do
		(
			bonerec = [(WOW2_Bone_Depth bones_read[i]), i]
			append bd bonerec
		)
		fn compfn a b = ( if a.x<b.x then return 1; else if a.x>b.x then return -1; else return 0; )
		qsort bd compfn

		echo "Transforming bones..."

		with animate on (
			step = "Bone transforms"
			for k=1 to head.nBones do
			(
				local i = bd[k].y
				--echo ("Doing bone " + i as string + " (depth: " + bd[k].x as string + ")" )
				--local i = k

				echo ("Transforming bone " + k as string)
				b = bones_read[i]
				cb = cbones[i]
				WOW2_Fill_AnimBlock b.btrans 3
				WOW2_Fill_AnimBlock b.brot 4
				WOW2_Fill_AnimBlock b.bscale 3
				-- translation
				if b.btrans.type==1	then
				(
					in coordsys parent op = copy cb.pos

					for j=1 to b.btrans.nKeys do
					(
						t = WOW2_Convert_Time b.btrans.times[j]
						at time t (
							in coordsys parent cb.pos = op + b.btrans.keys[j]
						)
					)
				)
				-- rotation
				if b.brot.type==1 then
				(
					for j=1 to b.brot.nKeys do
					(
						t = WOW2_Convert_Time b.brot.times[j]
						local kv = b.brot.keys[j]
						q = quat kv.x kv.y kv.z kv.w
						at time t (
							setNodeWorldRotation cb q
						)
					)
				)
				-- scaling
				if b.bscale.type==1 then
				(
					for j=1 to b.bscale.nKeys do
					(
						t = WOW2_Convert_Time b.bscale.times[j]
						at time t cb.scale = b.bscale.keys[j]
					)
				)
				-- cleanup
				b.btrans = undefined
				b.brot = undefined
				b.bscale = undefined
				gc()
			)
		)
	)
	---------------------- skinning
	if doCreateMesh and doCreateSkin then (
		step = "Create Skin"
		echo "Applying skin to mesh..."
		max modify mode

		m = cmesh
		select m

		addModifier m (Skin name:"Skin")
		sk = m.modifiers[#Skin]

		modPanel.setCurrentObject sk

		-- 1. add all bones to the skin
		for j=1 to head.nBones do
		(
			skinOps.addBone sk cbones[j] -1
		)
		update m
		max views redraw

		-- 2. for each vertex, setup the bone weights
		for j=1 to head.nVertices do
		(
			v = verts_read[j]
			--echo (j as string + ": " + v.bi1 as string + ", " + v.bi2 as string + ", " + v.bi3 as string + ", " + v.bi4 as string \
			--	+ " --- (" + v.bw1 as string + "," + v.bw2 as string + "," + v.bw3 as string + "," + v.bw4 as string + ")")
			local lbi = #()
			local lbw = #()
			if v.bw1>0 then ( append lbi (v.bi1+1); append lbw (v.bw1/255.0); )
			if v.bw2>0 then ( append lbi (v.bi2+1); append lbw (v.bw2/255.0); )
			if v.bw3>0 then ( append lbi (v.bi3+1); append lbw (v.bw3/255.0); )
			if v.bw4>0 then ( append lbi (v.bi4+1); append lbw (v.bw4/255.0); )
			--echo (" -----> " + lbi as string + " / " + lbw as string)
			skinOps.ReplaceVertexWeights sk j lbi lbw
		)
		update m
		step = "Skinning complete"
	)
)

fn WOW2_Create_Mesh =
(
	step = "Create Mesh"

	echo "Creating mesh..."

	v = #()
	f = #()
	t = #()
	mtl = #()

	-- using view:
	local view = views_read[viewNum]
	fseek bstream view.ofsnsubmesh #seek_set

	echo "Creating vertices..."

	-- build vertices and texcoords
	for i = 1 to head.nVertices do
	(
		append v verts_read[i].pos
		append t verts_read[i].uv
	)

	cmat = multimaterial numsubs:view.nsubmesh

	-- build faces by submesh
	-- (each submesh will be a material id)
	for s = 1 to view.nsubmesh do
	(
		echo ("Creating Submesh " + s as string)

		sm = WOW2_SubMesh()
		sm.id = ReadLong bstream #unsigned
		sm.ofsvert = ReadShort bstream #unsigned
		sm.nverts = ReadShort bstream #unsigned
		sm.oftri = ReadShort bstream #unsigned
		sm.tris = ReadShort bstream #unsigned
		sm.unkown1 = ReadShort bstream #unsigned
		sm.unknown2 = ReadShort bstream #unsigned
		sm.unknown3 = ReadShort bstream #unsigned
		sm.unknown4 = ReadShort bstream #unsigned
		local p = [0.0,0.0,0.0]
		p.x = ReadFloat bstream
		p.y = ReadFloat bstream
		p.z = ReadFloat bstream
		sm.n3floats = p

		append sm_read sm

		local up = sm.tris as integer -1

		if ((mod (up+1) 3) != 0.0 ) then echo "#ERROR sm.tris not a multiple of 3!"
		--else echo "#INFO sm.tris check passed!"
		
		local ofs = sm.oftri as integer

		for i = 1 to (up) by 3 do
		(
			local a = view.tri[ofs + i] + 1
			local b = view.tri[ofs + i + 1] + 1
			local c = view.tri[ofs + i + 2] + 1
			
			local face = ( [(view.ind[a] + 1), (view.ind[b] + 1), (view.ind[c] + 1)] as point3 )

			append f face
			append mtl s
		)

		local skinMaterial = standardMaterial name:("Mat_" + name_read + "_" + s as string)
		cmat[s] = skinMaterial
	)

	echo "Creating mesh object"

	cmesh = mesh vertices:v faces:f materialIDs:mtl tverts:t name:("Mesh_"+name_read)
	cmesh.material = cmat
	update cmesh

	-- set normals
	for i = 1 to head.nVertices do
	(
		setNormal cmesh i verts_read[i].normal
	)

	-- tvert faces
	step = "Build tv's"
	try
	(
		--Set texcoord faces 
		buildTVFaces cmesh false
		for i = 1 to f.count do
		(
			setTVFace cmesh i f[i]
		)
	)
	catch
	(
		echo (getCurrentException())
		throw ()
	)		
	update cmesh

	step = "Create Mesh Done"
)

fn WOW2_Read_Textures =
(
	step = "Read textures"
	if (fseek bstream head.ofsTextures #seek_set ) then
	(
		for i=1 to head.nTextures do
		(
			tex = WOW2_Texture()
			tex.type = ReadLong bstream
			tex.flags = ReadLong bstream #unsigned
			tex.namelen = ReadLong bstream #unsigned
			tex.ofsname = ReadLong bstream #unsigned
			append tex_read tex
		)

		for i=1 to head.nTextures do
		(
			if tex_read[i].type == 0 then
			(
				fseek bstream tex_read[i].ofsname #seek_set
				tex_read[i].filename = ReadFixedString bstream tex_read[i].namelen
			)
		)
	)
	step = "Read texture lookup table"
	if (fseek bstream head.ofsTexLookup #seek_set ) then
	(
		for i=1 to head.nTexLookup do
		(
			lv = ReadShort bstream
			append tex_lut lv
		)
	)
	if (fseek bstream head.ofsRenderFlags #seek_set ) then
	(
		for i=1 to head.nRenderFlags do
		(
			rf = WOW2_RenderFlags()
			rf.flags = ReadShort bstream
			rf.blending = ReadShort bstream
			append flags_read rf
		)
	)
)

fn WOW2_Convert_Texture filename =
(
	local destfile = (basePath + "\\data\\" + (replace filename (filename.count-2) 3 "tga"))
	ff = fopen destfile "rb"
	if ff==undefined then
	(
		echo ("Extracting " + filename)
		-- step 1 extract from mpq
		-- needs wow and python 2.4 installed
		DOSCommand (basePath+"\\extract\\extract.py \"" + filename + "\"")
		-- step 2 convert blp
		cmdline = (basePath+"\\extract\\blpconv.exe \"" + basePath + "\\data\\" + filename + "\"")
		--echo cmdline
		DOSCommand cmdline
	) else (
		echo ("Found " + filename)
		fclose ff
	)
	return destfile
)

fn WOW2_Convert_Textures =
(
	for i=1 to tex_read.count do
	(
		if tex_read[i].type != 0 and doCreateTextures and replace_tex[tex_read[i].type] != "" then
		(
			tex_read[i].filename = replace_tex[tex_read[i].type]
			tex_read[i].type = 0
		)
		if tex_read[i].type == 0 and doCreateTextures then
		(
			try (
				tex_read[i].convfilename = WOW2_Convert_Texture(tex_read[i].filename)
			) catch (
				echo ("Error converting " + tex_read[i].filename)
				tex_tead[i].type = -1
			)
		)
	)
)

fn WOW2_Create_Materials =
(
	local view = views_read[viewNum]

	step = "Convert Textures"
	echo "Converting textures..."
	WOW2_Convert_Textures()

	echo "-----"

	-- each submesh
	for i=1 to sm_read.count do
	(
		local tu = undefined

		-- find a texture for this submesh
		for j=1 to view.ntextures do
		(
			tu = view.texunits[j]
			if ((tu.submesh1+1) == i) and (tu.texunit==0) then exit 
		)

		if tu != undefined then
		(
			tex = tex_read[tex_lut[tu.texlookupindex + 1] + 1]
			if tex.type == 0 and doCreateTextures then
			(
				local bt = BitmapTexture()
				bt.filename = tex.convfilename
				echo ("Setting submaterial " + i as string + " to " + tex.filename)
				cmat[i].diffuseMap = bt
				showTextureMap cmat[i] bt on 
			)
			else
			(
				-- random color
				if doCreateTextures and tex.type >= 0 then echo ("Submesh " + i as string + " needs a replacable texture of type " + tex.type as string)
				cmat[i].diffuse = random(color 0 0 0) (color 255 255 255)
			)

			-- 2 sided
			local rf = flags_read[tu.rflags+1]
			if (bit.and rf.flags 4) != 0 then (
				--echo ("Setting two-sided for submaterial " + i as string)
				cmat[i].twoSided = true
			)
		)
	)
	setMeditMaterial setMatNum cmat
)

---
--- MAIN
fn WOW2_Main =
(
	WOW2_Open(filename)
	WOW2_Read_Header()
	WOW2_Read_Name()
	WOW2_Read_Anims()
	WOW2_Read_Verts()
	WOW2_Read_Textures()
	WOW2_Read_Views()
	if doCreateMesh then WOW2_Create_Mesh()
	WOW2_Read_Bones()
	if doCreateBones then WOW2_Create_Bones()
	WOW2_Create_Materials()
	WOW2_Close()
	format "======== Done ========\n" to:listener

	gc()

	max views redraw
)

-- UI
rollout WOW2_Dialog "M2 Model Import" width:400 height:400
(
	-- file selection
	label lFileName "Model filename or path in MPQ:" align:#left
	button bOpenFile "Open..." pos: [180,5]
	edittext tFileName

	label lSpacer1 "" height:5

	-- import options
	checkbox cImportMesh "Import mesh" checked:true across:2
	spinner nFps "FPS" range:[1,100,25] type:#integer fieldwidth:50
	checkbox cImportBones "Import bones" checked:true across:2
	spinner nLod "LOD" range:[1,4,1] type:#integer fieldwidth:50
	checkbox cTransformBones "Transform bones" checked:true across:2
	spinner nMat "Material slot" range:[1,24,1] type:#integer fieldwidth:50
	checkbox cImportSkin "Apply skin to mesh" checked:true across:2
	checkbox cFlipUV "Fix Y texcoords" checked:true
	checkbox cImportTextures "Import textures" checked:true across:2
	edittext tPath "Script path" text:(GetDir #scripts)

	label lSpacer2 "" height:5

	-- replacable textures
	label lTextures "Replacable textures (path in MPQ or blank)" align:#left
	label lSpacer3 "" height:5
	edittext tTex1 "Character body"
	edittext tTex2 "Cape               "
	edittext tTex6 "Hair/beard      "
	edittext tTex8 "Tauren fur       "
	edittext tTex11 "Creature skin 1"
	edittext tTex12 "Creature skin 2"
	edittext tTex13 "Creature skin 3"

	label lSpacer4 "" height:5

	button bOK "Import" across:2
	button bCancel "Cancel"

	on bOpenFile pressed do
	(
		local ret = getOpenFileName types:"M2 model (*.m2)|*.m2|All|*.*|"
		if ret != undefined then tFileName.text = ret
	)
	on bOK pressed do
	(
		-- setup options and start import
		filename = tFileName.text
		basePath = tPath.text
		doCreateMesh = cImportMesh.checked
		doCreateBones = cImportBones.checked
		doTransformBones = cTransformBones.checked
		doCreateSkin = cImportSkin.checked
		doCreateTextures = cImportTextures.checked
		flipuv_y = cFlipUV.checked
		viewNum = nLod.value as integer
		useFPS = nFps.value as integer
		setMatNum = nMat.value as integer
		replace_tex[1] = tTex1.text
		replace_tex[2] = tTex2.text
		replace_tex[6] = tTex6.text
		replace_tex[8] = tTex8.text
		replace_tex[11] = tTex11.text
		replace_tex[12] = tTex12.text
		replace_tex[13] = tTex13.text

		DestroyDialog WOW2_Dialog
		-- toggle comment for exception catching
		--/*
		try
		(
			WOW2_Main()
		)
		catch
		(
			format "-- Failed in <%>\n" step to:listener
			if bstream != undefined then WOW2_Close()
			gc()
		)
		/*/
		WOW2_Main()
		-- */
	)
	on bCancel pressed do
	(
		DestroyDialog WOW2_Dialog
	)
)

fn WOW2_UI_Main =
(
	if WOW2_DialogFloater != undefined do
	(
		closerolloutfloater WOW2_DialogFloater
	)		

	CreateDialog WOW2_Dialog width:400 height:400 style:#(#style_titlebar, #style_border, #style_sysmenu, #style_resizing)
)

WOW2_UI_Main()

