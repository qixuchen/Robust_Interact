#include "Others/data_utility.h"
#include "Others/data_struct.h"
#include "Others/read_write.h"
#include "Others/pruning.h"
#include "HDPI/HDPI.h"
#include "HDPI/PointPrune.h"
#include "HDPI/SamplePrune.h"
#include "Others/qhull_build.h"
#include <vector>
#include <ctime>
#include <sys/time.h>

int num_wrong_answer=0;
int crit_wrong_answer=0;
int num_questions=0;
double top_1_score = 0;
double rr_ratio=0;
int top_1_found=0;


int main(int argc, char *argv[])
{
    double HD_s=0;
    double HD_s_rr=0;
    int HD_s_count = 0;

    double HD_a=0;
    double HD_a_rr=0;
    int HD_a_count = 0;

    double PP=0;
    double PP_rr=0;
    int PP_count = 0;

    double PP_2=0;
    double PP_2_rr=0;
    int PP_2_count = 0;

    double SP=0;
    double SP_rr=0;
    int SP_count = 0;

    point_set_t *P0 = read_points((char*)"4d100k.txt");
    int dim = P0->points[0]->dim; //obtain the dimension of the point
    int k =1;
    int checknum=3;
    double theta=0.05;
    std::vector<point_t *> p_set, p0;
    skyband(P0, p_set, k);
    point_set_t *P = point_reload(p_set);

    for(int i=0; i<10; i++){
        // generate the utility vector
        srand(time(0));  // Initialize random number generator.
        point_t *u = alloc_point(dim);
        double sum = 0;
        for (int i = 0; i < dim; i++)
        {
            u->coord[i] = ((double) rand()) / RAND_MAX;
            sum += u->coord[i];
        }
        for (int i = 0; i < dim; i++)
            u->coord[i] = u->coord[i]/sum;


        // look for the ground truth top-k point
        std::vector<point_t *> top_current;
        find_top_k(u, P, top_current, k);
        printf("---------------------------------------------------------\n");
        printf("|%30s |%10s |%10s |\n", "Ground Truth", "--", "--");
        for (int i = 0; i < k; i++)
            printf("|%30s |%8s%2d |%10d |\n", "Point", "top -", i + 1, top_current[i]->id);
        printf("---------------------------------------------------------\n");
        top_1_score = dot_prod(u,top_current[0]);

        //Algorithm HDPI_sampling
        HD_s += HDPI_sampling(p_set, u, k, theta);
        HD_s_rr += rr_ratio;
        HD_s_count += top_1_found;

        //Algorithm HDPI_accurate
        HD_a += HDPI_accurate(p_set, u, k, theta);
        HD_a_rr += rr_ratio;
        HD_a_count += top_1_found;

        //Algorithm PointPrune
        PP += PointPrune(p_set, u, k, checknum, theta);
        PP_rr += rr_ratio;
        PP_count += top_1_found;

        //Algorithm PointPrune_v2
        PP_2 += PointPrune_v2(p_set, u, k, checknum, theta);
        PP_2_rr += rr_ratio;
        PP_2_count += top_1_found;

        //Algorithm SamplePrune
        SP += SamplePrune(p_set, u, k, checknum, theta);
        SP_rr += rr_ratio;
        SP_count += top_1_found;
    }
    printf("\nIn total\n");
    printf("|%20s |%10s |%8s |%10s\n", "Alg", "# rounds", "rr", "found top1");
    printf("|%20s |%10f |%8f |%8f\n", "HDPI-Sampling", HD_s/10, HD_s_rr/10, ((double)HD_s_count)/10);
    printf("|%20s |%10f |%8f |%8f\n", "HDPI-Accurate", HD_a/10, HD_a_rr/10, ((double)HD_a_count)/10);
    printf("|%20s |%10f |%8f |%8f\n", "PointPrune", PP/10, PP_rr/10, ((double)PP_count)/10);
    printf("|%20s |%10f |%8f |%8f\n", "PointPrune_v2", PP_2/10, PP_2_rr/10, ((double)PP_2_count)/10);
    printf("|%20s |%10f |%8f |%8f\n", "SamplePrune", SP/10, SP_rr/10, ((double)SP_count)/10);

    release_point_set(P, true);
    return 0;
}


        // // generate the utility vector
        // srand(time(0));  // Initialize random number generator.
        // point_t *u = alloc_point(dim);
        // double sum = 0;
        // for (int i = 0; i < dim; i++)
        // {
        //     u->coord[i] = ((double) rand()) / RAND_MAX;
        //     sum += u->coord[i];
        // }
        // for (int i = 0; i < dim; i++)
        //     u->coord[i] = u->coord[i]/sum;


        // // look for the ground truth top-k point
        // std::vector<point_t *> top_current;
        // find_top_k(u, P, top_current, k);
        // printf("---------------------------------------------------------\n");
        // printf("|%30s |%10s |%10s |\n", "Ground Truth", "--", "--");
        // for (int i = 0; i < k; i++)
        //     printf("|%30s |%8s%2d |%10d |\n", "Point", "top -", i + 1, top_current[i]->id);
        // printf("---------------------------------------------------------\n");
        // top_1_score = dot_prod(u,top_current[0]);

        // //Algorithm HDPI
        // HDPI_sampling(p_set, u, k);
        // HDPI_accurate(p_set, u, k);

        // PointPrune(p_set, u, k, checknum, theta);

        // PointPrune_v2(p_set, u, k, checknum, theta);

        // SamplePrune(p_set, u, k, checknum, theta);