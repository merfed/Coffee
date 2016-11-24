import struct

EEndianness_Native = "@"
EEndianness_Little = "<"
EEndianness_Big = ">"

def ReadUInt8( File, Endianness ):
	return struct.unpack( Endianness + "B", File.read( 1 ) )[0]

def ReadSInt8( File, Endianness ):
	return struct.unpack( Endianness + "b", File.read( 1 ) )[0]

def ReadUInt16( File, Endianness ):
	return struct.unpack( Endianness + "H", File.read( 2 ) )[0]

def ReadSInt16( File, Endianness ):
	return struct.unpack( Endianness + "h", File.read( 2 ) )[0]

def ReadUInt32( File, Endianness ):
	return struct.unpack( Endianness + "I", File.read( 4 ) )[0]

def ReadSInt32( File, Endianness ):
	return struct.unpack( Endianness + "i", File.read( 4 ) )[0]

def ReadFloat32( File, Endianness ):
	return struct.unpack( Endianness + "f", File.read( 4 ) )[0]

def WriteUInt8( File, Endianness, Value ):
	File.write( struct.pack( Endianness + "B", Value ) )

def WriteSInt8( File, Endianness, Value ):
	File.write( struct.pack( Endianness + "B", Value ) )

def WriteUInt16( File, Endianness, Value ):
	File.write( struct.pack( Endianness + "H", Value ) )

def WriteSInt16( File, Endianness, Value ):
	File.write( struct.pack( Endianness + "h", Value ) )

def WriteUInt32( File, Endianness, Value ):
	File.write( struct.pack( Endianness + "I", Value ) )

def WriteSInt32( File, Endianness, Value ):
	File.write( struct.pack( Endianness + "i", Value ) )

def WriteFloat32( File, Endianness, Value ):
	File.write( struct.pack( Endianness + "f", Value ) )
