#include "./Others/used_function.h"
#include "./Others/user_mode.h"
#include "./HDPI/partition_func.h"
#include "exp_stats.h"
#include <ctime>
#include <cmath>
#include <chrono>
#include <sys/time.h>

/**
 * @brief           PointPrune, with some modifications (e.g., end prematurely) 
 */
int PointPrune_v2(std::vector<point_t *> p_set, point_t *u, int checknum, double theta);