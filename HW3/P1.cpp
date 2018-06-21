#include <bits/stdc++.h>
#include "masks.h"
#define Size 256

unsigned char Original_img[Size][Size];

class Image{
	private:
		unsigned char ImageData[Size][Size];
		//int mask[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
		int mask[5][5] = {{0,0,1,0,0},{0,1,1,1,0},{1,1,1,1,1},{0,1,1,1,0},{0,0,1,0,0}};
		int obj_id[Size][Size] = {{0}};
	public:
		Image();
		void boundary(){
			unsigned char inside[Size][Size] = {{0}};
			for( int i = 2; i < Size-2; i++ )
				for( int j = 2; j < Size-2; j++ )
					if ( match(i, j) )	inside[i][j] = 1;
			for( int i = 2; i < Size-2; i++ )
				for( int j = 2; j < Size-2; j++ )
					if ( inside[i][j] )	ImageData[i][j] = 0;
		}
		int match( int x, int y ){
			for( int i = 0; i < 5; i++ )
				for( int j = 0; j < 5; j++ )
					if ( mask[i][j] == 1 && ImageData[x+i-2][y+j-2] == 0 )
						return 0;
			return 1;
		}
		void count_obj(){
			int id = 0;
			for( int i = 1; i < Size-1; i++ )
				for( int j = 1; j < Size-1; j++ ) {
					if ( ImageData[i][j] != 0 && obj_id[i][j] == 0 ) {
						id++;
						traverse(i, j, id);
					}
				}
			//printf("%d\n", id);

		}
		void traverse( int x, int y, int id){
			if ( x < 0 || x >= Size || y < 0 || y >= Size )	return;
			if ( ImageData[x][y] == 0 )	return;
			if ( obj_id[x][y] != 0 )	return;
			obj_id[x][y] = id;
			traverse(x-1,y-1, id);
			traverse(x-1,y, id);
			traverse(x-1,y+1, id);
			traverse(x,y-1, id);
		    traverse(x,y+1, id);
			traverse(x+1,y-1, id);
			traverse(x+1,y, id);
			traverse(x+1,y+1, id);
		}
		void skeletonize(){
			while ( true ) {
				int M_match = 0;
				int T_match = 0;

				unsigned char M[Size][Size] = {{0}};
				for ( int i = 1; i < Size-1; i++ )
					for ( int j = 1; j < Size-1; j++ ) {
						int flag = 0;
						for ( int m = 0; m < sizeof(sk_mask_1)/sizeof(sk_mask_1[0]); m++) {
							if ( flag )	break;
							int false_cnt = 0;
							for ( int k = 0; k < 3; k++ )
								for ( int l = 0; l < 3; l++ )
									if ( ImageData[i+k-1][j+l-1] != sk_mask_1[m][k][l]*255 )
										false_cnt++;
							if ( !false_cnt ) {
								flag = 1;
								M_match++;
								M[i][j] = 255;
							}
						}
					}
				unsigned char T[Size][Size] = {{0}};
				int a = 0;
				for ( int i = 1; i < Size-1; i++ )
					for ( int j = 1; j < Size-1; j++ )
						if (ImageData[i][j] == 255) a++;
				for ( int i = 1; i < Size-1; i++ )
					for ( int j = 1; j < Size-1; j++ ) {
						int flag = 0;
						for ( int m = 0; m < sizeof(sk_mask_2)/sizeof(sk_mask_2[0]); m++) {
							if ( flag )	break;
							int false_cnt = 0;
							for ( int k = 0; k < 3; k++ )
								for ( int l = 0; l < 3; l++ )
									if (  M[i+k-1][j+l-1] != sk_mask_2[m][k][l]*255 )
										false_cnt++;
							if ( false_cnt && M[i][j] ) {
								flag = 1;
								T_match++;
								T[i][j] = 255;
							}
						}
					}
				//printf("%d %d %d\n", a, M_match, T_match);
				for ( int i = 1; i < Size-1; i++ )
					for ( int j = 1; j < Size-1; j++ )
						if ( T[i][j] == 255 ) 
							ImageData[i][j] = 0;
				if ( T_match == 0 )	break;
				//memcpy(ImageData, T, sizeof(unsigned char)*Size*Size);
			}
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

	Image B, S;
	B.boundary();
	B.output(argv[2]);
	//I1.count_obj();
	S.skeletonize();
	S.output(argv[3]);
	


	return 0;
}
