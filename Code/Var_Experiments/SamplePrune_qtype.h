#include "Others/used_function.h"
#include "Others/user_mode.h"
#include "./HDPI/partition_func.h"
#include "UH/maxUtility.h"
#include "./SamplePrune.h"
#include "./variant_func.h"

extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;
extern int top_k_correct;

extern int i1_p1;
extern int i1_p2;
extern int i2_p1;
extern int i2_p2;
extern int i3_p1;
extern int i3_p2;


int SamplePrune_morethan2points(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int point_num);


int SamplePrune_desired_undesired(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int point_num);

