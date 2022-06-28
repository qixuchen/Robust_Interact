#include "./Others/used_function.h"
#include "./Others/user_mode.h"
#include "./HDPI/partition_func.h"



#include "Eigen/Eigen"
//#define VOLESTI_DEBUG
#include <fstream>
#include "boost/random.hpp"
#include "boost/random/uniform_int.hpp"
#include "boost/random/normal_distribution.hpp"
#include "boost/random/uniform_real_distribution.hpp"

#include "random_walks/random_walks.hpp"

#include "volume/volume_sequence_of_balls.hpp"
#include "volume/volume_cooling_gaussians.hpp"
#include "volume/volume_cooling_balls.hpp"

#include "exact_vols.h"
#include "generators/known_polytope_generators.h"
#include "sampling/sampling.hpp"

#define Precision 0.0000015


extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;
extern int top_k_correct;

/**
 * @brief                   Shift the old halfspace according to shift_point
 * 
 * @param old_halfspace     The old halfspace before shifting
 * @param shift_point       The shift point
 * @return halfspace_t*     halfspace after shifting
 */
halfspace_t * shift_halfspace(const halfspace_t *old_halfspace, std::vector<double> &shift_point);

halfspace_t *find_best_halfspace(std::vector<halfspace_t *> &selected_halfspaces, std::vector<std::vector<double>> &randPoints,
                                std::vector<double> &shift_point, double &ratio);

/**
 * @brief                   Sample a number of points from R
 * 
 * @param R                 The input polytope 
 * @param num_point         Number of points to be sampled
 * @param randPoints        The vector to store sampled points
 * @param shift_point       The point used to transform hyperplanes later
 */
void polytope_sampling(halfspace_set_t* R, int num_point, std::vector<std::vector<double>> &randPoints, std::vector<double> &shift_point);


int SamplePrune(std::vector<point_t *> p_set, point_t *u, int checknum, double theta);


int SamplePrune_containTop1(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int output_size);


int SamplePrune_alltopk(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int output_size, std::vector<point_t *> ground_truth);