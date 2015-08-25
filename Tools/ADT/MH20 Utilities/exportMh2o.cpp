#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "wowfiles/lichking/AdtLk.h"

int main(int argc, char **argv)
{
  AdtLk inputAdt = AdtLk(argv[1]);

  inputAdt.mh2oToFile();
	
  return 0;
}