#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

// types used to define data-types, not referenced within the wdb files.
enum e_t_coltype {
	CT_FLOAT = 0,
	CT_STRING,
	CT_BITMASK,
	CT_U8,
	CT_U16,
	CT_U32,
	CT_S8,
	CT_S16,
	CT_S32
};

// the wdb header
struct {
	char		signature[4];
	uint32_t	version;
	char		language[4];
	uint32_t	rowcount;
	uint32_t	unknown;
} header;

struct {
	uint32_t	version;
	char		signature[4];

	char		*wdbschema;
	char		*sqlschema;
} config;

//   enumerated array of our column types.
enum e_t_coltype	field_types[1024];
int			field_count;

// reverses a NON-NULL-TERMINATED string, and returns a newly allocated copy.
// user must free the result manually using free();
char *revstring( char *stringIn, int slen ) {
	int x;
	char *stringOut = malloc( slen+1 );
	for( x=0; x < slen; x++ ) {
		stringOut[x] = stringIn[ slen-x-1 ];
	}
	stringOut[slen] = '\0';
	return stringOut;
}

// quotes a string for insert into a database by escaping apostrophes.
// user must free the result manually using free();
char *quotestring( char *stringIn ) {
	int slen = strlen(stringIn);
	char *stringOut = malloc( slen * 2 + 1 );
	int x, y=0;

	for( x=0; x < slen; x++ ) {
		if( stringIn[x] == '\'' ) {
			stringOut[y] = '\'';
			y++;
			stringOut[y] = '\'';
			y++;
		} else {
			stringOut[y] = stringIn[x];
			y++;
		}
	}
	stringOut[y] = '\0';

	return stringOut;
}

int read_header( FILE *file ) {
	int readin = fread( &header, 1, sizeof(header), file );
	if( readin < sizeof(header) ) {
		fprintf( stderr, "Short read parsing file header.\n" );
		return -1;
	}

	char *fixsig = revstring(header.signature, 4);
	char *fixlang = revstring(header.language, 4);
	fprintf( stderr, "Signature: '%s' / Language: '%s' / Version: %u / RowCount: %u / Unknown: %u\n",
		fixsig, fixlang, header.version, header.rowcount, header.unknown );

	memcpy( header.signature, fixsig, 4 );
	memcpy( header.language, fixlang, 4 );

	free( fixsig );
	free( fixlang );

	return 0;
}

int read_bytes( void *target, size_t length, FILE *file, int x, int y ) {
	int bytesIn = fread( target, 1, length, file );
	if( bytesIn < length ) {
		fprintf( stderr, "Short read on record %d field %d\n", y, x );
		return -1;
	}
	return bytesIn;
}

void sqlparse( char **line, int key, void *data ) {
	char *lineIn = *line;
	
	char numBuf[1024];
	char *fOut = NULL;
	if( field_types[key] == CT_FLOAT ) {
		fOut = numBuf;
		snprintf( numBuf, sizeof(numBuf), "%f", *(float *)data );
	} else if( field_types[key] == CT_BITMASK ) {
		fOut = numBuf;
		snprintf( numBuf, sizeof(numBuf), "%08x", *(uint32_t *)data );
	} else if( field_types[key] == CT_STRING ) {
		fOut = (char *)data;
	} else if( field_types[key] == CT_U8 ) {
		fOut = numBuf;
		snprintf( numBuf, sizeof(numBuf), "%u", *(uint8_t *)data );
	} else if( field_types[key] == CT_U16 ) {
		fOut = numBuf;
		snprintf( numBuf, sizeof(numBuf), "%u", *(uint16_t *)data );
	} else if( field_types[key] == CT_U32 ) {
		fOut = numBuf;
		snprintf( numBuf, sizeof(numBuf), "%u", *(uint32_t *)data );
	} else if( field_types[key] == CT_S8 ) {
		fOut = numBuf;
		snprintf( numBuf, sizeof(numBuf), "%d", *(int8_t *)data );
	} else if( field_types[key] == CT_S16 ) {
		fOut = numBuf;
		snprintf( numBuf, sizeof(numBuf), "%d", *(int16_t *)data );
	} else if( field_types[key] == CT_S32 ) {
		fOut = numBuf;
		snprintf( numBuf, sizeof(numBuf), "%d", *(int32_t *)data );
	}

	char keyStr[32];
	long posStart=0;
	long x, lineLen = strlen(lineIn);
	int keyLen, valLen = strlen(fOut);

	snprintf( keyStr, 32, "$(%d)", key+1 );
	keyLen = strlen( keyStr );

	for( x=0; x < lineLen && posStart == 0; x++ ) {
		if( strncmp( keyStr, &lineIn[x], keyLen ) == 0 ) {
			posStart = x;
		}
	}
	if( posStart == 0 ) {
//		fprintf( stderr, "Failed to find key %s in schema.  (This may mean the key is ignored.)\n", keyStr );
		return;
	}

	char *lineOut = malloc( lineLen - keyLen + valLen + 1 );
	if( !lineOut ) {
		fprintf( stderr, "Failed to allocate output SQL line for parser: %s\n", strerror(errno) );
		return;
	}

	memcpy( lineOut, lineIn, posStart );
	memcpy( &lineOut[posStart], fOut, valLen );
	memcpy( &lineOut[posStart+valLen], &lineIn[posStart+keyLen], lineLen-posStart-keyLen+1 );

	free( lineIn );
	*line = lineOut;
	return;
}

