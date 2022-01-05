#include "Others/data_utility.h"
#include "Others/data_struct.h"
#include "Others/read_write.h"
#include "Others/pruning.h"
#include "Others/qhull_build.h"
#include "Others/used_function.h"
#include "ST2D/ST2D.h"
#include "2DPI/2dPI.h"
#include "Median_Hull/medianhull.h"
#include "HDPI/PointPrune.h"
#include "HDPI/SamplePrune.h"
#include "HDPI/STMD.h"
#include <vector>
#include <ctime>
#include <sys/time.h>

int num_wrong_answer=0;
int crit_wrong_answer=0;
int num_questions=0;
double top_1_score = 0;
double rr_ratio=0;
int top_1_found=0;

int main(int argc, char *argv[]){

    double theta=0.05;
    int checknum=3;
    int num_repeat = 100;

    double twoRI=0;
    double twoRI_rr=0;
    int twoRI_count = 0;
    
    double med=0;
    double med_rr=0;
    int med_count = 0;

    double DPI=0;
    double DPI_rr=0;
    int DPI_count = 0;

    double PP_2=0;
    double PP_2_rr=0;
    int PP_2_count = 0;

    double SP=0;
    double SP_rr=0;
    int SP_count = 0;

    double ST=0;
    double ST_rr=0;
    int ST_count = 0;


    // std::vector<point_t *> p_set;
    // skyband(P0, p_set, k);
    // point_set_t *P = point_reload(p_set);

    srand(time(0));  // Initialize random number generator.
    point_set_t *P0 = read_points((char*)"2d5K.txt");
    int dim = P0->points[0]->dim; //obtain the dimension of the point
    std::vector<point_t *> p_set, top, p_convh;
    skyband(P0, p_set, 1);
    find_top1(p_set, top);
    skyline_c(top, p_convh); //p_convh contains all the top-1 point on the convex hull
    // look for the ground truth top-k point
    point_set_t *P = point_reload(p_set);
    std::vector<point_t *> top_current;
     // generate the utility vector
    point_t *u = alloc_point(dim);

    for(int i=0; i<num_repeat; i++){
        double sum = 0;
        for (int i = 0; i < dim; i++)
        {
            u->coord[i] = ((double) rand()) / RAND_MAX;
            sum += u->coord[i];
        }
        for (int i = 0; i < dim; i++){
            u->coord[i] = u->coord[i]/sum;
        }


        find_top_k(u, P0, top_current, 1);
        printf("---------------------------------------------------------\n");
        printf("|%30s |%10s |%10s |\n", "Ground Truth", "--", "--");
        printf("|%30s |%8s%2d |%10d |\n", "Point", "top -", 1, top_current[0]->id);
        printf("---------------------------------------------------------\n");

        top_1_score = dot_prod(u,top_current[0]);
        // double top_2_score = dot_prod(u,top_current[1]);
        // double epsilon = (top_1_score - top_2_score) / top_1_score;

        // the 2RI algorithm
        twoRI += ST2D(p_convh, u, checknum, theta);
        twoRI_rr += rr_ratio;
        twoRI_count += top_1_found;
            

        // // the Median algorithm
        // int maxRound = 1000;
        // med += Median_Adapt(p_set, u, maxRound, 1, theta);
        // med_rr += rr_ratio;
        // med_count += top_1_found;

        // // the 2DPI algorithm
        // for (int i = 0; i < p_set.size(); i++)
        //     p_set[i]->pid = i;
        // DPI += twoDPI(p_set,u,theta);
        // DPI_rr += rr_ratio;
        // DPI_count += top_1_found;
        // for (int i = 0; i < p_set.size(); i++)
        //     p_set[i]->dim = dim;
        // p_set.clear();
        // point_reload(P, p_set);

        //Algorithm PointPrune_v2 (0.4, 20, 0.9)
        PP_2 += PointPrune_v2(p_set, u, checknum, theta);
        PP_2_rr += rr_ratio;
        PP_2_count += top_1_found;

        //Algorithm SamplePrune (0.4, 21.2 ,0.93)
        SP += SamplePrune(p_set, u, checknum, theta);
        SP_rr += rr_ratio;
        SP_count += top_1_found;

        // Algorithm HRI (2, 17.2, 0.89)
        ST += STMD(p_set, u, theta);
        ST_rr += rr_ratio;
        ST_count += top_1_found;



    }



    
    printf("\nIn total\n");
    printf("|%20s |%10s |%8s |%10s\n", "Alg", "# rounds", "rr", "found top1");
    printf("|%20s |%10f |%8f |%8f\n", "2RI", twoRI/num_repeat, twoRI_rr/num_repeat, ((double)twoRI_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "Median", med/num_repeat, med_rr/num_repeat, ((double)med_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "2DPI", DPI/num_repeat, DPI_rr/num_repeat, ((double)DPI_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "PointPrune_v2", PP_2/num_repeat, PP_2_rr/num_repeat, ((double)PP_2_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "SamplePrune", SP/num_repeat, SP_rr/num_repeat, ((double)SP_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "STMD", ST/num_repeat, ST_rr/num_repeat, ((double)ST_count)/num_repeat);
    release_point_set(P, true);
    return 0;

    // printf("# of incorrect answer : %d\n", num_wrong_answer);
    // printf("rr: %f\n", rr_ratio);

}