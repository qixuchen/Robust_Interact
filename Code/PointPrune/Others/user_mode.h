#include "data_struct.h"
#include "data_utility.h"
#include "operation.h"
#include <ctime>
#include <cstdlib>

#ifndef USER_MODE_H
#define USER_MODE_H

extern int num_wrong_answer;
extern int crit_wrong_answer;
extern int num_questions;

int user_rand_err(const std::vector<point_t *> &p_set, point_t* u, int p1, int p2, double err_rate);

point_t* user_rand_err(point_t* u, point_t* p1, point_t* p2, double err_rate);


int checking(const std::vector<point_t *> &p_set, point_t* u, int p1, int p2, double err_rate, int k);

point_t* checking(point_t* u, point_t* p1, point_t* p2, double err_rate, int k);

point_t* checking_varyk(point_t* u, point_t* p1, point_t* p2, double err_rate, int k, double skip_rate);

#endif