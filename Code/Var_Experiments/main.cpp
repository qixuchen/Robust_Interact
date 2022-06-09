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

int i1_p1;
int i1_p2;
int i2_p1;
int i2_p2;
int i3_p1;
int i3_p2;

double compute_std(int* round_array, int n){
    double sum = 0, var_sum = 0;
    for(int i = 0; i < n; i++) sum += round_array[i];
    double mean = sum/n;
    for(int i = 0; i < n; i++) var_sum += (round_array[i]-mean)*(round_array[i]-mean);
    return sqrt(var_sum/n);
}


int main(int argc, char *argv[]){

    double twoRI = 0;
    double twoRI_rr = 0;
    int twoRI_count = 0;
    
    double med = 0;
    double med_rr = 0;
    int med_count = 0;

    double Hull = 0;
    double Hull_rr = 0;
    int Hull_count = 0;

    double DPI = 0;
    double DPI_rr = 0;
    int DPI_count = 0;

    double HD_s = 0;
    double HD_s_rr = 0;
    int HD_s_count = 0;

    double HD_a = 0;
    double HD_a_rr = 0;
    int HD_a_count = 0;

    double PP_2 = 0;
    double PP_2_rr = 0;
    int PP_2_count = 0;

    double SP = 0;
    double SP_rr = 0;
    int SP_count = 0;

    double UA = 0;
    double UA_rr = 0;
    int UA_count = 0;
    
    double UH = 0;
    double UH_rr = 0;
    int UH_count = 0;

    double PL = 0;
    double PL_rr = 0;
    int PL_count = 0;

    double AR = 0;
    double AR_rr = 0;
    int AR_count = 0;

    double RH = 0;
    double RH_rr= 0 ;
    int RH_count = 0;

    int output_size=5;

    srand(time(0));  // Initialize random number generator.
    point_set_t *P0 = read_points((char*)"4d1k.txt");
    int dim = P0->points[0]->dim; //obtain the dimension of the point
    std::vector<point_t *> p_set, top, p_convh;
    skyband(P0, p_set, 1);

    // find_top1(p_set, top);
    // skyline_c(top, p_convh); //p_convh contains all the top-1 point on the convex hull
    // // look for the ground truth top-k point
    point_set_t *P = point_reload(p_set);
    std::vector<point_t *> top_current;
     // generate the utility vector
    point_t *u = alloc_point(dim);

    double theta=0.05;
    int checknum=3;
    int point_num = 4;
    float avg_time;

    int num_repeat = 1;


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

        // find_top_k(u, P0, top_current, 2);
        // printf("---------------------------------------------------------\n");
        // printf("|%30s |%10s |%10s |\n", "Ground Truth", "--", "--");
        // printf("|%30s |%8s%2d |%10d |\n", "Point", "top -", 1, top_current[0]->id);
        // printf("---------------------------------------------------------\n");
        top_current.clear();
        find_top_k(u, P0, top_current, output_size);
        printf("---------------------------------------------------------\n");
        printf("|%30s |%10s |%10s |\n", "Ground Truth", "--", "--");
        for(int i = 0; i < top_current.size(); i++){
            printf("|%30s |%8s%2d |%10d |\n", "Point", "top -", i+1, top_current[i]->id);
        }
        printf("---------------------------------------------------------\n");

        std::vector<point_t *> ground_truth = top_current;

        top_1_score = dot_prod(u,top_current[0]);
        double top_2_score = dot_prod(u,top_current[1]);
        double epsilon = (top_1_score - top_2_score) / top_1_score;

        int maxRound = 1000;


        // //Algorithm HDPI_accurate
        // HD_a += HDPI_accurate(p_set, u, theta);
        // HD_a_rr += rr_ratio;
        // HD_a_count += top_1_found;


        // //Algorithm PointPrune_v2 
        // PP_2 += PointPrune_v2(p_set, u, checknum, theta);
        // PP_2_rr += rr_ratio;
        // PP_2_count += top_1_found;

        // //Algorithm PointPrune_v2 
        PP_2 += PointPrune_morethan2points(p_set, u, checknum, theta, point_num);
        PP_2_rr += rr_ratio;
        PP_2_count += top_1_found;

    
        // Algorithm SamplePrune 
        // SP +=   SamplePrune_alltopk(p_set, u, checknum, theta, output_size, ground_truth);
        // SP_rr += rr_ratio;
        // SP_count += top_k_correct;

        // // the UH-Simplex algorithm
        // int s = 2, cmp_option = SIMPLEX;
        // int prune_option = RTREE, dom_option = HYPER_PLANE, stop_option = EXACT_BOUND;
        // UH += max_utility(P, u, s, epsilon, maxRound, cmp_option, stop_option, prune_option, dom_option, theta);
        // UH_rr += rr_ratio;
        // UH_count += top_1_found;

        // // Algorithm UtilityApprox
        // UA += utilityapprox(P, u , 2, epsilon, 100, theta);
        // UA_rr += rr_ratio;
        // UA_count += top_1_found;

        // // Algorithm Preference_learning
        // PL += Preference_Learning_accuracy(p_set, u, theta);
        // PL_rr += rr_ratio;
        // PL_count += top_1_found;

        // // Algorithm Active_Ranking
        // AR += Active_Ranking(p_set, u, theta);
        // AR_rr += rr_ratio;
        // AR_count += top_1_found;

        // // Algorithm RH
        // RH += Random_half(p_set, u, theta);
        // RH_rr += rr_ratio;
        // RH_count += top_1_found;


    }

    
    printf("\nIn total\n");
    printf("|%20s |%10s |%8s |%10s\n", "Alg", "# rounds", "rr", "found top1");
    printf("|%20s |%10f |%8f |%8f\n", "2RI", twoRI/num_repeat, twoRI_rr/num_repeat, ((double)twoRI_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "2DPI", DPI/num_repeat, DPI_rr/num_repeat, ((double)DPI_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "Median", med/num_repeat, med_rr/num_repeat, ((double)med_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "Hull", Hull/num_repeat, Hull_rr/num_repeat, ((double)Hull_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "HDPI-Accurate", HD_a/num_repeat, HD_a_rr/num_repeat, ((double)HD_a_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "PointPrune_v2", PP_2/num_repeat, PP_2_rr/num_repeat, ((double)PP_2_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "SamplePrune", SP/num_repeat, SP_rr/num_repeat, ((double)SP_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "UtilityApprox", UA/num_repeat, UA_rr/num_repeat, ((double)UA_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "Preference_Learning", PL/num_repeat, PL_rr/num_repeat, ((double)PL_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "Active_Ranking", AR/num_repeat, AR_rr/num_repeat, ((double)AR_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "UH-SIMPLEX", UH/num_repeat, UH_rr/num_repeat, ((double)UH_count)/num_repeat);
    printf("|%20s |%10f |%8f |%8f\n", "RH", RH/num_repeat, RH_rr/num_repeat, ((double)RH_count)/num_repeat);
    
    release_point_set(P, true);
    return 0;

}

// std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

// std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

// avg_time = ((float) std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count())/PP_2;
//    printf("%f\n", avg_time);
