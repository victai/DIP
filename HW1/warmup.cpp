#include "stdio.h"
#include "iostream"
#include "stdlib.h"

#define Size 256
using namespace std;

int main(int argc, char **argv)
{
	FILE *file;
	unsigned char Imagedata[3][Size][Size];
	unsigned char new_Imagedata[Size][Size] = {{0}};
	if (!(file=fopen(argv[1],"rb"))){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*3, file);
	fclose(file);

	for(int i = 0; i < Size; i++){
		for(int j = 0; j < Size; j++){
			new_Imagedata[i][j] += 0.30*Imagedata[0][i][j] + \
								   0.59*Imagedata[1][i][j] + \
								   0.11*Imagedata[2][i][j];
		}
	}

	unsigned char trans_img[Size][Size];
	for(int i = 0; i < Size; i++)
		for(int j = 0; j < Size; j++)
			trans_img[i][j] = new_Imagedata[j][i];
	
	if (!(file=fopen(argv[2],"wb"))){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}

	fwrite(trans_img, sizeof(unsigned char), Size*Size, file);
	fclose(file);
	
	return 0;
}	
