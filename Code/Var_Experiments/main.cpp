#include "Others/data_utility.h"
#include "Others/data_struct.h"
#include "Others/read_write.h"
#include "Others/pruning.h"
#include "Others/qhull_build.h"
#include "Others/used_function.h"
#include "ST2D/ST2D.h"
#include "2DPI/2dPI.h"
#include "Median_Hull/medianhull.h"
#include "HDPI/HDPI.h"
#include "PointPrune.h"
#include "PointPrune_qtype.h"
#include "SamplePrune_qtype.h"
#include "SamplePrune.h"
#include "UtilityApprox/UtilityApprox.h"
#include "Preference_Learning/preferLearn.h"
#include "Active_Ranking/active_ranking.h"
#include "UH/maxUtility.h"
#include "RH/alg_one.h"

#include <vector>
#include <ctime>
#include <cmath>
#include <sys/time.h>

int num_wrong_answer=0;
int crit_wrong_answer=0;
int num_questions=0;
double top_1_score = 0;
double rr_ratio=0;
int top_1_found=0;
int top_k_correct=0;

int main(int argc, char *argv[]){

    int output_size = 5;

    srand(time(0));  // Initialize random number generator.
    point_set_t *P0 = read_points((char*)"4d1k.txt");
    int dim = P0->points[0]->dim; //obtain the dimension of the point
    std::vector<point_t *> p_set, top, p_convh;

    // IMPORTANT: change output_size when finding topk
    skyband(P0, p_set, output_size);

    // find_top1(p_set, top);
    // skyline_c(top, p_convh); //p_convh contains all the top-1 point on the convex hull
    // // look for the ground truth top-k point
    point_set_t *P = point_reload(p_set);
    std::vector<point_t *> top_current;
     // generate the utility vector
    point_t *u = alloc_point(dim);

    double theta = 0.05;
    int checknum = 3;
    int point_num = 4;
    float avg_time;

    int num_repeat = 1;

    timeval start, end;

    for(int i=0; i<num_repeat; i++){
        printf("Round # %d\n", i+1);
        double sum = 0;
        for (int i = 0; i < dim; i++)
        {
            u->coord[i] = ((double) rand()) / RAND_MAX;
            sum += u->coord[i];
        }
        for (int i = 0; i < dim; i++){
            u->coord[i] = u->coord[i]/sum;
        }

        // find the ground truth
        top_current.clear();
        find_top_k(u, P0, top_current, output_size);
        printf("---------------------------------------------------------\n");
        printf("|%30s |%10s |%10s |\n", "Ground Truth", "--", "--");
        for(int i = 0; i < top_current.size(); i++){
            printf("|%30s |%8s%2d |%10d |\n", "Point", "top -", i+1, top_current[i]->id);
        }
        printf("---------------------------------------------------------\n");

        top_1_score = dot_prod(u,top_current[0]);


        PointPrune_containTop1(p_set, u, checknum, theta, output_size);

        SamplePrune_containTop1(p_set, u, checknum, theta, output_size);

        PointPrune_morethan2points(p_set, u, checknum, theta, point_num);

        SamplePrune_morethan2points(p_set, u, checknum, theta, point_num);

        PointPrune_desired_undesired(p_set, u, checknum, theta, point_num);

        SamplePrune_desired_undesired(p_set, u, checknum, theta, point_num);

        PointPrune_Alltopk(p_set, u, checknum, theta, output_size, top_current);

        SamplePrune_alltopk(p_set, u, checknum, theta, output_size, top_current);


    }

    
    release_point_set(P, true);
    return 0;

}
