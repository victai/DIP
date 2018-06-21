#include "bits/stdc++.h"

#define Size 256
using namespace std;

unsigned char Original_img[Size][Size];

class Image{
	private:
		unsigned char ImageData[Size][Size];
	public:
		Image();
		void decrease_brightness(int factor){
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					ImageData[i][j] /= 3;
		}
		void hist_equal(){
			int cnt[Size] = {0};
			float cdf[Size] = {0};
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					cnt[ImageData[i][j]]++;
			cdf[0] = float(cnt[0]) / (Size*Size);
			for(int i = 1; i < Size; i++)
				cdf[i] = cdf[i-1] + float(cnt[i]) / (Size*Size);
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					ImageData[i][j] = char(round(cdf[ImageData[i][j]]*255));
		}
		void local_hist_equal(int winsize){
			int length = (winsize-1)/2;
			unsigned char tmp[Size][Size];
			for(int i = 0; i < Size; i++){
				for(int j = 0; j < Size; j++){
					int rank = 0;
					int cnt = 0;
					for(int k = i-length; k <= i+length; k++){
						if(k < 0 || k >= Size)	continue;
						for(int l = j-length; l <= j+length; l++){
							if(l < 0 || l >= Size)	continue;
							cnt++;
							if(ImageData[i][j] > ImageData[k][l])	rank++;
						}
					}
					tmp[i][j] = round(rank*255 / cnt);
				}
			}
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					ImageData[i][j] = tmp[i][j];
		}
		void log_trans(){
			double M = 0;
			double tmp[Size][Size];
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					tmp[i][j] = log(ImageData[i][j]+1);
					if(tmp[i][j] > M)	M = tmp[i][j];
				}
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					ImageData[i][j] = round(tmp[i][j]*255/M);
				}
		}
		void inv_log_trans(double base){
			double M = 0;
			double tmp[Size][Size];
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					tmp[i][j] = pow(base, ImageData[i][j]);
					if(tmp[i][j] > M)	M = tmp[i][j];
				}
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					ImageData[i][j] = round(tmp[i][j]*255/M);
				}
		}
		
		void power_trans(double gamma){
			double M = 0;
			double tmp[Size][Size];
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					tmp[i][j] = pow(ImageData[i][j], gamma);
					if(tmp[i][j] > M)	M = tmp[i][j];
				}
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					ImageData[i][j] = round(tmp[i][j]*255/M);
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
	for(int i = 0; i < Size; i++)
		for(int j = 0; j < Size; j++)
			ImageData[i][j] = Original_img[i][j];
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

	Image D, H, L, LOG, INV_LOG, POWER;
	D.decrease_brightness(3);
	D.output(argv[2]);
	H.decrease_brightness(3);
	H.hist_equal();
	H.output(argv[3]);
	L.decrease_brightness(3);
	L.local_hist_equal(21);
	L.output(argv[4]);
	LOG.decrease_brightness(3);
	LOG.log_trans();
	LOG.output(argv[5]);
	INV_LOG.decrease_brightness(3);
	INV_LOG.inv_log_trans(1.03);
	INV_LOG.output(argv[6]);
	POWER.decrease_brightness(3);
	POWER.power_trans(0.5);
	POWER.output(argv[7]);

	return 0;
}	
