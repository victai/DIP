#include "bits/stdc++.h"
#include <random>

#define Size 256
using namespace std;

unsigned char Original_img[Size][Size];

class Image{
	private:
		unsigned char ImageData[Size][Size];
	public:
		Image();
		void gaussian_noise(double mu, double sigma){
			default_random_engine generator;
			normal_distribution<double> distribution(mu, sigma);
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					float tmp = ImageData[i][j];
					tmp += round(distribution(generator));
					tmp = max(0.0, double(tmp));
					tmp = min(255.0, double(tmp));
					ImageData[i][j] = tmp;
				}
		}
		void salt_and_pepper(double salt, double pepper){
			srand(time(NULL));
			int s=0;
			int p=0;
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					double num = double(rand())/RAND_MAX;
					if(num <= salt)			ImageData[i][j] = 255;
					else if(num >= pepper)	ImageData[i][j] = 0;
				}
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
		void median_filter(int filter_sz){
			int length = (filter_sz-1)/2;
			double tmp[Size][Size];
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					vector<int> vec;
					int cnt = 0;
					for(int k = i-length; k <= i+length; k++){
						if(k < 0 || k >= Size) continue;
						for(int l = j-length; l <= j+length; l++){
							if(l < 0 || l >= Size) continue;
							vec.push_back(ImageData[k][l]);
							cnt++;
						}
					}
					sort(vec.begin(), vec.end());
					tmp[i][j] = vec[round(cnt/2)];
				}
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					ImageData[i][j] = tmp[i][j];
		}
		void outlier(double epsilon, int filter_sz){
			int length = (filter_sz-1)/2;
			int tmp[Size][Size];
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					double sum = 0;
					int cnt = 0;
					int black_cnt = 0;
					for(int k = i-length; k <= i+length; k++){
						if(k < 0 || k >= Size)	continue;
						for(int l = j-length; l <= j+length; l++){
							if(l < 0 || l >= Size)	continue;
							if(k == i && l == j)	continue;
							sum += ImageData[k][l];
							cnt++;
							if(ImageData[k][l] < 100)	black_cnt++;
						}
					}
					if(fabs(ImageData[i][j] - sum/cnt) > epsilon && black_cnt<50)
						tmp[i][j] = round(sum/cnt);
					else	tmp[i][j] = ImageData[i][j];
				}
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					ImageData[i][j] = tmp[i][j];
			
		}
		double PSNR(){
			double mse = 0;
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					mse += pow(Original_img[i][j]-ImageData[i][j], 2);
			printf("MSE: %lf\n", mse);
			mse /= Size*Size;
			printf("PSNR: %lfdb\n", 10*log10(255*255/mse));
		}
		void wrinkle(){
			int filter_sz = 3;
			double filter[filter_sz][filter_sz] = {{1,1,1}, {1,5,1}, {1,1,1}};
			double sum = 0;
			for(int i = 0; i < filter_sz; i++)
				for(int j = 0; j < filter_sz; j++)
					sum += filter[i][j];
			outlier(5, 13);
			low_pass(filter_sz, sum, &filter[0][0]);
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


	int filter_sz = 3;
	double filter[filter_sz][filter_sz] = {{1,1,1}, {1,15,1}, {1,1,1}};
	double sum = 0;
	for(int i = 0; i < filter_sz; i++)
		for(int j = 0; j < filter_sz; j++)
			sum += filter[i][j];
	Image G1, G2, S1, S2, RG, RS;
	G1.gaussian_noise(0, 10);
	G1.output(argv[3]);
	G2.gaussian_noise(0, 30);
	G2.output(argv[4]);
	S1.salt_and_pepper(0.05, 0.95);
	S1.output(argv[5]);
	S2.salt_and_pepper(0.15, 0.95);
	S2.output(argv[6]);
	G1.low_pass(filter_sz, sum, &filter[0][0]);
	G1.output(argv[7]);
	S1.median_filter(3);
	S1.output(argv[8]);
	if (!(fin=fopen(argv[2],"rb"))){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	fread(Original_img, sizeof(unsigned char), Size*Size, fin);
	fclose(fin);
	Image WRINKLE;
	WRINKLE.wrinkle();
	WRINKLE.output(argv[9]);

	return 0;
}	
