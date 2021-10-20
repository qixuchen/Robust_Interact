#include "alg_spaceprune.h"
#include <sys/time.h>

/**
 * @brief                   Shift the old halfspace according to shift_point
 * 
 * @param old_halfspace     The old halfspace before shifting
 * @param shift_point       The shift point
 * @return halfspace_t*     halfspace after shifting
 */
halfspace_t * shift_halfspace(const halfspace_t *old_halfspace, std::vector<double> &shift_point){
    halfspace_t *hs = alloc_halfspace(old_halfspace->point1, old_halfspace->point2, 
                                        old_halfspace->offset, old_halfspace->direction);
    hs->offset=0;

    int dim = hs->point1->dim;
    double sum=0;
    for(int i=0;i<dim;i++){
        sum+=hs->normal->coord[i]*shift_point[i];
    }
    hs->offset-=sum;
    return hs;
}

halfspace_t *find_best_halfspace(std::vector<halfspace_t *> &selected_halfspaces, std::vector<std::vector<double>> &randPoints,
                                std::vector<double> &shift_point){
    if(selected_halfspaces.size()==0){
        printf("No halfspace can be selected");
        return NULL;
    }

    int dim = selected_halfspaces[0]->normal->dim;
    int best_num=0;
    int best_index=-1;
    for(int i=0; i<selected_halfspaces.size(); i++){
        int cur_num=0;
        halfspace_t *cur_hs = shift_halfspace(selected_halfspaces[i],shift_point);
        for(int j=0; j<randPoints.size(); j++){
            double sum=0;
            for(int k=0; k<dim; k++){
                sum += cur_hs->normal->coord[k]*randPoints[j][k];
            }
            if(sum>cur_hs->offset){
                cur_num++;
            }
        }
        if(cur_num > best_num){
            best_num=cur_num;
            best_index=i;
        }
    }

    if(best_index<0){
        printf("SpacePrune cannot find the best hyperplane");
        return NULL;
    }
    halfspace_t * res=selected_halfspaces[best_index];
    selected_halfspaces.erase(selected_halfspaces.begin()+best_index);
    return res; 
}


/**
 * @brief                   Sample a number of points from R
 * 
 * @param R                 The input polytope 
 * @param num_point         Number of points to be sampled
 * @param randPoints        The vector to store sampled points
 * @param shift_point       The point used to transform hyperplanes later
 */
void polytope_sampling(halfspace_set_t* R, int num_point, std::vector<std::vector<double>> &randPoints, std::vector<double> &shift_point){

    typedef double NT;
    typedef Cartesian<NT>    Kernel;
    typedef typename Kernel::Point    Point;
    typedef boost::mt19937    RNGType;
    typedef HPolytope<Point> Hpolytope;
    typedef BoostRandomNumberGenerator<boost::mt11213b, NT> RNG;

    int M = R->halfspaces.size();
    if (M < 1)
    {
        printf("%s\n", "Error: No halfspace in the set.");
        return;
    }
    int dim = R->halfspaces[0]->normal->dim;


    vector<vector<double>> HP_vec;
    vector<double> d_v;
    d_v.push_back(M+1);
    d_v.push_back(dim+1);
    HP_vec.push_back(d_v);
    //Initialize Hpolytope
    //IMPORTANT: need to multiply -1
    for(int i=0;i<M;i++){
        HP_vec.push_back(std::vector<double>());
        HP_vec[i+1].push_back(0);
        for (int j = 0; j < dim; j++)
        {
            HP_vec[i+1].push_back(-1*R->halfspaces[i]->normal->coord[j]);
        }

    }
    HP_vec.push_back(std::vector<double>());
    HP_vec[M+1].push_back(1);
    for (int i = 0; i < dim; i++){
        HP_vec[M+1].push_back(-1);
    }

    Hpolytope HP;
    HP.init(HP_vec);
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

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    RNGType rng(seed);
    boost::normal_distribution<> rdist(0,1);
    boost::random::uniform_real_distribution<>(urdist);
    boost::random::uniform_real_distribution<> urdist1(-1,1);

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

    std::cout << "After shifting:"<<std::endl;
    HP.print();
    std::cout <<std::endl;


    Point p(n);

    vector<Point> p_vec;

    uniform_sampling<BilliardWalk>(p_vec, HP, gen, walk_len, num_point, p, 10);
    for(int i=0; i<p_vec.size(); i++){
        p_vec[i].print();
    }

    //load points into randPoints
    randPoints.clear();
    for(int i=0; i<p_vec.size(); i++){
        randPoints.push_back(vector<double>());
        for (int j = 0; j < dim; j++)
        {
            randPoints[i].push_back(p_vec[i][j]);
        }
    }

    //load points into shift_point
    for (int j = 0; j < dim; j++){
        shift_point.push_back(c[j]);
    }
    
    
}



