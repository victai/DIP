#include <bits/stdc++.h>
#define train_height 248
#define train_width 450
#define test_height 125
#define test_width 390
#define max_obj 100

unsigned char TrainingSet[train_height][train_width];
int train_obj_cnt[max_obj] = {0};
int train_obj_id[train_height][train_width] = {{0}};
int train_upper[max_obj] = {0};
int train_lower[max_obj] = {0};
int train_left[max_obj] = {0};
int train_right[max_obj] = {0};
int train_obj_num = 0;

unsigned char TestingSet[test_height][test_width];
int test_obj_cnt[max_obj] = {0};
int test_obj_id[test_height][test_width] = {{0}};
int test_upper[max_obj] = {0};
int test_lower[max_obj] = {0};
int test_left[max_obj] = {0};
int test_right[max_obj] = {0};
int test_obj_num = 0;

char ans[max_obj] = " ABCDEFGHIJKLMNbOQSPRTUVWXYZadfhklcegijmnop0123tqrsuvwxyz4689!#&57$%^@*";

bool sort_left_to_right (int i, int j) {
    return test_left[i] < test_left[j];
}

void train_traverse( int x, int y, int id){
    if ( x < 0 || x >= train_height || y < 0 || y >= train_width )	return;
    if ( TrainingSet[x][y] != 0 )	return;
    if ( train_obj_id[x][y] == id )	return;
    train_obj_id[x][y] = id;
    train_obj_cnt[id]++;
    train_traverse(x-1,y-1, id);
    train_traverse(x-1,y, id);
    train_traverse(x-1,y+1, id);
    train_traverse(x,y-1, id);
    train_traverse(x,y+1, id);
    train_traverse(x+1,y-1, id);
    train_traverse(x+1,y, id);
    train_traverse(x+1,y+1, id);
}

void train_find_obj(){
    // labelling connected components
    int id = 0;
    int cnt = 0;
    for( int i = 0; i < train_height; i++ )
        for( int j = 0; j < train_width; j++ ) {
            if ( TrainingSet[i][j] == 0 ) cnt++;
            if ( TrainingSet[i][j] == 0 && train_obj_id[i][j] == 0 ) {
                id++;
                train_traverse(i, j, id);
            }
        }
    // combining nearby objects
    int winsize = 20;
    for ( int i = winsize; i < train_height-winsize; i++ )
        for ( int j = winsize; j < train_width-winsize; j++ ){
            if ( train_obj_cnt[train_obj_id[i][j]] < 50 && train_obj_cnt[train_obj_id[i][j]] > 0) {
                int neighbors[max_obj] = {0};
                for ( int k = -winsize; k < winsize; k++ ) 
                    for ( int l = -winsize; l < winsize; l++ )
                        if ( train_obj_id[i+k][j+l] != train_obj_id[i][j] )
                            neighbors[train_obj_id[i+k][j+l]]++;
                int max_id = std::distance( neighbors, std::max_element( neighbors+1, neighbors+max_obj ));
                int old_id = train_obj_id[i][j];
                train_traverse(i, j, max_id);
                train_obj_cnt[old_id] = 0;
            }
        }
    // filling empty ids.
    id = 1;
    for ( int k = 1; k < max_obj; k++ ) {
        int flag = 0;
        for ( int i = 0; i < train_height; i++ )
            for ( int j = 0; j < train_width; j++ ) {
                if ( train_obj_id[i][j] == k && k > id ) {
                    train_obj_cnt[k] = 0;
                    train_traverse(i, j, id);
                }
            }
        if (train_obj_cnt[id] != 0)   id++;
    }
    train_obj_num = id;
    
    /*
    int end = 450;
    for ( int i = 180; i < 230; i++)
        for(int j = end-50; j < end; j++)
            printf("%3d%s", train_obj_id[i][j], (j==end-1)?"\n":"");
    for ( int i = 0; i < max_obj; i++ )
        printf("%3d: %3d\n", i, train_obj_cnt[i]);
    */
    
    // getting boundaries
    memset(train_upper, 0b00000001, sizeof(int)*max_obj);
    memset(train_left, 0b00000001, sizeof(int)*max_obj);
    for ( int i = 0; i < train_height; i++ )
        for ( int j = 0; j < train_width; j++ )
            if ( train_obj_cnt[train_obj_id[i][j]] != 0 ){
                train_upper[train_obj_id[i][j]] = std::min( train_upper[train_obj_id[i][j]], i );
                train_lower[train_obj_id[i][j]] = std::max( train_lower[train_obj_id[i][j]], i );
                train_left[train_obj_id[i][j]] = std::min( train_left[train_obj_id[i][j]], j );
                train_right[train_obj_id[i][j]] = std::max( train_right[train_obj_id[i][j]], j );
            }

    train_left[37] += 4; // deal with "i"
    for ( int i = train_upper[57]; i <= train_lower[57]; i++ )        // connect "4"
        for ( int j = train_left[57]+9; j <= train_left[57]+12; j++ ) {
            train_obj_id[i][j] = 57;
            TrainingSet[i][j] = 0;
        }
}

