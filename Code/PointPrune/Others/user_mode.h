#ifndef USER_MODE_H
#define USER_MODE_H

#include "data_struct.h"
#include "data_utility.h"
#include "operation.h"
#include <ctime>
#include <cstdlib>

point_t* user_rand_err(point_t* u, point_t* p1, point_t* p2, double err_rate);

point_t* checking(point_t* u, point_t* p1, point_t* p2, double err_rate, double k);

#endif