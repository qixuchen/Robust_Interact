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
#include "SamplePrune.h"
#include "UtilityApprox/UtilityApprox.h"
#include "Preference_Learning/preferLearn.h"
#include "Active_Ranking/active_ranking.h"
#include "UH/maxUtility.h"
#include "RH/alg_one.h"
#include "exp_stats.h"

#include <vector>
#include <ctime>
#include <cmath>
#include <sys/time.h>

double compute_std(int* round_array, int n){
    double sum = 0, var_sum = 0;
    for(int i = 0; i < n; i++) sum += round_array[i];
    double mean = sum/n;
    for(int i = 0; i < n; i++) var_sum += (round_array[i]-mean)*(round_array[i]-mean);
    return sqrt(var_sum/n);
}


int main(int argc, char *argv[]){

    srand(time(0));  // Initialize random number generator.
    point_set_t *P0 = read_points((char*)"2d100k.txt");
    int dim = P0->points[0]->dim; //obtain the dimension of the point
    std::vector<point_t *> p_set, top, p_convh, skylines;

    // int p_num = P0->numberOfPoints;
    // vector<point_t*> origin;
    // for(int i=0; i < p_num; i++){
    //     origin.push_back(P0->points[i]);
    // }

    skyline_pset(P0, skylines);
    printf("%ld\n", skylines.size());

    skyband(P0, p_set, 1);
    find_top1(p_set, top);
    skyline_c(top, p_convh); //p_convh contains all the top-1 point on the convex hull
    // look for the ground truth top-k point
    point_set_t *P = point_reload(p_set);
    std::vector<point_t *> top_current;
     // generate the utility vector
    point_t *u = alloc_point(dim);

    double theta = 0.05;
    int checknum = 3;
    int num_repeat = 200;


    for(int i=0; i<num_repeat; i++){
        printf("round %d\n", i);
        double sum = 0;
        for (int i = 0; i < dim; i++)
        {
            u->coord[i] = ((double) rand()) / RAND_MAX;
            sum += u->coord[i];
            // if(i == 1){
            //     u->coord[i] = ((double) rand()) / RAND_MAX / 2; 
            // }
            // else{
            //     u->coord[i] = ((double) rand()) / RAND_MAX / 2 + 0.5;
            // }
        }
        for (int i = 0; i < dim; i++){
            u->coord[i] = u->coord[i]/sum;
        }


        find_top_k(u, P0, top_current, 2);
        printf("---------------------------------------------------------\n");
        printf("|%30s |%10s |%10s |\n", "Ground Truth", "--", "--");
        printf("|%30s |%8s%2d |%10d |\n", "Point", "top -", 1, top_current[0]->id);
        printf("---------------------------------------------------------\n");

        best_score = dot_prod(u,top_current[0]);
        double top_2_score = dot_prod(u,top_current[1]);
        double epsilon = (best_score - top_2_score) / best_score;

        int maxRound = 1000;

        // *************************************
        // Beginning of algs

        // if(dim==2){ // 2d algs

            // // the 2RI algorithm
            ST2D(p_convh, u, checknum, theta);

            // the Median algorithm
            // Median_Adapt(p_set, u, maxRound, theta);

            // // the Hull algorithm
            // Hull_Adapt(p_set, u, maxRound, theta);

            // // the 2DPI algorithm
            // for (int i = 0; i < p_set.size(); i++)
            //     p_set[i]->pid = i;
            // twoDPI(p_set,u,theta);
            // for (int i = 0; i < p_set.size(); i++)
            //     p_set[i]->dim = dim;
            // p_set.clear();
            // point_reload(P, p_set);
        // }

        // // // Algorithm HDPI_accurate
        // HDPI_accurate(p_set, u, theta);

        // // Algorithm PointPrune_v2 
        // PointPrune_v2(p_set, u, checknum, theta);

        // // Algorithm SamplePrune 
        // SamplePrune(p_set, u, checknum, theta);

        // Algorithm UtilityApprox
        // utilityapprox(P, u , 2, epsilon, 100, theta);

        // Algorithm Preference_learning
        // Preference_Learning_accuracy(p_set, u, theta);

        // // Algorithm Active_Ranking
        // Active_Ranking(p_set, u, theta);

        // // Algorithm RH
        // Random_half(p_set, u, theta);

        // // the UH-Simplex algorithm
        // int s = 2, cmp_option = SIMPLEX;
        // int prune_option = RTREE, dom_option = HYPER_PLANE, stop_option = EXACT_BOUND;
        // max_utility(P, u, s, epsilon, maxRound, cmp_option, stop_option, prune_option, dom_option, theta);

    }
    release_point_set(P, true);
    std::cout << "accuracy: " << ((double) correct_count)/num_repeat << std::endl;
    std::cout << "avg question num: "<< question_num/num_repeat << std::endl;
    std::cout << "avg return size: "<< return_size/num_repeat << std::endl;
    print_time_stats();
    std::cout << "avg accumulate time: "<< tot_time / num_repeat << std::endl;
    return 0;

}