void train_clip(){
    for (int i = 0; i < train_height; i++)
        for ( int j = 0; j < train_width; j++)
            if (TrainingSet[i][j] > 128)    TrainingSet[i][j] = 255;
            else TrainingSet[i][j] = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_traverse( int x, int y, int id){
    if ( x < 0 || x >= test_height || y < 0 || y >= test_width )	return;
    if ( TestingSet[x][y] != 0 )	return;
    if ( test_obj_id[x][y] == id )	return;
    test_obj_id[x][y] = id;
    test_obj_cnt[id]++;
    test_traverse(x-1,y-1, id);
    test_traverse(x-1,y, id);
    test_traverse(x-1,y+1, id);
    test_traverse(x,y-1, id);
    test_traverse(x,y+1, id);
    test_traverse(x+1,y-1, id);
    test_traverse(x+1,y, id);
    test_traverse(x+1,y+1, id);
}

void test_find_obj(){
    // labelling connected components
    int id = 0;
    int cnt = 0;
    for( int i = 0; i < test_height; i++ )
        for( int j = 0; j < test_width; j++ ) {
            if ( TestingSet[i][j] == 0 ) cnt++;
            if ( TestingSet[i][j] == 0 && test_obj_id[i][j] == 0 ) {
                id++;
                test_traverse(i, j, id);
                if ( test_obj_cnt[id] < 10 ) {
                    test_obj_cnt[id] = 0;
                    test_traverse(i, j, 0);
                }
            }
        }
    // combining nearby objects
    int winsize = 10;
    for ( int i = winsize; i < test_height-winsize; i++ )
        for ( int j = winsize; j < test_width-winsize; j++ ){
            if ( test_obj_cnt[test_obj_id[i][j]] > 0) {
                int neighbors[max_obj] = {0};
                int has_neighbor = 0;
                for ( int k = -winsize; k < winsize; k++ ) 
                    for ( int l = -winsize; l < winsize; l++ )
                        if ( test_obj_id[i+k][j+l] != test_obj_id[i][j] && test_obj_id[i+k][j+l] != 0){
                            neighbors[test_obj_id[i+k][j+l]]++;
                            has_neighbor = 1;
                        }
                int max_id = std::distance( neighbors, std::max_element( neighbors+1, neighbors+max_obj ));
                int old_id = test_obj_id[i][j];
                if (has_neighbor){
                    test_traverse(i, j, max_id);
                    test_obj_cnt[old_id] = 0;
                }
            }
        }
    // filling empty ids.
    id = 1;
    for ( int k = 1; k < max_obj; k++ ) {
        int flag = 0;
        for ( int i = 0; i < test_height; i++ )
            for ( int j = 1; j < test_width; j++ ) {
                if ( test_obj_id[i][j] == k && k > id ) {
                    test_obj_cnt[k] = 0;
                    test_traverse(i, j, id);
                }
            }
        if (test_obj_cnt[id] != 0)   id++;
    }
    test_obj_num = id;
    
    /*
    printf("%d\n", id); 
    int end = 300;
    for ( int i = 0; i < 100; i++)
        for(int j = 250; j < end; j++)
            printf("%3d%s", test_obj_id[i][j], (j==end-1)?"\n":"");
    
    for ( int i = 0; i < max_obj; i++ )
        printf("%3d: %3d\n", i, test_obj_cnt[i]);
    */

    // getting boundaries
    memset(test_upper, 0b00000001, sizeof(int)*max_obj);
    memset(test_left, 0b00000001, sizeof(int)*max_obj);
    for ( int i = 0; i < test_height; i++ )
        for ( int j = 0; j < test_width; j++ )
            if ( test_obj_cnt[test_obj_id[i][j]] != 0 ){
                test_upper[test_obj_id[i][j]] = std::min( test_upper[test_obj_id[i][j]], i );
                test_lower[test_obj_id[i][j]] = std::max( test_lower[test_obj_id[i][j]], i );
                test_left[test_obj_id[i][j]] = std::min( test_left[test_obj_id[i][j]], j );
                test_right[test_obj_id[i][j]] = std::max( test_right[test_obj_id[i][j]], j );
            }
}

void test_clip(){
    for (int i = 0; i < test_height; i++)
        for ( int j = 0; j < test_width; j++)
            if (TestingSet[i][j] > 128)    TestingSet[i][j] = 255;
            else TestingSet[i][j] = 0;
}

void identify_obj() {
    std::vector<int> test_left_to_right_id;
    for ( int i = 1; i < test_obj_num; i++ )
        test_left_to_right_id.push_back(i);
    std::sort(test_left_to_right_id.begin(), test_left_to_right_id.end(), sort_left_to_right);
    
    for ( int i = 1; i < test_obj_num; i++ ) {
        int id = test_left_to_right_id[i-1];
        double test_h = test_lower[id] - test_upper[id];
        double test_w = test_right[id] - test_left[id];
        /*
        for ( int a = test_upper[id]; a <= test_lower[id]; a++ ) 
            for ( int b = test_left[id]; b <= test_right[id]; b++ ) 
                printf("%3d%s", TestingSet[a][b], (b==test_right[id])?"\n":" ");
        */
        int best_match = 0;
        int best_true_positive = 0;
        int best_true_negative = 0;
        int best_false_positive = 1000;
        int best_false_negative = 1000;
        for ( int j = 1; j < train_obj_num; j++ ) {
            double train_h = train_lower[j] - train_upper[j];
            double train_w = train_right[j] - train_left[j];
            if ( fabs(test_w / test_h - train_w / train_h) > 0.2 )   continue;  // filtering by width-height ratio
            int true_positive = 0;
            int true_negative = 0;
            int false_positive = 0;
            int false_negative = 0;
            for ( int x = train_upper[j]; x <= train_lower[j]; x++ ) {
                for ( int y = train_left[j]; y <= train_right[j]; y++ ) {
                    int relative_x = test_upper[id] + (int)round((double)(x - train_upper[j]) / train_h * test_h);
                    int relative_y = test_left[id] + (int)round((double)(y - train_left[j]) / train_w * test_w);
                    if ( TrainingSet[x][y] == 0 && (TestingSet[relative_x][relative_y] == 0) )
                        true_positive++;
                    else if ( TrainingSet[x][y] == 255 && (TestingSet[relative_x][relative_y] == 255) )
                        true_negative++;
                    else if ( TrainingSet[x][y] == 0 && (TestingSet[relative_x][relative_y] == 255) )
                        false_positive++;
                    else if ( TrainingSet[x][y] == 255 && (TestingSet[relative_x][relative_y] == 0) )
                        false_negative++;
                }
            }
            if ( true_positive + 0.3*true_negative - 0.5*false_positive - false_negative > \
                    best_true_positive + 0.3*best_true_negative - 0.5*best_false_positive - best_false_negative ) {
                best_match = j;
                best_true_positive = true_positive;
                best_true_negative = true_negative;
                best_false_positive = false_positive;
                best_false_negative = false_negative;
            }
            //printf("===================\n%c:\ntrue_positive: %d\ntrue_negative: %d\nfalse_positive: %d\nfalse_negative: %d\n", ans[j], true_positive, true_negative, false_positive, false_negative);
        }
        printf("%c\n", ans[best_match]);
        printf("===========================================\n");
    }
}

void median_filter(int filter_sz) {
    int step = (filter_sz-1)/2;
    for ( int i = step; i < test_height-step; i++ )
        for ( int j = step; j < test_width-step; j++ ) {
            std::vector<int> vec;
            for ( int k = i-step; k <= i+step; k++ )
                for ( int l = j-step; l <= j+step; l++ )
                    vec.push_back(TestingSet[k][l]);
            sort(vec.begin(), vec.end());
            TestingSet[i][j] = vec[round(vec.size()/2)];
        }
}

int main(int argc, char* argv[]){
    FILE *fp = fopen(argv[1], "r");
    if (!fp){
        printf("cannot open file !\n");
        exit(1);
    }
    fread(TrainingSet, sizeof(unsigned char), train_height*train_width, fp);
    fclose(fp);
    fp = fopen(argv[2], "r");
    if (!fp){
        printf("cannot open file !\n");
        exit(1);
    }
    fread(TestingSet, sizeof(unsigned char), test_height*test_width, fp);
    fclose(fp);

    train_clip();
    train_find_obj();

    median_filter(3); // deal with salt and pepper
    test_clip();
    test_find_obj();
    identify_obj();
    

    return 0;
}
