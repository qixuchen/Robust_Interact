#include "Others/used_function.h"
#include "Others/user_mode.h"
#include "./HDPI/partition_func.h"
#include "UH/maxUtility.h"
#include "./PointPrune.h"
#include "./variant_func.h"

extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;
extern int top_k_correct;

int PointPrune_morethan2points(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int point_num);


int PointPrune_desired_undesired(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int point_num);


