// VolEsti (volume computation and sampling library)

// Copyright (c) 20012-2018 Vissarion Fisikopoulos
// Copyright (c) 2018 Apostolos Chalkis

// Licensed under GNU LGPL.3, see LICENCE file

// VolEsti example

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

// template
// <
//     typename WalkType,
//     typename Polytope,
//     typename RNG
// >
// WalkType func(Polytope const& Pin,RNG &rng)
// {
//     typedef typename Polytope::PointType Point;
//     typedef typename Point::FT NT;
//     typedef Ball<Point> BallType;
//     typedef BallIntersectPolytope <Polytope, BallType> PolyBall;
//     typedef typename Polytope::VT VT;
//     typedef std::list <Point> PointList;
//     int n = Pin.dimension();
//     Point p(n);
//     return WalkType(Pin, p, rng);
//     // typedef typename WalkTypePolicy::template Walk
//     //                                           <
//     //                                             Polytope,
//     //                                             RandomNumberGenerator
//     //                                           > WalkType;
// }




int main()
{
    typedef double NT;
    typedef Cartesian<NT>    Kernel;
    typedef typename Kernel::Point    Point;
    typedef boost::mt19937    RNGType;
    typedef HPolytope<Point> Hpolytope;
    typedef BoostRandomNumberGenerator<boost::mt11213b, NT> RNG;

    std::cout << "Volume algorithm: Cooling Balls" << std::endl << std::endl;

    /*
    //  Important: move the polytope to the origin
        auto InnerBall = P.ComputeInnerBall();
        Point c = InnerBall.first;
    //  P.normalize();
    //  P.shift(c.getCoefficients());
    */


    using std::vector;
    vector<vector<double>> HP_vec;
    vector<double> d_v{4,3};
    HP_vec.push_back(d_v);
    
    vector<double> v1{4,1,1};
    vector<double> v2{0,-1,1};
    vector<double> v3{4,1,-1};
    vector<double> v4{0,-1,-1};
    HP_vec.push_back(v1);
    HP_vec.push_back(v2);
    HP_vec.push_back(v3);
    HP_vec.push_back(v4);


    Hpolytope HP;
    HP.init(HP_vec);

    //Hpolytope HP = gen_cube<Hpolytope>(10, false);
    HP.print();
    //Compute chebychev ball
    std::pair<Point,NT> CheBall;
    CheBall = HP.ComputeInnerBall();

    // Setup the parameters
    int n = HP.dimension();
    int walk_len=1;
    int n_threads=1;
    NT e=1, err=0.1;
    NT C=2.0, ratio, frac=0.1, delta=-1.0;
    int N = 500 * ((int) C) + ((int) (n * n / 2));
    int W = 6*n*n+800;
    ratio = 1.0-1.0/(NT(n));

    int rnum = std::pow(e,-2) * 400 * n * std::log(n);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    RNGType rng(seed);
    boost::normal_distribution<> rdist(0,1);
    boost::random::uniform_real_distribution<>(urdist);
    boost::random::uniform_real_distribution<> urdist1(-1,1);

    double tstart;

    ////////////////////////////////////////////////////////////////
    /// H-Polytopes
    ///
    ///
    ///

    std::cout << "Volume estimation on H-polytopes (cube-10)" << std::endl;

    RNG gen(n);

    auto InnerBall = HP.ComputeInnerBall();
    Point c = InnerBall.first;
    HP.normalize();

    // std::cout << "After normalize:"<<std::endl;
    // HP.print();
    // std::cout <<std::endl;

    HP.shift(c.getCoefficients());

    // std::cout << "Point C:"<<std::endl;
    // c.print();
    // std::cout <<std::endl;

    // std::cout << "After shifting:"<<std::endl;
    // HP.print();
    // std::cout <<std::endl;


    Point p(n);

    vector<Point> p_vec;

    uniform_sampling<BilliardWalk>(p_vec, HP, gen, walk_len, 10, p, 10);
    for(int i=0; i<p_vec.size(); i++){
        p_vec[i].print();
    }
    /*
    tstart = (double)clock()/(double)CLOCKS_PER_SEC;
    std::cout << "BallWalk (cube) = "
              << volume_cooling_balls<BallWalk, RNG>(HP, e, walk_len) << " , ";
    std::cout << (double)clock()/(double)CLOCKS_PER_SEC - tstart << std::endl;
    tstart = (double)clock()/(double)CLOCKS_PER_SEC;

    tstart = (double)clock()/(double)CLOCKS_PER_SEC;
    std::cout << "RDHRWalk (cube) = "
              << volume_cooling_balls<RDHRWalk, RNG>(HP, e, walk_len) << " , ";
    std::cout << (double)clock()/(double)CLOCKS_PER_SEC - tstart << std::endl;

    tstart = (double)clock()/(double)CLOCKS_PER_SEC;
    std::cout << "CDHRWalk (cube) = "
              << volume_cooling_balls<CDHRWalk, RNG>(HP, e, walk_len) << " , ";
    std::cout << (double)clock()/(double)CLOCKS_PER_SEC - tstart << std::endl;
    */

    tstart = (double)clock()/(double)CLOCKS_PER_SEC;
    std::cout << "BirdWalk (cube) = "
              << volume_cooling_balls<BilliardWalk, RNG>(HP, e, walk_len) << " , ";
    std::cout << (double)clock()/(double)CLOCKS_PER_SEC - tstart << std::endl;
    


   
    return 0;
}

/*
    point(const unsigned int dim, iter begin, iter endit)
    {
        d = dim;
        coeffs.resize(d);
        int i = 0;

        for (iter it=begin ; it != endit ; it++)
            coeffs(i++) = *it;
    }
    point(const unsigned int dim, std::vector<typename K::FT> cofs)
    {
        d = dim;
        coeffs.resize(d);
        iter it = cofs.begin();
        int i=0;

        for (; it != cofs.end(); it++,i++)
            coeffs(i) = *it;

    }
    
    //define matrix A and vector b, s.t. Ax<=b and the dimension
    void init(std::vector<std::vector<NT> > const& Pin)
    {
        _d = Pin[0][1] - 1;
        A.resize(Pin.size() - 1, _d);
        b.resize(Pin.size() - 1);
        for (unsigned int i = 1; i < Pin.size(); i++) {
            b(i - 1) = Pin[i][0];
            for (unsigned int j = 1; j < _d + 1; j++) {
                A(i - 1, j - 1) = -Pin[i][j];
            }
        }
    }
*/