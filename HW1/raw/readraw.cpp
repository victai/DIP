#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>
using namespace std;

ifstream fin;

int main(void){
	unsigned int img[64][64];
	FILE *fp = fopen("sample1.raw", "rb");
	for(int i = 0; i < 64; i++)
		for(int j = 0; j < 64; j++){
			fscanf(fp, "%d", &img[i][j]);
			printf("%d\n", img[i][j]);
		}

	/*
	fstream f("sample1.raw", ios::in | ios::binary);
	f.seekg(0, ios::end);
	int fl_sz = f.tellg();
	cout << fl_sz << endl;
	char ch;
	string str;
	f.get(ch);
	cout << ch << endl;
	//while(!f.eof()){
	//	f >> ch;
	//	cout << ch << endl;
	//}

	f.close();
	*/
	return 0;
}
