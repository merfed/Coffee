/*
 * main.cpp
 *
 *  Created on: 22.10.2009
 *      Author: Bastian
 */

#include "libm2/modeleditor.h"

int main(int argc, char **argv) {
	FILE* Input;
	int FileSize;
	char*f;
	Input=fopen(argv[1],"rb+");
	if(Input==NULL)
	{
		printf(" ERROR: Could not load file\n");
		return 0;
	}
	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);

	f=new char[FileSize];

	fread(f,FileSize,1,Input);
	fclose(Input);

	ModelEditor me=ModelEditor(f,FileSize);
	me.calcVertexBox();

	printf("Saving Changes\n");
	Input=fopen(argv[1],"wb");
	fwrite(f,FileSize,1,Input);
	fclose(Input);

	return 0;

}
