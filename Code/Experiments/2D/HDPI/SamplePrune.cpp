#include "SamplePrune.h"
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
                                std::vector<double> &shift_point, double &ratio){
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
        if(cur_num >= best_num){
            best_num=cur_num;
            best_index=i;
        }
    }

    ratio = ((double) best_num)/randPoints.size();
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

    shift_point.clear();
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
    // HP.print();

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

    // std::cout << "After shifting:"<<std::endl;
    // HP.print();
    // std::cout <<std::endl;


    Point p(n);

    vector<Point> p_vec;

    uniform_sampling<BilliardWalk>(p_vec, HP, gen, walk_len, num_point, p, 10);
    // for(int i=0; i<p_vec.size(); i++){
    //     p_vec[i].print();
    // }

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



int SamplePrune(std::vector<point_t *> p_set, point_t *u, int checknum, double theta)
{
    int k=1;
    //reset statistics
    num_questions=0;
    num_wrong_answer=0;
    crit_wrong_answer=0;

    int num_points=50;
    std::vector<std::vector<double>> randPoints;
    std::vector<double> shift_point;



    //p_set_1 contains the points which are not dominated by >=1 points
    //p_set_k contains the points which are not dominated by >=k points
    //p_top_1 contains the points which are the convex points
    std::vector<point_t *> p_top_1;
    int dim = p_set[0]->dim;
    point_t *uk = alloc_point(dim);
    find_top1_sampling(p_set, p_top_1, uk, 0, 0);//use sampling method
    release_point(uk);
    //half_set_set          contains all the partitions(intersection of halfspaces)
    //considered_half_set   contains all the possible partitions considered
    //choose_item_points    contains all the points used to construct hyperplanes(questions) (set C in the paper)
    //choose_item_set       contains all the hyperplanes(questions) which can be asked user
    //R_half_set            The utility range R
    std::vector<halfspace_set_t *> half_set_set, half_set_set_cp;
    std::vector<int> considered_half_set, considered_half_set_cp;   //[i] shows the index in half_set_set
    std::vector<point_t *> choose_item_points, choose_item_points_cp;
    std::vector<choose_item *> choose_item_set,  choose_item_set_cp;
    std::vector<halfspace_t*> selected_halfspaces;
    halfspace_set_t *R_half_set_cp = R_initial(dim);
    halfspace_set_t *R_half_set = R_initial(dim);
    get_extreme_pts_refine_halfspaces_alg1(R_half_set_cp);
    get_extreme_pts_refine_halfspaces_alg1(R_half_set);
    construct_halfspace_set_with_copy(p_top_1, choose_item_points, choose_item_points_cp, half_set_set, 
                                    half_set_set_cp, considered_half_set, considered_half_set_cp);

    build_choose_item_table(half_set_set, choose_item_points, choose_item_set);
    for(int i =0 ; i<choose_item_set.size();i++){
        choose_item * c_i=deepcopy_choose_item(choose_item_set[i]);
        choose_item_set_cp.push_back(c_i);
    }

    point_t* point_result = NULL, *true_point_result = NULL;
    halfspace_t *hy=NULL, *hy_cp=NULL;
    bool encounter_err = false, end_premature=false;

    while(true_point_result==NULL){

        point_result = NULL;
        //index: the index of the chosen hyperplane(question)
        //p1:    the first point
        //p2:    the second point
        int index = choose_best_item(choose_item_set);
        // double v1 = dot_prod(u, choose_item_set[index]->hyper->point1);
        // double v2 = dot_prod(u, choose_item_set[index]->hyper->point2);
        point_t* p1 = choose_item_set[index]->hyper->point1;
        point_t* p2 = choose_item_set[index]->hyper->point2;
        point_t* user_choice = user_rand_err(u,p1,p2,theta);



        //start of phase 1
        //==========================================================================================================================================

        while (point_result==NULL)
        {
            if (user_choice==p1)
            {
                hy = alloc_halfspace(p2, p1, 0, true);
                index = modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, true);
            }
            else
            {
                hy = alloc_halfspace(p1, p2, 0, true);
                index = modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, false);
            }
            p1 = choose_item_set[index]->hyper->point1;
            p2 = choose_item_set[index]->hyper->point2;
            user_choice = user_rand_err(u,p1,p2,theta);

            //Find whether there exist point which is the topk point w.r.t any u in R
            R_half_set->halfspaces.push_back(hy);
            halfspace_t *hy2 = deepcopy_halfspace(hy);
            selected_halfspaces.push_back(hy2);

            get_extreme_pts_refine_halfspaces_alg1(R_half_set);
            std::vector<point_t *> top_current;
            point_result = NULL;
            bool check_result = find_possible_topk(p_set, R_half_set, k, top_current);
            if (check_result)
            {
                point_result = check_possible_topk(p_set, R_half_set, k, top_current);
            }
            else if(considered_half_set.size() == 1){
                point_result=half_set_set[considered_half_set[0]]->represent_point[0];
            }
        }
        //=================================================================================================================================
        //End of phase 1

        encounter_err = false, end_premature=false;
        while(true_point_result==NULL && selected_halfspaces.size()>0){
            double prune_ratio=0;
            randPoints.clear();
            shift_point.clear();
            polytope_sampling(R_half_set_cp, num_points, randPoints, shift_point);


            point_t *best_p1=NULL, *best_p2=NULL;
            halfspace_t *best_hs = find_best_halfspace(selected_halfspaces, randPoints,shift_point, prune_ratio);
            best_p1 = best_hs->point1;
            best_p2 = best_hs->point2;

            //find the corresponding hyperplane index in choose_item_set_cp
            int item_size = choose_item_set_cp.size();
            int best_index=-1;
            for(int j=0;j<item_size;j++){
                hyperplane_t* h= alloc_hyperplane(best_hs->normal, best_hs->offset);
                if(is_same_hyperplane(h,choose_item_set_cp[j]->hyper)){
                    best_index=j;
                    release_hyperplane(h);
                    break;
                }
                release_hyperplane(h);
            }

            if(best_index<0){
                printf("Cannot find the best index\n");
                return -1;
            }

            point_t *p1, *p2, *user_choice;
            p1 = choose_item_set_cp[best_index]->hyper->point1;
            p2 = choose_item_set_cp[best_index]->hyper->point2;

            double skip_rate = 0.2;
            //printf("%10f\n",ratio);
            if(best_p1==p1){
                //user_choice = checking(u,p2,p1,theta,checknum);
                user_choice = checking_varyk(u,p2,p1,theta,checknum,skip_rate);
            }
            else{
                //user_choice = checking(u,p1,p2,theta,checknum);
                user_choice = checking_varyk(u,p1,p2,theta,checknum,skip_rate);
            }
            if(user_choice!=best_p2){
                encounter_err = true;
            }
            if(encounter_err==true && prune_ratio<0.2){
                //printf("enc err\n");
                end_premature=true;
            }
            
            halfspace_t *hy_cp=NULL;

            if (user_choice==p1)
            {
                //encounter_err = true;
                hy_cp = alloc_halfspace(p2, p1, 0, true);
                best_index=modify_choose_item_table(choose_item_set_cp, half_set_set_cp, considered_half_set_cp, best_index, true);
            }
            else
            {
                hy_cp = alloc_halfspace(p1, p2, 0, true);
                modify_choose_item_table(choose_item_set_cp, half_set_set_cp, considered_half_set_cp, best_index, false);
            }

            R_half_set_cp->halfspaces.push_back(hy_cp);
            get_extreme_pts_refine_halfspaces_alg1(R_half_set_cp);

            // IMPORTANT: Remove the halfspaces that are no longer helpful (lies on one side of R)
            int m=0;
            while(m<selected_halfspaces.size()){
                hyperplane_t *h = alloc_hyperplane(selected_halfspaces[m]->normal, selected_halfspaces[m]->offset);
                if(check_situation_accelerate(h,R_half_set_cp,0)!=0){
                    selected_halfspaces.erase(selected_halfspaces.begin()+m);
                }
                else{
                    m++;
                }
            }

            std::vector<point_t *> top_current;
            true_point_result = NULL;
            bool check_result = find_possible_topk(p_set, R_half_set_cp, k, top_current);
            if (check_result)
            {
                true_point_result = check_possible_topk(p_set, R_half_set_cp, k, top_current);
            }
            else if(considered_half_set_cp.size() == 1){
                true_point_result=half_set_set_cp[considered_half_set_cp[0]]->represent_point[0];
            }

            if(end_premature){

                //printf("end premature\n");
                break;
            }
        }

        if(true_point_result!=NULL){
            break;
        }

        //  re-initialize all data structures used in the inner loop with the record of the outer loop
        //  std::vector<halfspace_set_t *> half_set_set
        //  std::vector<int> considered_half_set
        //  std::vector<point_t *> choose_item_points
        //  std::vector<choose_item *> choose_item_set
        //  std::vector<halfspace_t*> selected_halfspaces
        //  halfspace_set_t *R_half_set

        selected_halfspaces.clear();

        release_halfspace_set(R_half_set);
        R_half_set = R_initial(dim);
        R_half_set = deepcopy_halfspace_set(R_half_set_cp);
        get_extreme_pts_refine_halfspaces_alg1(R_half_set);

        choose_item_points.clear();
        for(int i=0; i<choose_item_points_cp.size();i++){
            choose_item_points.push_back(choose_item_points_cp[i]);
        }

        considered_half_set.clear();
        for(int i=0; i<considered_half_set_cp.size();i++){
            considered_half_set.push_back(considered_half_set_cp[i]);
        }
        //////
        half_set_set.clear();
        // while(half_set_set.size()>0){
        //     release_halfspace_set(half_set_set[0]);
        //     half_set_set.erase(half_set_set.begin());
        // }
        // half_set_set.clear();
        ////////
        for(int i=0; i<half_set_set_cp.size();i++){
            halfspace_set_t *hs = deepcopy_halfspace_set(half_set_set_cp[i]);
            half_set_set.push_back(hs);
        }

        while(choose_item_set.size()>0){
            release_choose_item(choose_item_set[0]);
            choose_item_set.erase(choose_item_set.begin());
        }
        choose_item_set.clear();
        for(int i =0 ; i<choose_item_set_cp.size();i++){
            choose_item * c_i=deepcopy_choose_item(choose_item_set_cp[i]);
            choose_item_set.push_back(c_i);
        }


    }

    printf("|%30s |%10d |%10s |\n", "SamplePrune", num_questions, "--");
    printf("|%30s |%10s |%10d |\n", "Point", "--", true_point_result->id);
    printf("---------------------------------------------------------\n");
    printf("# of wrong answers:%d\n",num_wrong_answer);
    printf("# of critical wrong answers:%d\n",crit_wrong_answer);
    printf("regret ratio: %10f \n", dot_prod(u, true_point_result)/top_1_score);
    rr_ratio = dot_prod(u, true_point_result)/top_1_score;
    top_1_found= (rr_ratio>=1);
    return num_questions;
    
}