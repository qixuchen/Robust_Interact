#include "../Others/user_mode.h"
#include "partition_func.h"

extern int num_questions;




int PointPrune(std::vector<point_t *> p_set, point_t *u, int k, int checknum, double theta);


/**
 * @brief           PointPrune, with some modifications (e.g., end prematurely) 
 */
int PointPrune_v2(FILE *wPtr, std::vector<point_t *> p_set, point_set_t *P0, int checknum);