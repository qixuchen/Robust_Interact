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


int main(int argc, char *argv[])
{
    point_set_t *P0 = read_points((char*)"4d100k.txt");
    int dim = P0->points[0]->dim; //obtain the dimension of the point
    int k =1;
    int checknum=3;
    double theta=0.05;
    std::vector<point_t *> p_set, p0;
    skyband(P0, p_set, k);
    point_set_t *P = point_reload(p_set);

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

    //Algorithm HDPI
    HDPI_sampling(p_set, u, k);
    //HDPI_accurate(p_set, u, k);

    //PointPrune(p_set, u, k, checknum, theta);

    //PointPrune_v2(p_set, u, k, checknum, theta);

    SamplePrune(p_set, u, k, checknum, theta);

    release_point_set(P, true);
    return 0;
}
