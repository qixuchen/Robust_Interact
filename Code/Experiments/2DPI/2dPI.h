#include "../Others/used_function.h"






extern int num_questions;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;

/*
 * @brief Used for 2D dimensional datasets.
 * Start from x-axis to y-axis, record the range of utility and the corresponding top-k record.
 * Then find one of the top-k point by asking questions
 * @param original_set       The original dataset
 * @param u                  User's utility function
 * @param k                  The threshold
 */
int twoDPI(std::vector<point_t*> &top_set, point_t* u, double theta);
