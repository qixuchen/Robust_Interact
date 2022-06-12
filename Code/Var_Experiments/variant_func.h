#ifndef VAR_FUNC
#define VAR_FUNC

#include "./Others/used_function.h"
#include "./Others/user_mode.h"
#include "./HDPI/partition_func.h"
#include <cmath>
#include <algorithm>
#include <utility>



/**
 * @brief check if the returned set contains identical points.
 * 
 * @param point_set 
 * @return true 
 * @return false 
 */
bool contain_same_point(std::vector<point_t *> point_set);

/**
 * @brief user choose one point from the displayed points.
 * 
 * @param u             the utiliy vector
 * @param point_set     the set of displayed point
 * @return int     the idx of the point chosen by the user
 */
int user_choose_point(point_t* u, std::vector<point_t *> point_set);

/**
 * @brief user partition the points into desired group and undesired group
 * 
 * @param u             the utiliy vector
 * @param point_set     the set of displayed point
 * @param desired       the set containing all desried points
 * @param undesired     the set containing all undesried points
 * @return int     the idx of the point chosen by the user
 */
void user_choose_desired_undesired(point_t* u, std::vector<point_t *> point_set, std::vector<point_t *> & desired, std::vector<point_t *> & undesired);


/**
 * @brief   Check if hp is a valid hyperplan in choose_item_set.
 *          If yes, return the index in choose_item_set.
 *          If not, return -1
 * 
 * @param choose_item_set 
 * @param hp 
 * @return int 
 */
int check_hyperplane_validity(std::vector<choose_item*> choose_item_set, hyperplane_t *hp);


/**
 * @brief   Choose k items to display 
 * 
 * @param choose_item_set 
 * @param chosen_point_set 
 * @param k 
 * @return true 
 * @return false 
 */
bool choose_k_best_item(std::vector<choose_item*> choose_item_set, std::vector<point_t *> &chosen_point_set, int k);



#endif