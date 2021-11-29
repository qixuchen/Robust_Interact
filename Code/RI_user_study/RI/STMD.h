#include "../Others/used_function.h"
#include "../Others/user_mode.h"
#include "partition_func.h"
#include <math.h>
#include <algorithm>
extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;

int STMD(std::vector<point_t *> p_set, point_t *u, int k, double theta);