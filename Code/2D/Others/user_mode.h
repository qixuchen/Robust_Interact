#include "data_struct.h"
#include "data_utility.h"
#include "operation.h"
#include <ctime>
#include <cstdlib>

#ifndef USER_MODE_H
#define USER_MODE_H

extern int num_wrong_answer;

int user_rand_err(const std::vector<point_t *> &p_set, point_t* u, int p1, int p2, double err_rate);

int checking(const std::vector<point_t *> &p_set, point_t* u, int p1, int p2, double err_rate, int k);

#endif