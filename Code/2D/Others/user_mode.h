#include "data_struct.h"
#include "data_utility.h"
#include "operation.h"
#include <ctime>
#include <cstdlib>

#ifndef USER_MODE_H
#define USER_MODE_H

point_t* user_rand_err(point_t* u, point_t* p1, point_t* p2, double err_rate);

int checking(std::vector<point_t *> p_set, point_t* u, int p1, int p2, double err_rate, int k);

#endif