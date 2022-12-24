#include "../Others/used_function.h"
#include "./exp_stats.h"

/**
 * @brief Ask user questions and give a ranking
 * @param original_set 		The original dataset
 * @param u 				The linear function
 * @param theta				The user error rate
 */
int Active_Ranking(std::vector<point_t*> p_set, point_t* u,  double theta);