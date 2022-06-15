#include "../Others/used_function.h"
#include "../Others/user_mode.h"
#include "partition_func.h"

extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;
extern int top_k_correct;

/**
 * @brief Asking user question and return one of the top-k point
 *        Find the top-1 point accurately convex Hull
 * @param p_set 		 The dataset
 * @param u 			 The linear function
 * @param k 			 The parameter
 */
int HDPI_accurate(std::vector<point_t *> p_set, point_t *u, double theta);

int HDPI_accurate_containtop1(std::vector<point_t *> p_set, point_t *u, double theta, int output_size);

/**
 * @brief Asking user question and return one of the top-k point
 *        Find the top-1 point accurately convex Hull
 * @param p_set 		 The dataset
 * @param u 			 The linear function
 * @param k 			 The parameter
 */
int HDPI_accurate_Alltopk(std::vector<point_t *> p_set, point_t *u, double theta, int k, std::vector<point_t*> ground_truth);
