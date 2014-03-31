# Removing Events, Lights, Ribbon Emitters and Particle Emitters from a Model #
To remove events from a model you have to use any hex editor you can find and set the number of whatever you want to remove to zero. Do so by opening the file in a hex editor and set the number at the offset for the number of events to zero.

	#include <cstdio>
	int main(int, char** argv)
	{
	  FILE* f (fopen (argv[1], "rw"));
	  const int null (0);
	  fseek (f, 0x100, SEEK_SET);
	  fwrite (&null, 4, 1, f);
	  fwrite (&null, 4, 1, f);
	  fclose (f);
	  return 0;
	}