int read_data( FILE *file ) {
	int x, y;
	int bytesIn, bytesExpect;
	unsigned long recordCount = 0;
	unsigned long bytesRecord;
	unsigned long long bytesTotal = 0;
	char delim;

	union {
		char		val_string[1024];	// *probably* large enough.
		float		val_float;
		int32_t		val_bitmask;
		uint32_t	val_u32;
		uint16_t	val_u16;
		uint8_t		val_u8;
		int32_t		val_s32;
		int16_t		val_s16;
		int8_t		val_s8;
	} val;

	// rows
	for( y=0; ; y++ ) {
		delim = 0;
		bytesRecord = 0;

		char *sqlline = strdup( config.sqlschema );

		// columns
		for( x=0; x < field_count; x++ ) {
			char strIn;
			int z, rbyte;

			switch( field_types[x] ) {
				case CT_FLOAT:
					bytesIn = read_bytes( &val.val_float, sizeof(float), file, x, y );
					if( bytesIn < 0 ) return -1;
					sqlparse( &sqlline, x, &val.val_float );
					break;
				case CT_BITMASK:
					bytesIn = read_bytes( &val.val_bitmask, sizeof(uint32_t), file, x, y );
					if( bytesIn < 0 ) return -1;
					sqlparse( &sqlline, x, &val.val_bitmask );
					break;
				case CT_STRING:
					bytesIn = 0;
					z = 0;
					do { 
						rbyte = fread( &strIn, 1, sizeof(char), file );
						if( rbyte <= 0 ) {
							fprintf( stderr, "Short read on row %d column %d.\n", y, x );
							return -1;
						}

						val.val_string[z] = strIn;
						z++;
					} while( strIn != '\0' && z < 1024 );
					if( strIn != '\0' ) {
						fprintf( stderr, "Found a string larger than 1024 bytes.  Can't read this.\n" );
						return -1;
					}

					char *quoted = quotestring(val.val_string);
					sqlparse( &sqlline, x, quoted );
					free( quoted );

					bytesExpect = 0;
					bytesRecord += z;
					break;
				case CT_U32:
					bytesIn = read_bytes( &val.val_u32, sizeof(uint32_t), file, x, y );
					if( bytesIn < 0 ) return -1;
					sqlparse( &sqlline, x, &val.val_u32 );
					break;
				case CT_U16:
					bytesIn = read_bytes( &val.val_u16, sizeof(uint16_t), file, x, y );
					if( bytesIn < 0 ) return -1;
					sqlparse( &sqlline, x, &val.val_u16 );
					break;
				case CT_U8:
					bytesIn = read_bytes( &val.val_u8, sizeof(uint8_t), file, x, y );
					if( bytesIn < 0 ) return -1;
					sqlparse( &sqlline, x, &val.val_u8 );
					break;
				case CT_S32:
					bytesIn = read_bytes( &val.val_s32, sizeof(uint32_t), file, x, y );
					if( bytesIn < 0 ) return -1;
					sqlparse( &sqlline, x, &val.val_s32 );
					break;
				case CT_S16:
					bytesIn = read_bytes( &val.val_s16, sizeof(uint16_t), file, x, y );
					if( bytesIn < 0 ) return -1;
					sqlparse( &sqlline, x, &val.val_s16 );
					break;
				case CT_S8:
					bytesIn = read_bytes( &val.val_s8, sizeof(uint8_t), file, x, y );
					if( bytesIn < 0 ) return -1;
					sqlparse( &sqlline, x, &val.val_s8 );
					break;
			}

			bytesRecord += bytesIn;
		}

		printf( sqlline );
		free( sqlline );

		bytesTotal += bytesRecord;
		recordCount++;
	
		/* Here we check for a terminator.
		 * We do this by reading 8 bytes, checking if it's all-zero, THEN check for feof, and if
		 * this is all true we say 'done'.  Otherwise, we rewind back 8 bytes and carry on.
		 */
		uint64_t term[2];
		fpos_t fpos;

		if( fgetpos( file, &fpos ) != 0 ) {
			fprintf( stderr, "Could not get current cursor position: %s\n", strerror(errno) );
			return -1;
		}

		if( fread( term, 8, 2, file ) < 1 ) {
			fprintf( stderr, "Failed to check for terminator: %s\n", strerror(errno) );
			return -1;
		}

		if( term[0] == 0 && feof(file) ) {
			fprintf( stderr, "8-byte terminator received, reading finished.  Parsed %llu bytes in %lu records.\n",
				bytesTotal, recordCount );
			return 0;
		}

		if( fsetpos( file, &fpos ) != 0 ) {
			fprintf( stderr, "Failed to rewind 8 bytes: %s\n", strerror(errno) );
			return -1;
		}
	}

	return 0;
}