/**
 * @brief The SpacePrune Algorithm
 * @param p_set 		    The refined dataset
 * @param u 				The linear utility function
 * @param k 				The threshold top-k
 * @param theta             User error Rate
 */
int SpacePrune(std::vector<point_t*> p_set, point_t* u, int k, double theta)
{


    point_random(p_set);
    int dim = p_set[0]->dim, M = p_set.size(), numOfQuestion = 0;
    int num_points=20;
    

    //initial: add the basic halfspace into R_hyperplane
    halfspace_set_t* R_half_set = R_initial(dim);
    halfspace_set_t* R_half_set_cp = R_initial(dim);

    std::vector<std::vector<double>> randPoints;
    std::vector<double> shift_point;


    std::vector<point_t*> top_current;
    std::vector<point_t*> current_point, current_use;

    std::vector<halfspace_t*> selected_halfspaces;

    point_t* point_result = NULL, *true_point_result=NULL;
    current_use.push_back(p_set[0]);
    //compare: p_set contains all the points

    int i =1;
    while(i<M && point_result==NULL){

        bool same_exist=false;
        for(int j=0; j < current_use.size(); j++)
        {
            if(is_same_point(p_set[i], current_use[j]))
            {
                same_exist=true;
                break;
            }
        }
        if(!same_exist)
        {
            for(int j=0; j < current_use.size(); j++)
            {
                current_point.push_back(current_use[j]);
            }
            current_use.push_back(p_set[i]);
            while(current_point.size()>0)
            {
                int num_point = current_point.size();
                int scan_index = 0;//the index where the points we have scanned
                bool need_ask = false;
                double distance = 999999;
                int p_index = -1;// the index where the point we select to ask question
                //find the question asked user
                for(int j=0; j < num_point; j++)
                {
                    hyperplane_t* h = alloc_hyperplane(p_set[i], current_point[scan_index], 0);
                    int relation = check_situation_accelerate(h, R_half_set, 0);
                    //if intersect, calculate the distance
                    if(relation==0)
                    {
                        need_ask=true;
                        double d_h;
                        d_h=calculate_distance(h, R_half_set->in_center);
                        if(d_h < distance)
                        {
                            distance=d_h;
                            p_index=scan_index;
                        }
                        scan_index++;
                    }
                    else
                    {
                        current_point.erase(current_point.begin() + scan_index);
                    }
                }

                if(need_ask)
                {
                    numOfQuestion++;
                    double v1=dot_prod(u, p_set[i]);
                    double v2=dot_prod(u, current_point[p_index]);
                    halfspace_t* half=NULL;
                    if(v1>v2)
                    {
                        half=alloc_halfspace(current_point[p_index], p_set[i], 0, true);
                        R_half_set->halfspaces.push_back(half);
                    }
                    else
                    {
                        half=alloc_halfspace(p_set[i], current_point[p_index], 0, true);
                        R_half_set->halfspaces.push_back(half);
                    }
                    halfspace_t *hy2 = deepcopy_halfspace(half);
                    selected_halfspaces.push_back(hy2);
                    current_point.erase(current_point.begin() + p_index);
                    get_extreme_pts_refine_halfspaces_alg1(R_half_set);

                    //polytope_sampling(R_half_set, num_points, randPoints, shift_point);
                    //cout<<randPoints[0].size()<<endl;
                    //check if we can find top-k point in R_half_set
                    top_current.clear(); 
                    //top_current.shrink_to_fit();
                    point_result = NULL;
                    bool check_result = find_possible_topk(p_set, R_half_set, k, top_current);
                    if(check_result)
                    {
                        point_result = check_possible_topk(p_set, R_half_set, k, top_current);
                    }
                }
            }
        }
        i++; //increment i
    }
    polytope_sampling(R_half_set_cp, num_points, randPoints, shift_point);
    find_best_halfspace(selected_halfspaces, randPoints,shift_point);
    printf("|%30s |%10d |%10s |\n", "RH", numOfQuestion, "--");
    printf("|%30s |%10s |%10d |\n", "Point", "--", point_result->id);
    printf("---------------------------------------------------------\n");
    return numOfQuestion;
}
