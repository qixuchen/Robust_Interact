#include "../Others/used_function.h"

extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;
extern int top_k_correct;

/**
 * @brief Ask user question and find one of the top-k point
 *        The input set is original, we refine it with skyband for finding the point which is the top-k point w.r.t any u in R
 * @param original_set 		The original dataset
 * @param u 				The linear function
 * @param k 				The threshold top-k
 */
void Random_half_onion(point_set_t* original_set, point_t* u, int k);


/**
 * @brief Ask user question and find one of the top-k point
 *        The input set is refined by skyband
 *        For find the point which is the top-k point w.r.t any u in R, we search the whole set
 * @param original_set 		The refined dataset
 * @param u 				The linear function
 * @param k 				The threshold top-k
 */
int Random_half(std::vector<point_t*> p_set, point_t* u, double theta);


int Random_half_Alltopk(std::vector<point_t*> p_set, point_t* u, double theta, int output_size, std::vector<point_t*> ground_truth);