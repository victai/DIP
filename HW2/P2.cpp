#include "bits/stdc++.h"

#define Size 512
using namespace std;

unsigned char Original_img[Size][Size];

class Image{
	private:
		unsigned char ImageData[Size][Size];
	public:
		Image();
		void high_pass(){
			int filter_sz = 3;
			double filter[filter_sz][filter_sz] = {{0,-1,0}, {-1,5,-1}, {0,-1,0}};
			//double filter[filter_sz][filter_sz] = {{-1,-1,-1}, {-1,9,-1}, {-1,-1,-1}};
			double sum = 0;
			for(int i = 0; i < filter_sz; i++)
				for(int j = 0; j < filter_sz; j++)
					sum += filter[i][j];
			int length = (filter_sz-1)/2;
			double tmp[Size][Size];
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					double new_val = 0;
					for(int k = 0; k < filter_sz; k++){
						if(i-length+k < 0 || i-length+k >= Size) continue;
						for(int l = 0; l < filter_sz; l++){
							if(j-length+l < 0 || j-length+l >= Size) continue;
							new_val += (filter[k][l]* \
									   ImageData[i-length+k][j-length+l]);
						}
					}
					if(new_val < 0)	new_val = 0;
					tmp[i][j] = new_val;
				}
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					ImageData[i][j] = round(tmp[i][j]);
		}
		void low_pass(int filter_sz, double sum, double* filter){
			int length = (filter_sz-1)/2;
			double tmp[Size][Size];
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					double new_val = 0;
					for(int k = 0; k < filter_sz; k++){
						if(i-length+k < 0 || i-length+k >= Size) continue;
						for(int l = 0; l < filter_sz; l++){
							if(j-length+l < 0 || j-length+l >= Size) continue;
							new_val += (*(filter+k*filter_sz+l))* \
									   (ImageData[i-length+k][j-length+l]);
						}
					}
					tmp[i][j] = new_val;
				}
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					ImageData[i][j] = round(tmp[i][j] / sum);
		}
		void Unsharp_Masking(double c){
			int filter_sz = 3;
			double filter[filter_sz][filter_sz] = {{1,4,1}, {4,16,4}, {1,4,1}};
			double sum = 0;
			for(int i = 0; i < filter_sz; i++)
				for(int j = 0; j < filter_sz; j++)
					sum += filter[i][j];
			low_pass(filter_sz, sum, &filter[0][0]);
			
			unsigned char tmp[Size][Size];
			memcpy(tmp, ImageData, sizeof(unsigned char)*Size*Size);
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					int a = round(Original_img[i][j]*c/(2*c-1) - ImageData[i][j]*(1-c)/(2*c-1));
					if(a < 0)	a = 0;
					ImageData[i][j] = a;
				}
		}
		void warping(){
			double x_, y_;
			unsigned char tmp[Size][Size] = {{0}};
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					if(i<=Size/2 && ((j<=Size/2 && j>=i) || (j>=Size/2 && j<=Size-i))){ //上
						x_ = 15*sin(10*M_PI*i/Size);
						y_ = 15*sin(10*M_PI*j/Size);
					}
					else if(j<Size/2 && ((i<Size/2 && j<i) || (i>Size/2 && i<Size-j))){ //左
						x_ = 15*sin(4*M_PI*i/Size);
						y_ = 15*sin(4*M_PI*j/Size);
					}
					else if(i>=Size/2 && ((j<=Size/2 && i>=Size-j) || (j>=Size/2 && i>=j))){ //下
						x_ = 30*sin(8*M_PI*i/Size);
						y_ = 30*sin(8*M_PI*j/Size);
					}
					else{	//右
						x_ = 20*sin(3*M_PI*i/Size);
						y_ = 20*sin(3*M_PI*j/Size);
					}
					int pos_x = round(i+y_);
					int pos_y = round(j+x_);
					if(pos_x >=0 && pos_x < Size && pos_y >=0 && pos_y < Size)
						tmp[i][j] = ImageData[pos_x][pos_y];
				}
			memcpy(ImageData, tmp, sizeof(unsigned char)*Size*Size);
		}
		void output(char out[100]){
			FILE *fout;
			if (!(fout=fopen(out,"wb"))){
				cout<<"Cannot open file!"<<endl;
				exit(1);
			}
			fwrite(ImageData, sizeof(unsigned char), Size*Size, fout);
			fclose(fout);
		}

};
Image::Image(){
	memcpy(ImageData, Original_img, sizeof(unsigned char)*Size*Size);
}

int main(int argc, char **argv)
{
	FILE *fin;
	if (!(fin=fopen(argv[1],"rb"))){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	fread(Original_img, sizeof(unsigned char), Size*Size, fin);
	fclose(fin);
	
	Image HIGH, UNSHARP, WARP;
	HIGH.high_pass();
	HIGH.output(argv[2]);
	UNSHARP.Unsharp_Masking(0.65);
	UNSHARP.output(argv[3]);
	WARP.high_pass();
	WARP.warping();
	WARP.output(argv[4]);

	return 0;
}	
