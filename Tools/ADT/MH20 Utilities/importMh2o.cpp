#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "wowfiles/lichking/AdtLk.h"

int main(int argc, char **argv)
{
  AdtLk inputAdt = AdtLk(argv[1]);

  inputAdt.importMh2o(argv[2]);
  
  inputAdt.toFile();
	
  return 0;
}