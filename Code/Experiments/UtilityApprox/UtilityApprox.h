#ifndef FAKEPOINTS
#define FAKEPOINTS

#include "../Others/data_struct.h"
#include "../Others/data_utility.h"
#include "../Others/used_function.h"

#include "../Others/operation.h"

extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;

/**
 * @brief Danupon's UtilityApprox algorithm
 * @param P the dataset
 * @param u the real utility vector
 * @param s the number of points shown in each question
 * @param epsilon the threshold of regret ratio
 * @param maxRound the upper bound of number of questions
 * @param theta user error rate
 */
int utilityapprox(point_set_t *P, point_t *u, int s, double epsilon, int maxRound, double theta);


#endif