
cdef extern from 'Python.h':
	object PyString_FromStringAndSize(char *s, int size)
	char* malloc(int size)
	void free(char * p)

cdef extern from 'mpq.h':
	int LIBMPQ_EFILE_NOT_FOUND
	int LIBMPQ_FILE_UNCOMPRESSED_SIZE
	int LIBMPQ_MPQ_NUMFILES
	int LIBMPQ_MPQ_UNCOMPRESSED_SIZE
	
	ctypedef struct mpq_archive:
		char *filename
		unsigned int blocksize
	ctypedef struct mpq_file:
		char *filename # doesnt work :(
		#unsigned int filepos
		#unsigned int accessed
	
	#char *libmpq_version()
	int libmpq_archive_open(mpq_archive *mpq_a, char *mpq_filename)
	int libmpq_archive_close(mpq_archive *mpq_a)
	int libmpq_archive_info(mpq_archive *mpq_a, unsigned int infotype)
	# *dest must have enough space
	int libmpq_file_getdata(mpq_archive *mpq_a, int number, char *dest)
	int libmpq_file_info(mpq_archive *mpq_a, unsigned int infotype, int number)
	int libmpq_file_number(mpq_archive *mpq_a, char *name)
	#int libmpq_file_number_from_hash(mpq_archive *mpq_a, unsigned int hash1, unsigned int hash2)
	#int libmpq_file_check(mpq_archive *mpq_a, void *file, int type)
	#int libmpq_hash_filename(mpq_archive *mpq_a, unsigned char *pbKey, unsigned int *seed1, unsigned int*seed2)
	#int libmpq_pkzip_decompress(char *out_buf, int *out_length, char *in_buf, int in_length)
	#int libmpq_zlib_decompress(char *out_buf, int *out_length, char *in_buf, int in_length)
	#int libmpq_huff_decompress(char *out_buf, int *out_length, char *in_buf, int in_length)
	#int libmpq_wave_decompress_stereo(char *out_buf, int *out_length, char *in_buf, int in_length)
	#int libmpq_wave_decompress_mono(char *out_buf, int *out_length, char *in_buf, int in_length)
	#int libmpq_multi_decompress(char *out_buf, int *pout_length, char *in_buf, int in_length)


cdef class Archive:
	cdef mpq_archive mpq_a
	
	def __init__(self, fn):
		if libmpq_archive_open(&self.mpq_a, fn) < 0:
			raise IOError, 'unable to open archive: %s'%fn
	
	def read(self, fn):
		cdef char * buf
		cdef int fnum
		cdef int size
		cdef int ret
		fnum = libmpq_file_number(&self.mpq_a, fn)
		size = libmpq_file_info(&self.mpq_a, LIBMPQ_FILE_UNCOMPRESSED_SIZE, fnum)
		buf = malloc(size)
		ret = libmpq_file_getdata(&self.mpq_a, fnum, buf)
		if ret < 0:
			free(buf)
			raise IOError, 'unable to read from file (number:%d, size:%d), errcode:%d'%(fnum, size, ret)
		s = PyString_FromStringAndSize(buf, size)
		free(buf)
		return s
	
	def namelist(self):
		cdef char * buf
		cdef int ret
		fnum = libmpq_archive_info(&self.mpq_a, LIBMPQ_MPQ_NUMFILES) - 1
		size = libmpq_file_info(&self.mpq_a, LIBMPQ_FILE_UNCOMPRESSED_SIZE, fnum)
		buf = malloc(size)
		ret = libmpq_file_getdata(&self.mpq_a, fnum, buf)
		if ret < 0:
			free(buf)
			raise IOError, 'unable to read from file (number:%d, size:%d), errcode:%d'%(fnum, size, ret)
		s = PyString_FromStringAndSize(buf, size)
		free(buf)
		return s.splitlines()
	
	def close(self):
		libmpq_archive_close(&self.mpq_a)
		#self.mpq_a = NULL
	
	#def filenumber(self, fn):
	#	fnum = libmpq_file_number(&self.mpq_a, fn)
	#	if fnum == LIBMPQ_EFILE_NOT_FOUND:
	#		raise IOError, 'No such file in archive: %s'%fn
	#	return fnum
	
	#def has_file(self, fn):
	#	return self.__contains__(fn)
	
	def __contains__(self, fn):
		return libmpq_file_number(&self.mpq_a, fn) != LIBMPQ_EFILE_NOT_FOUND
	
	def __del__(self):
		self.close()
	
	property name:
		def __get__(self):
			return self.mpq_a.filename
		
	property blocksize:
		def __get__(self):
			return self.mpq_a.blocksize
	
	property size:
		def __get__(self):
			return libmpq_archive_info(&self.mpq_a, LIBMPQ_MPQ_UNCOMPRESSED_SIZE)
	
	property numfiles:
		def __get__(self):
			return libmpq_archive_info(&self.mpq_a, LIBMPQ_MPQ_NUMFILES)
	
	#property closed
	

cdef class File:
	cdef mpq_file mpq_f
	cdef readonly Archive archive
	cdef readonly unsigned int size
	cdef readonly int number
	
	def __init__(self, fn, Archive archive not None):
		cdef int fnum
		fnum = libmpq_file_number(&archive.mpq_a, fn)
		if fnum != LIBMPQ_EFILE_NOT_FOUND:
			self.archive = archive
			self.number = fnum
			self.size = libmpq_file_info(&archive.mpq_a, LIBMPQ_FILE_UNCOMPRESSED_SIZE, fnum)
		else:
			raise IOError, 'unable to open file: %s'%fn
	
	def close(self):
		pass
	
	def __del__(self):
		self.close()
	
	def read(self, size = -1):
		# size param omitted
		cdef char * buf
		cdef int ret
		buf = malloc(self.size)
		ret = libmpq_file_getdata(&self.archive.mpq_a, self.number, buf)
		if ret < 0:
			free(buf)
			raise IOError, 'unable to read from file (number:%d, size:%d), errcode:%d'%(self.number, self.size, ret)
		s = PyString_FromStringAndSize(buf, self.size)
		free(buf)
		return s
	
	property name:
		def __get__(self):
			return self.mpq_f.filename
	
