#include "STMD.h"
#include <sys/time.h>



int STMD(std::vector<point_t *> p_set, point_t *u, double theta){

    int k = 1;
    //p_set_1 contains the points which are not dominated by >=1 points
    //p_set_k contains the points which are not dominated by >=k points
    //p_top_1 contains the points which are the convex points
    std::vector<point_t *> p_set_1, top;
    //onion(p_set, p_set_1, 1);
    int dim = p_set[0]->dim;
    find_top1(p_set, top);
    skyline_c(top, p_set_1);
    //half_set_set          contains all the partitions(intersection of halfspaces)
    //considered_half_set   contains all the possible partitions considered
    //choose_item_points    contains all the points used to construct hyperplanes(questions) (set C in the paper)
    //choose_item_set       contains all the hyperplanes(questions) which can be asked user
    //R_half_set            The utility range R
    std::vector<halfspace_set_t *> half_set_set;
    std::vector<int> considered_half_set;   //[i] shows the index in half_set_set
    std::vector<point_t *> choose_item_points;
    std::vector<choose_item *> choose_item_set;
    construct_halfspace_set(p_set_1, choose_item_points, half_set_set, considered_half_set);

    //index the index of the chosen hyperplane(question)
    //v1    the utility of point 1
    //v2    the utility of point 2
    int index = build_choose_item_table(half_set_set, choose_item_points, choose_item_set);
    point_t* p1 = choose_item_set[index]->hyper->point1;
    point_t* p2 = choose_item_set[index]->hyper->point2;

    //initial
    halfspace_set_t *R_half_set = R_initial(dim);
    get_extreme_pts_refine_halfspaces_alg1(R_half_set);
    halfspace_t *hy;
    int numOfQuestion = 0;
    point_t* point_result = NULL;
    while (considered_half_set.size() > 1 && point_result== NULL)
    {
        numOfQuestion++;
        point_t* user_choice = user_rand_err(u,p1,p2,theta);
        if (user_choice == p1)
        {
            hy = alloc_halfspace(choose_item_set[index]->hyper->point2, choose_item_set[index]->hyper->point1, 0, true);
            index = modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, true);
        }
        else
        {
            hy = alloc_halfspace(choose_item_set[index]->hyper->point1, choose_item_set[index]->hyper->point2, 0, true);
            index = modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, false);
        }
        p1 = choose_item_set[index]->hyper->point1;
        p2 = choose_item_set[index]->hyper->point2;

        //Find whether there exist point which is the topk point w.r.t any u in R
        R_half_set->halfspaces.push_back(hy);
        get_extreme_pts_refine_halfspaces_alg1(R_half_set);
        std::vector<point_t *> top_current;
        point_result = NULL;
        bool check_result = find_possible_topk(p_set, R_half_set, k, top_current);
        if (check_result)
        {
            point_result = check_possible_topk(p_set, R_half_set, k, top_current);
        }
    }

    ////////////////////////////////////////
    std::vector<std::pair<int,double>> util_scores; //key is pos, value is utility score
    for(int i=0;i<p_set_1.size();i++){
        util_scores.push_back(make_pair(i,dot_prod(R_half_set->in_center, p_set_1[i])));
    }
    std::sort(util_scores.begin(),util_scores.end(),[] (std::pair<int,double> const &a, std::pair<int,double> const &b){
                                                    return a.second>b.second;});

    std::vector<point_t *> new_p_set;
    double beta = 4;
    int new_size = floor(beta*p_set_1.size() * theta);
    for(int i = 0; i < new_size; i++){
        new_p_set.push_back(p_set_1[util_scores[i].first]);
    }
    
    if(new_size>1){    // if new_size <=1, simply return point_result
        point_result = NULL;
        release_halfspace_set(R_half_set);
        half_set_set.clear();
        considered_half_set.clear();
        choose_item_points.clear();
        choose_item_set.clear();    

        construct_halfspace_set(new_p_set, choose_item_points, half_set_set, considered_half_set);

        index = build_choose_item_table(half_set_set, choose_item_points, choose_item_set);
        p1 = choose_item_set[index]->hyper->point1;
        p2 = choose_item_set[index]->hyper->point2;

        //initialization
        R_half_set = R_initial(dim);
        get_extreme_pts_refine_halfspaces_alg1(R_half_set);
    }
    while (considered_half_set.size() > 1 && point_result== NULL)
    {
        numOfQuestion++;
        point_t* user_choice = user_rand_err(u,p1,p2,theta);
        if (user_choice == p1)
        {
            hy = alloc_halfspace(choose_item_set[index]->hyper->point2, choose_item_set[index]->hyper->point1, 0, true);
            index = modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, true);
        }
        else
        {
            hy = alloc_halfspace(choose_item_set[index]->hyper->point1, choose_item_set[index]->hyper->point2, 0, true);
            index = modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, false);
        }
        p1 = choose_item_set[index]->hyper->point1;
        p2 = choose_item_set[index]->hyper->point2;

        //Find whether there exist point which is the topk point w.r.t any u in R
        R_half_set->halfspaces.push_back(hy);
        get_extreme_pts_refine_halfspaces_alg1(R_half_set);
        std::vector<point_t *> top_current;
        point_result = NULL;
        bool check_result = find_possible_topk(p_set, R_half_set, k, top_current);
        if (check_result)
        {
            point_result = check_possible_topk(p_set, R_half_set, k, top_current);
        }
    }


    point_result = (point_result!=NULL)?point_result:half_set_set[considered_half_set[0]]->represent_point[0];
    printf("|%30s |%10d |%10s |\n", "STMD", numOfQuestion, "--");
    printf("|%30s |%10s |%10d |\n", "Point", "--", point_result->id);
    printf("---------------------------------------------------------\n");
    rr_ratio = dot_prod(u, point_result)/top_1_score;
    top_1_found= (rr_ratio>=1);
    return numOfQuestion;

}