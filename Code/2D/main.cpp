#include "Others/data_utility.h"
#include "Others/data_struct.h"
#include "Others/read_write.h"
#include "Others/pruning.h"
#include "Others/qhull_build.h"
#include "Others/used_function.h"
#include "ST2D/ST2D.h"
#include <vector>
#include <ctime>
#include <sys/time.h>

int main(int argc, char *argv[]){
    point_set_t *P = read_points((char*)"2d.txt");
    int dim = P->points[0]->dim; //obtain the dimension of the point
    double theta=0;
    int k = 1;
    std::vector<point_t *> p_set, top, p_convh;
    skyband(P, p_set, 1);
    find_top1(p_set, top);
    skyline_c(top, p_convh); //p_convh contains all the top-1 point on the convex hull
     // generate the utility vector
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
    point_set_t *P0 = point_reload(p_convh);
    std::vector<point_t *> top_current;
    find_top_k(u, P0, top_current, 1);
    printf("---------------------------------------------------------\n");
    printf("|%30s |%10s |%10s |\n", "Ground Truth", "--", "--");
    printf("|%30s |%8s%2d |%10d |\n", "Point", "top -", 1, top_current[0]->id);
    printf("---------------------------------------------------------\n");


    ST2D(p_convh, u, k, theta);

    release_point_set(P0, true);
    return 0;

}