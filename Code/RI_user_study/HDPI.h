#include "Others/used_function.h"
#include "RI/partition_func.h"

/**
 * @brief Asking user question and return one of the top-k point
 *        For find the point which is the top-k point w.r.t any u in R, we search the whole set
 *        Find the top-1 point by sampling
 * @param p_set 		 The original dataset
 * @param u 			 The linear function
 */
int HDPI_sampling(FILE *wPtr, std::vector<point_t *> p_set, point_set_t *P0, int &questions, int alg_num);

/**
 * @brief Asking user question and return one of the top-k point
 *        For find the point which is the top-k point w.r.t any u in R, we search the whole set
 *        Find the top-1 point accurately
 * @param p_set 		 The original dataset
 * @param u 			 The linear function
 */
int HDPI_accurate(FILE *wPtr, std::vector<point_t *> p_set, point_set_t *P0, int &questions, int alg_num);
