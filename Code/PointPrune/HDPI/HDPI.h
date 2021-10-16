#include "../Others/used_function.h"
#include "../Others/user_mode.h"


extern int num_questions;
extern int num_wrong_answer;
/**
 * @brief Asking user question and return one of the top-k points
 *        Find the top-1 point by sampling
 * @param p_set 		 The dataset
 * @param u 			 The linear function
 * @param k 			 The parameter
 */
int HDPI_sampling(std::vector<point_t *> p_set, point_t *u, int k);

/**
 * @brief Asking user question and return one of the top-k point
 *        Find the top-1 point accurately convex Hull
 * @param p_set 		 The dataset
 * @param u 			 The linear function
 * @param k 			 The parameter
 */
int HDPI_accurate(std::vector<point_t *> p_set, point_t *u, int k);



int PointPrune(std::vector<point_t *> p_set, point_t *u, int k, int checknum, double theta);