#include "../Others/used_function.h"
#include "../Others/user_mode.h"
#include "partition_func.h"

extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;

extern int i1_p1;
extern int i1_p2;
extern int i2_p1;
extern int i2_p2;
extern int i3_p1;
extern int i3_p2;

int PointPrune(std::vector<point_t *> p_set, point_t *u, int checknum, double theta);


/**
 * @brief           PointPrune, with some modifications (e.g., end prematurely) 
 */
int PointPrune_v2(std::vector<point_t *> p_set, point_t *u, int checknum, double theta);