int read_file( char *filepath ) {
	FILE *file = fopen( filepath, "r" );
	if( !file ) {
		fprintf( stderr, "Failed to open file '%s': %s\n", filepath, strerror(errno) );
		return -1;
	}

	if( read_header( file ) < 0 ) {
		return -1;
	}

	if( memcmp( config.signature, header.signature, 4 ) != 0 ) {
		fprintf( stderr, "This WDB file is not compatible with this format file.\n" );
		fprintf( stderr, "WDB: %c%c%c%c / FMT: %c%c%c%c\n",
			header.signature[0],
			header.signature[1],
			header.signature[2],
			header.signature[3],
			config.signature[0],
			config.signature[1],
			config.signature[2],
			config.signature[3] );
		return -1;
	}

	if( config.version != header.version ) {
		fprintf( stderr, "This WDB file version is not compatible with this format file version.\n" );
		return -1;
	}

	if( read_data( file ) < 0 ) {
		return -1;
	}

	return 0;
}

int set_wdbtypes_from_schema() {
	long x, y=0, kpos=0;
	char key[8];
	long schemaLen = strlen(config.wdbschema);
	
	key[0] = '\0';

	for( x=0; x<schemaLen && kpos < 8; x++ ) {
		if( config.wdbschema[x] == ' '
		 || config.wdbschema[x] == '\r'
		 || config.wdbschema[x] == '\n'
		 || config.wdbschema[x] == '\t' ) {
			if( key[0] != '\0' ) {
				if( strcmp( key, "s" ) == 0 )
					field_types[y] = CT_STRING;
				else if( strcmp( key, "b" ) == 0 )
					field_types[y] = CT_BITMASK;
				else if( strcmp( key, "f" ) == 0 )
					field_types[y] = CT_FLOAT;
				else if( strcmp( key, "u32" ) == 0 )
					field_types[y] = CT_U32;
				else if( strcmp( key, "u16" ) == 0 )
					field_types[y] = CT_U16;
				else if( strcmp( key, "u8" ) == 0 )
					field_types[y] = CT_U8;
				else if( strcmp( key, "s32" ) == 0 )
					field_types[y] = CT_S32;
				else if( strcmp( key, "s16" ) == 0 )
					field_types[y] = CT_S16;
				else if( strcmp( key, "s8" ) == 0 )
					field_types[y] = CT_S8;
				else {
					fprintf( stderr, "Invalid WDB key #%lu: %s\n", y, key );
					return -1;
				}
				y++;
				kpos = 0;
				key[0] = '\0';
			} else {
				// I guess there's a space in front.
			}
		} else {
			key[kpos] = config.wdbschema[x];
			kpos++;
			key[kpos] = '\0';
		}
	}
	field_count = y;
	fprintf( stderr, "%d WDB fields defined.\n", field_count );

	return field_count-1;
}

