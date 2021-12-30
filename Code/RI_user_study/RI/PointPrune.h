#include "../Others/user_mode.h"
#include "partition_func.h"




int PointPrune(FILE *wPtr, std::vector<point_t *> p_set, point_set_t *P0, int checknum, int &questions, double *max, double *min);


/**
 * @brief           PointPrune, with some modifications (e.g., end prematurely) 
 */
int PointPrune_v2(FILE *wPtr, std::vector<point_t *> p_set, point_set_t *P0, int checknum, int &questions, double *max, double *min);