#include "./Others/used_function.h"
#include "./Others/user_mode.h"
#include "./HDPI/partition_func.h"

#ifndef POINTPRUNE_ORIGIN
#define POINTPRUNE_ORIGIN

extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;
extern int top_k_correct;

int find_best_hyperplane(std::vector<choose_item*> choose_item_set, std::vector<halfspace_t *> &selected_halfspaces, 
                        point_t *&best_p1, point_t *&best_p2, double &ratio);
/**
 * @brief           PointPrune, with some modifications (e.g., end prematurely) 
 */
int PointPrune_v2(std::vector<point_t *> p_set, point_t *u, int checknum, double theta);


int PointPrune_containTop1(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int output_size);


int PointPrune_Alltopk(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int output_size, std::vector<point_t *> ground_truth);

#endif
