#include "../Others/used_function.h"
#include "../Others/user_mode.h"
#include "partition_func.h"

extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;



int PointPrune(std::vector<point_t *> p_set, point_t *u, int k, int checknum, double theta);


/**
 * @brief           PointPrune, with some modifications (e.g., end prematurely) 
 */
int PointPrune_v2(std::vector<point_t *> p_set, point_t *u, int k, int checknum, double theta);