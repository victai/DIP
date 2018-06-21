#include <bits/stdc++.h>
using namespace std;

#define Size 512

unsigned char Original_img[Size][Size];
double filter_4[3][3] = {{0,-1,0},{-1,4,-1},{0,-1,0}};
double filter_8[3][3] = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}};

void Create_Filter(double* filter, int filter_sz, double sigma){
	int len = (filter_sz-1)/2;
	double sum = 0;
	for(int i = -len; i <= len; i++)
		for(int j = -len; j <= len; j++){
			*(filter+(i+len)*filter_sz+j+len) = exp(-(pow(i,2)+pow(j,2))/(2*pow(sigma,2)))/(2*M_PI*pow(sigma,2));
			sum += *(filter+(i+len)*filter_sz+j+len);
		}
	printf("sum = %lf\n", sum);
	for(int i = 0; i < filter_sz; i++)
		for(int j = 0; j < filter_sz; j++)
			*(filter+i*filter_sz+j) /= sum/273;
}

class Image{
	private:
		unsigned char ImageData[Size][Size];
		unsigned char edge_map[Size][Size];
	public:
		Image();
		double calculate_first_1(int x, int y){
			int GR = 0, GC = 0;
			if(y > 0)		GR = ImageData[x][y] - ImageData[x][y-1];
			if(x < Size-1)	GC = ImageData[x][y] - ImageData[x+1][y];
			return pow(pow(GR,2) + pow(GC,2), 0.5);
		}
		double calculate_first_2(int x, int y){
			int GR = 0, GC = 0;
			if(x < Size-1 && y < Size-1)	GR = ImageData[x][y] - ImageData[x+1][y+1];
			if(y < Size-1 && x < Size-1)	GC = ImageData[x][y+1] - ImageData[x+1][y];
			return pow(pow(GR,2) + pow(GC,2), 0.5);
		}
		double sobel_mask(int x, int y, int K = 2){
			int GR = 0, GC = 0;
			if(x < Size-1 && x > 0 && y < Size-1 && y > 0){
				GR = round((ImageData[x-1][y+1]+K*ImageData[x][y+1]+ImageData[x+1][y+1] - \
							ImageData[x-1][y-1]+K*ImageData[x][y-1]+ImageData[x+1][y+1]) / (K+2));
				GC = round((ImageData[x-1][y-1]+K*ImageData[x-1][y]+ImageData[x-1][y+1] - \
							ImageData[x+1][y-1]+K*ImageData[x+1][y]+ImageData[x+1][y+1]) / (K+2));
			}
			return pow(pow(GR,2) + pow(GC,2), 0.5);
		}
		double calculate_second(int x, int y, int f){
			double sum = 0;
			int val;
			for(int k = 0; k < 3; k++){
				if(x-1+k < 0 || x-1+k > Size-1)	continue;
				for(int l = 0; l < 3; l++){
					if(y-1+l < 0 || y-1+l > Size-1)	continue;
					if(f == 4)			val = filter_4[k][l];
					else if(f == 8)		val = filter_8[k][l];
					sum += int(ImageData[x-1+k][y-1+l])*val;
				}
			}
			return round(sum);
		}
		void First_order_1(double threshold){
			double Gradient_map[Size][Size] = {{0}};
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					Gradient_map[i][j] = calculate_first_1(i, j);
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					if(Gradient_map[i][j] > threshold)	ImageData[i][j] = 1;
					else								ImageData[i][j] = 0;
				}
		}
		void First_order_2(double threshold){
			double Gradient_map[Size][Size] = {{0}};
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					Gradient_map[i][j] = calculate_first_2(i, j);
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					if(Gradient_map[i][j] > threshold)	ImageData[i][j] = 1;
					else								ImageData[i][j] = 0;
				}
		}
		void First_order_sobel(double threshold){
			double Gradient_map[Size][Size] = {{0}};
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					Gradient_map[i][j] = sobel_mask(i, j, 2);
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					if(Gradient_map[i][j] > threshold)	ImageData[i][j] = 1;
					else								ImageData[i][j] = 0;
				}
		}
		void Second_order(double threshold, int f){
			unsigned char tmp[Size][Size] = {{0}};
			int cnt1 = 0, cnt2=0;
			double second_order_map[Size][Size] = {{0}};
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					second_order_map[i][j] = calculate_second(i, j, 4);
				}
			for(int i = 1; i < Size-1; i++){
				for(int j = 1; j < Size-1; j++){
					if(abs(calculate_first_1(i, j-1) - calculate_first_1(i, j+1)) > threshold){
						if(calculate_second(i, j-1, f)*calculate_second(i, j+1, f) < 0){
							cnt1++;
							tmp[i][j] = 1;
						}
					}
					if(abs(calculate_first_1(i-1, j) - calculate_first_1(i+1, j)) > threshold){
						if(calculate_second(i-1, j, f)*calculate_second(i+1, j, f) < 0){
							cnt2++;
							tmp[i][j] = 1;
						}
					}
					if(abs(calculate_first_1(i-1, j-1) - calculate_first_1(i+1, j+1)) > threshold){
						if(calculate_second(i-1, j-1, f)*calculate_second(i+1, j+1, f) < 0){
							cnt2++;
							tmp[i][j] = 1;
						}
					}
					if(abs(calculate_first_1(i+1, j-1) - calculate_first_1(i-1, j+1)) > threshold){
						if(calculate_second(i+1, j-1, f)*calculate_second(i-1, j+1, f) < 0){
							cnt2++;
							tmp[i][j] = 1;
						}
					}
				}
			}
			memcpy(ImageData, tmp, sizeof(unsigned char)*Size*Size);
		}
		void Denoise(){
			double Gradient_map[Size][Size] = {{0}};
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++)
					Gradient_map[i][j] = calculate_first_2(i, j);
			FILE *fout = fopen("gradient.raw", "wb");
			fwrite(Gradient_map, sizeof(int), Size*Size, fout);
			fclose(fout);
		}
		int Connect(int x, int y){
			if(x < 0 || x > Size-1 || y < 0 || y > Size-1)	return 0;
			for(int i = -1; i < 2; i++)
				for(int j = -1; j < 2; j++){
					if(x+i < 0 || x+i > Size-1 || y+j < 0 || y+j > Size-1)	continue;
					if(edge_map[x+i][y+j] == 2){
						edge_map[x][y] = 2;
						return 2;
					}
				}
			for(int i = -1; i < 2; i++)
				for(int j = -1; j < 2; j++){
					if(x+i < 0 || x+i > Size-1 || y+j < 0 || y+j > Size-1)	continue;
					if(edge_map[x+i][y+j] == 1){
						edge_map[x+i][y+j] = Connect(x+i, y+j);
					}
				}
		}
		void Canny(int filter_sz, double TL, double TH){
			int len = (filter_sz-1)/2;
			double Gaussian_filter[filter_sz][filter_sz];
			unsigned char tmp[Size][Size];
			Create_Filter(&Gaussian_filter[0][0], filter_sz, 1);
			for(int i = 0; i < filter_sz; i++)
				for(int j = 0; j < filter_sz; j++)
					printf("%lf\n", Gaussian_filter[i][j]);
			//double filter[5][5] = {{2,4,5,4,2},{4,9,12,9,4},{5,12,15,12,5},{4,9,12,9,4},{2,4,5,4,2}};
			//len=2;
			for(int i = 0; i < Size; i++){
				for(int j = 0; j < Size; j++){
					double sum = 0, cnt = 0;
					for(int k = 0; k < filter_sz; k++){
						if(i-len+k < 0 || i-len+k > Size-1)	continue;
						for(int l = 0; l < filter_sz; l++){
							if(j-len+l < 0 || j-len+l > Size-1)	continue;
							sum += int(ImageData[i][j])*Gaussian_filter[k][l];
							cnt += Gaussian_filter[k][l];
						}
					}
					tmp[i][j] = round(sum/cnt);
				}
			}
			memcpy(ImageData, tmp, sizeof(unsigned char)*Size*Size);
			int grad_map[Size][Size] = {{0}};
			double theta[Size][Size] = {{0}};
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					if(i == Size-1 || j == 0)	continue;
					double GR, GC;
					GC = ImageData[i][j] - ImageData[i+1][j];
					GR = ImageData[i][j] - ImageData[i][j-1];
					grad_map[i][j] = sqrt(pow(GC,2)+pow(GR,2));
					if(GR == 0) GR = 1;
					theta[i][j] = atan(GC/GR)*180/M_PI;
				}
			int edge_map[Size][Size] = {{0}};
			for(int i = 1; i < Size-1; i++){
				for(int j = 1; j < Size-1; j++){
					if(theta[i][j] < 22.5 && theta[i][j] > -22.5){
						if(grad_map[i][j] > grad_map[i][j-1] && grad_map[i][j] > grad_map[i][j+1])
							edge_map[i][j] = grad_map[i][j];
					}
					else if(theta[i][j] > 22.5 && theta[i][j] < 67.5){
						if(grad_map[i][j] > grad_map[i-1][j+1] && grad_map[i][j] > grad_map[i+1][j-1])
							edge_map[i][j] = grad_map[i][j];
					}
					else if(theta[i][j] < -22.5 && theta[i][j] > -67.5){
						if(grad_map[i][j] > grad_map[i+1][j+1] && grad_map[i][j] > grad_map[i-1][j-1])
							edge_map[i][j] = grad_map[i][j];
					}
					else{
						if(grad_map[i][j] > grad_map[i-1][j] && grad_map[i][j] > grad_map[i+1][j])
							edge_map[i][j] = grad_map[i][j];
					}
				}
			}
			for(int i = 0; i < Size; i++)
				for(int j = 0; j < Size; j++){
					if(edge_map[i][j] >= TH)							edge_map[i][j] = 2;
					else if(edge_map[i][j] < TH && edge_map[i][j] > TL)	edge_map[i][j] = 1;
					else												edge_map[i][j] = 0;
				}
			memset(ImageData, 0, Size*Size*sizeof(unsigned char));
			for(int i = 1; i < Size-1; i++)
				for(int j = 1; j < Size-1; j++){
					/*
					if(edge_map[i][j] == 2)	ImageData[i][j] = 1;
					else if(edge_map[i][j] == 1){
						for(int k = -1; k < 2; k++)
							for(int l = -1; l < 2; l++){
								if(edge_map[i+k][j+l] == 2)	ImageData[i][j] = 1;
							}
					}*/
					if(edge_map[i][j] == 1)	Connect(i, j);
					if(edge_map[i][j] == 2)	ImageData[i][j] = 1;
				}
		}
		void output(char filename[100]){
			FILE *fout = fopen(filename, "wb");
			if(!fout){
				cout << "can't open file!" << endl;
				exit(1);
			}
			fwrite(ImageData, sizeof(unsigned char), Size*Size, fout);
			fclose(fout);
		}

};

Image::Image(){
	memcpy(ImageData, Original_img, sizeof(unsigned char)*Size*Size);
	memset(edge_map, 0, sizeof(unsigned char)*Size*Size);
}

int main(int argc, char** argv){
	FILE *fin = fopen(argv[1], "rb");
	if (!fin){
		cout << "can't open file!" << endl;
		exit(1);
	}
	fread(Original_img, sizeof(unsigned char), Size*Size, fin);
	fclose(fin);
	
	Image FIRST, SECOND, CANNY;
	FIRST.First_order_2(40);
	FIRST.output(argv[3]);
	SECOND.Second_order(30, 4);
	SECOND.output(argv[4]);
	CANNY.Canny(5, 10, 40);
	CANNY.output(argv[5]);

	fin = fopen(argv[1], "rb");
	if (!fin){
		cout << "can't open file!" << endl;
		exit(1);
	}
	fread(Original_img, sizeof(unsigned char), Size*Size, fin);
	fclose(fin);

	Image DENOISE;
	DENOISE.First_order_2(40);
	DENOISE.output(argv[6]);

	return 0;
}
