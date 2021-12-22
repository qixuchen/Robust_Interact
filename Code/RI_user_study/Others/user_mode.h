#include "data_struct.h"
#include "data_utility.h"
#include "used_function.h"
#include "operation.h"
#include <ctime>
#include <cstdlib>

#ifndef USER_MODE_H
#define USER_MODE_H

extern int num_checking;
extern int num_wrong_checking;

int checking_varyk(point_t* p1, point_t* p2, int k, double skip_rate, int &num_questions, double *max, double *min);

#endif