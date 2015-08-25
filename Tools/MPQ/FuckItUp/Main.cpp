#define __STORMLIB_SELF__
#include <stdio.h>
#include <stdlib.h>

#include "StormLib.h"
#include "StormCommon.h"

int main(int argc, char** argv)
{
  printf("-----------------------------------------------\n");
  printf("FuckItUp - Partly fuck up an MPQ to protect it.\n");
  printf(" (c) Schlumpf 2011 --  DO NOT DISTRIBUTE.\n");
  printf("-----------------------------------------------\n");
  if(argc != 2 && argc != 3)
  {
    printf(">> Usage: %s inputmpq [outputmpq]\n\n", argv[0]);
    return -1;
  }
  
  if(argc == 3)
  {
    FILE* input = fopen(argv[1],"r");
    FILE* output = fopen(argv[2],"w+");
    fseek(input,0,SEEK_END);size_t size = ftell(input);fseek(input,0,SEEK_SET);
    char* temp = new char[size];
    fread(temp,size,1,input);
    fwrite(temp,size,1,output);
    fclose(input);
    fclose(output);
    delete[] temp;
  }

  HANDLE hMpq;
  printf("Opening archive %s ...\n", argv[1]);
  if(SFileOpenArchive(argv[argc-1], 0, 0, &hMpq))
  {
    printf("Fucking up archive ...\n");
    if(SFileRemoveFile(hMpq, LISTFILE_NAME, SFILE_OPEN_FROM_MPQ))
    {
      printf("Compressing archive ...\n");
      if(SFileCompactArchive(hMpq, NULL))
      {
        if(SFileCloseArchive(hMpq))
        {
          printf("Done. Saved to %s.\n",argv[argc-1]);
          return 0;
        }
      }
    }
  }
  printf("Failed.\n");
}
