#include "../Others/used_function.h"
#include "../Others/user_mode.h"
#include "partition_func.h"



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


point_t* SamplePrune(FILE *wPtr, std::vector<point_t *> p_set, point_set_t *P0, int checknum, int &questions);
