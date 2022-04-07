#include "../Others/used_function.h"


extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;

/**
 * @brief Ask user questions and give a ranking
 * @param original_set 		The original dataset
 * @param u 				The linear function
 * @param theta				The user error rate
 */
int Active_Ranking(std::vector<point_t*> p_set, point_t* u,  double theta);