int parse_format( int argc, char **argv ) {
	int x, y=0;
	for( x=2; x < argc; x++ ) {
		if( strcmp( argv[x], "f" ) == 0 ) {
			field_types[y] = CT_FLOAT;
		} else if( strcmp( argv[x], "s" ) == 0 ) {
			field_types[y] = CT_STRING;
		} else if( strcmp( argv[x], "b" ) == 0 ) {
			field_types[y] = CT_BITMASK;
		} else if( strcmp( argv[x], "u8" ) == 0 ) {
			field_types[y] = CT_U8;
		} else if( strcmp( argv[x], "u16" ) == 0 ) {
			field_types[y] = CT_U16;
		} else if( strcmp( argv[x], "u32" ) == 0 ) {
			field_types[y] = CT_U32;
		} else if( strcmp( argv[x], "s8" ) == 0 ) {
			field_types[y] = CT_S8;
		} else if( strcmp( argv[x], "s16" ) == 0 ) {
			field_types[y] = CT_S16;
		} else if( strcmp( argv[x], "s32" ) == 0 ) {
			field_types[y] = CT_S32;
		} else {
			fprintf( stderr, "Unknown field type: '%s'\n", argv[x] );
			return -1;
		}
		y++;
	}
	field_count = y;
	return 0;
}

int set_wdb( char *inBuf ) {
	long bufLen = strlen(inBuf);
	char *newLine;
	if( config.wdbschema ) {
		long curLen = strlen( config.wdbschema );
		newLine = malloc( curLen + bufLen + 1 );
		newLine[0] = '\0';
		strncpy( newLine, config.wdbschema, curLen+1 );
		strncpy( &newLine[ curLen ], inBuf, bufLen+1 );
		free( config.wdbschema );
		config.wdbschema = newLine;
	} else {
		newLine = malloc( bufLen + 1 );
		newLine[0] = '\0';
		strncpy( newLine, inBuf, bufLen+1 );
		config.wdbschema = newLine;
	}
	return 0;
}

int set_sql( char *inBuf ) {
	long bufLen = strlen(inBuf);
	char *newLine;
	if( config.sqlschema ) {
		long curLen = strlen( config.sqlschema );
		newLine = malloc( curLen + bufLen + 1 );
		newLine[0] = '\0';
		strncpy( newLine, config.sqlschema, curLen+1 );
		strncpy( &newLine[ curLen ], inBuf, bufLen+1 );
		free( config.sqlschema );
		config.sqlschema = newLine;
	} else {
		newLine = malloc( bufLen + 1 );
		newLine[0] = '\0';
		strncpy( newLine, inBuf, bufLen+1 );
		config.sqlschema = newLine;
	}
	return 0;
}

int set_version( char *inBuf ) {
	int bufLen = strlen(inBuf);
	if( inBuf[bufLen-1] == '\n' )
		inBuf[bufLen-1] = '\0';
	bufLen = strlen(inBuf);

	// For winlosers
	if( inBuf[bufLen-1] == '\r' )
		inBuf[bufLen-1] = '\0';
	bufLen = strlen(inBuf);

	config.version = strtol( inBuf, NULL, 10 );
	return 0;
}

int set_signature( char *inBuf ) {
	int bufLen = strlen(inBuf);
	if( inBuf[bufLen-1] == '\n' )
		inBuf[bufLen-1] = '\0';
	bufLen = strlen(inBuf);

	// For winlosers
	if( inBuf[bufLen-1] == '\r' )
		inBuf[bufLen-1] = '\0';
	bufLen = strlen(inBuf);

	if( bufLen != 4 ) {
		fprintf( stderr, "A 4-byte signature is expected.\n" );
		return -1;
	}

	int x;
	for( x=0; x < 4; x++ ) {
		config.signature[x] = inBuf[x];
	}

	return 0;
}

