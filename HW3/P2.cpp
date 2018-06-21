#include <bits/stdc++.h>
#include <math.h>
#include "masks.h"
#define Size 512

unsigned char Original_img[Size][Size];
double M[Size][Size][9] = {{{0}}};
double T[Size][Size][9] = {{{0}}};

class Image{
	private:
		unsigned char ImageData[Size][Size];
		double centroid[3][9] = {{0}};
		int cluster[Size][Size] = {{0}};
	public:
		Image();
		void Law(int winsize) {
			for ( int m = 0; m < 9; m++ )
				for ( int i = winsize; i < Size-winsize; i++ )
					for ( int j = winsize; j < Size-winsize; j++ )
						M[i][j][m] = conv(i, j, LAWS_mask[m]);
			for ( int m = 0; m < 9; m++ )
				for ( int i = winsize; i < Size-winsize; i++ )
					for ( int j = winsize; j < Size-winsize; j++ )
						for ( int k = 0; k < winsize; k++ )
							for ( int l = 0; l < winsize; l++ )
								T[i][j][m] += pow(M[i+k-int(winsize/2)][j+l-int(winsize/2)][m], 2);

		}
		double conv( int x, int y, double mask[3][3] ) {
			double sum = 0;
			for ( int i = 0; i < 3; i++ )
				for ( int j = 0; j < 3; j++ )
					sum += ImageData[x+i-1][y+j-1] * mask[i][j];
			return sum;
		}
		void init_centroid() {
			std::copy( T[20][20], T[20][20]+9, centroid[0] );
			std::copy( T[20][100], T[20][100]+9, centroid[1] );
			std::copy( T[400][400], T[400][400]+9, centroid[2] );
		}
		int nearest(int x, int y) {
			int n = -1;
			double min = 1e20;
			for ( int i = 0; i < 3; i++ ) {
				double d = 0;
				for ( int j = 0; j < 9; j++ )
					d += pow(T[x][y][j] - centroid[i][j], 2);
				if ( d < min ) {
					min = d;
					n = i;
				}
			}
			return n;
		}
		void kmeans() {
			init_centroid();
			int last_cluster[Size][Size] = {{0}};
			int iter = 0;
			while (true){
				for ( int i = 1; i < Size-1; i++ )
					for ( int j = 1; j < Size-1; j++ )
						cluster[i][j] = nearest(i, j);
				int cnt[3] = {0};
				memset(centroid, 0, sizeof(int)*3*9);
				for ( int i = 0; i < Size; i++ ) {
					for ( int j = 0; j < Size; j++ ) {
						cnt[cluster[i][j]]++;
						for ( int k = 0; k < 9; k++ )
							centroid[cluster[i][j]][k] += T[i][j][k];
					}
				}
				for ( int i = 0; i < 3; i++ )
					for ( int j = 0; j < 9; j++ )
						centroid[i][j] /= cnt[i];
			   	//for ( int i = 0; i < 3; i++ )
				//	printf("%d ", cnt[i]);
				//puts("");

				int diff = 0;
				for ( int i = 0; i < Size; i++ ) {
					if ( diff ) break;
					for ( int j = 0; j < Size; j++ )
						if ( cluster[i][j] != last_cluster[i][j] ) {
							diff++;
							break;
						}
				}
				if ( !diff ) break;
				memcpy(last_cluster, cluster, sizeof(int)*Size*Size);
				iter++;
			}
			//printf("%d\n", iter);
			for ( int i = 0; i < Size; i++ )
				for ( int j = 0; j < Size; j++ )
					ImageData[i][j] = cluster[i][j] * 127;
		}
		void swap(){
			int s[3] = {40, 80, 10};
			double T0[40][40];
			double T1[80][80];
			double T2[10][10];
			for ( int i = 0; i < s[2]; i++ )
				for ( int j = 0; j < s[2]; j++ )
					T2[i][j] = Original_img[i][j];
			for ( int i = 0; i < s[0]; i++ )
				for ( int j = 0; j < s[0]; j++ )
					T0[i][j] = Original_img[420+i][20+j];
			for ( int i = 0; i < s[1]; i++ )
				for ( int j = 0; j < s[1]; j++ )
					T1[i][j] = Original_img[400+i][400+j];
			unsigned char tmp[Size][Size];
			for ( int i = 0; i < Size; i+=s[1]/2 )
				for ( int j = 0; j < Size; j+=s[1]/2 )
					if ( cluster[i][j] == 0 )
						for ( int k = 0; k < s[1]; k++ )
							for ( int l = 0; l < s[1]; l++ )
								if ( i+k < Size && j+l < Size )
									tmp[i+k][j+l] = T1[k][l];
			for ( int i = 0; i < Size; i+=s[2]/2 )
				for ( int j = 0; j < Size; j+=s[2]/2 )
					if ( cluster[i][j] == 0 )
						for ( int k = 0; k < s[2]; k++ )
							for ( int l = 0; l < s[2]; l++ )
								if ( i+k < Size && j+l < Size )
									tmp[i+k][j+l] = T2[k][l];
			for ( int i = 0; i < Size; i+=s[0]/2 )
				for ( int j = 0; j < Size; j+=s[0]/2 )
					if ( cluster[i][j] == 0 )
						for ( int k = 0; k < s[0]; k++ )
							for ( int l = 0; l < s[0]; l++ )
								if ( i+k < Size && j+l < Size )
									tmp[i+k][j+l] = T0[k][l];
			memcpy(ImageData, tmp, sizeof(unsigned char)*Size*Size);

		}
		void output( char filename[128] ) {
			FILE *fout = fopen( filename, "wb" );
			if ( !fout ) {
				printf("can't open file!\n");
				exit(1);
			}
			fwrite(ImageData, sizeof(unsigned char), Size*Size, fout);
			fclose(fout);
		}


};

Image::Image(){
	memcpy(ImageData, Original_img, sizeof(unsigned char)*Size*Size);
}

int main(int argc, char** argv) {
	FILE *fin = fopen(argv[1], "rb");
	if ( !fin ) {
		printf("can't open file!\n");
		exit(1);
	}
	fread(Original_img, sizeof(unsigned char), Size*Size, fin);
	fclose(fin);

	Image K;
	K.Law(15);
	K.kmeans();
	K.output(argv[2]);
	K.swap();
	K.output(argv[3]);
	


	return 0;
}