int parse_config( char *configPath ) {
	unsigned long linesRead = 0;

	FILE *file = fopen( configPath, "r" );
	if( !file ) {
		fprintf( stderr, "Failed to open format file: %s\n", strerror(errno) );
		return -1;
	}

	enum {
		STAGE_NONE,
		STAGE_SIGNATURE,
		STAGE_VERSION,
		STAGE_WDB,
		STAGE_SQL
	} stage = STAGE_NONE;

	while( !feof( file ) ) {
		char lineBuf[4096];
		if( fgets( lineBuf, 4096, file ) == NULL ) {
			fprintf( stderr, "End of file: %s (Read %lu lines)\n", strerror(errno), linesRead );
			goto endfile;
		}
		if( strncmp( lineBuf, "//", 2 ) == 0 ) {
			// comment, ignore it.
		} else if( strncmp( lineBuf, "+signature", 10 ) == 0 ) {
			stage = STAGE_SIGNATURE;
		} else if( strncmp( lineBuf, "+version", 8 ) == 0 ) {
			stage = STAGE_VERSION;
		} else if( strncmp( lineBuf, "+wdb", 4 ) == 0 ) {
			stage = STAGE_WDB;
		} else if( strncmp( lineBuf, "+sql", 4 ) == 0 ) {
			stage = STAGE_SQL;
		} else {
			switch( stage ) {
				case STAGE_NONE:
					fprintf( stderr, "Junk line: %s", lineBuf );
					break;
				case STAGE_SIGNATURE:
					if( set_signature( lineBuf ) < 0 )
						return -1;
					break;
				case STAGE_VERSION:
					if( set_version( lineBuf ) < 0 )
						return -1;
					break;
				case STAGE_WDB:
					if( set_wdb( lineBuf ) < 0 )
						return -1;
					break;
				case STAGE_SQL:
					if( set_sql( lineBuf ) < 0 )
						return -1;
					break;
			}
		}
		linesRead++;
	}

endfile:
	fclose( file );

	// convert our wdb schema to type keys we can easily reference.
	return set_wdbtypes_from_schema();
}

void show_help( char *selfname ) {
	fprintf( stderr, "Usage: %s <filename> <format file>\n", selfname );
	fprintf( stderr, "	Format files are used to identify the table structure, and are keyed as follows:\n" );
	fprintf( stderr, "		f	float\n" );
	fprintf( stderr, "		s	string\n" );
	fprintf( stderr, "		b	bitmask\n" );
	fprintf( stderr, "		u8	unsigned integer (8-bit, 1-byte)\n" );
	fprintf( stderr, "		u16	unsigned integer (16-bit, 2-byte)\n" );
	fprintf( stderr, "		u32	unsigned integer (32-bit, 4-byte)\n" );
	fprintf( stderr, "		s8	signed integer (8-bit, 1-byte)\n" );
	fprintf( stderr, "		s16	signed integer (16-bit, 2-byte)\n" );
	fprintf( stderr, "		s32	signed integer (32-bit, 4-byte)\n" );
	fprintf( stderr, "	Example:\n" );
						//       id  len cid sid unk n n n n did qid t bp  sp  isl c r
	fprintf( stderr, "		%s itemcache.wdb itemcache.fmt\n", selfname );
	fprintf( stderr, "	Example format file contents:\n" );
	fprintf( stderr, "		+signature\n" );
	fprintf( stderr, "		WIDB\n" );
	fprintf( stderr, "		+version\n" );
	fprintf( stderr, "		6898\n" );
	fprintf( stderr, "		+wdb\n" );
	fprintf( stderr, "		// cheesecache.wdb format\n" );
	fprintf( stderr, "		u32 f s s s32 u32 b f\n" );
	fprintf( stderr, "		+sql\n" );
	fprintf( stderr, "		// insert command\n" );
	fprintf( stderr, "		INSERT INTO cheese_templates( id, diameter, family, flavour, quality, age, texture, bouquet )\n" );
	fprintf( stderr, "			VALUES( $(1), $(2), $(3), $(4), $(5), $(6), $(7), $(8) );\n" );
	fprintf( stderr, "\n" );
	fprintf( stderr, "	(Additional format files available at http://www.sourcepeek.com/wiki/Category:WDB2SQL)\n" );
}

int main( int argc, char **argv ) {
	if( argc < 3 ) {
		show_help( argv[0] );
		return 1;
	}

	field_count = 0;
	memset( &config, 0, sizeof(config) );

	fprintf( stderr, "Parsing format...\n" );
	//if( parse_format( argc, argv ) < 0 ) {
	if( parse_config( argv[2] ) < 0 ) {
		return -1;
	}
	fprintf( stderr, "Done.  Reading file...\n" );
	if( read_file( argv[1] ) < 0 ) {
		return -1;
	}
	fprintf( stderr, "Done.\n" );

	return 0;
}
