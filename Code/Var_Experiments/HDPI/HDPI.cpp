#include "HDPI.h"
#include <sys/time.h>

/**
 * @brief Asking user question and return one of the top-k point
 *        Find the top-1 point accurately convex Hull
 * @param p_set 		 The dataset
 * @param u 			 The linear function
 * @param k 			 The parameter
 */
int HDPI_accurate(std::vector<point_t *> p_set, point_t *u, double theta)
{
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
    while (considered_half_set.size() > 1)
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
        if (point_result!= NULL)
        {
            printf("|%30s |%10d |%10s |\n", "HD-PI_accurate", numOfQuestion, "--");
            printf("|%30s |%10s |%10d |\n", "Point", "--", point_result->id);
            printf("---------------------------------------------------------\n");
            rr_ratio = dot_prod(u, point_result)/top_1_score;
            top_1_found= (rr_ratio>=1);
            return numOfQuestion;
        }
    }
    printf("|%30s |%10d |%10s |\n", "HD-PI_accurate", numOfQuestion, "--");
    printf("|%30s |%10s |%10d |\n", "Point", "--",
           half_set_set[considered_half_set[0]]->represent_point[0]->id);
    printf("---------------------------------------------------------\n");
    rr_ratio = dot_prod(u, half_set_set[considered_half_set[0]]->represent_point[0])/top_1_score;
    top_1_found= (rr_ratio>=1);
    return numOfQuestion;
}




/**
 * @brief 
 * 
 * @param p_set 
 * @param u 
 * @param theta 
 * @param output_size 
 * @return int 
 */
int HDPI_accurate_containtop1(std::vector<point_t *> p_set, point_t *u, double theta, int output_size)
{
    int k = 1;
    num_questions=0;
    //p_set_1 contains the points which are not dominated by >=1 points
    //p_set_k contains the points which are not dominated by >=k points
    //p_top_1 contains the points which are the convex points
    std::vector<point_t *> p_set_1, top, returned_set;
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
    point_t* user_choice = user_rand_err(u,p1,p2,theta);

    //initial
    halfspace_set_t *R_half_set = R_initial(dim);
    get_extreme_pts_refine_halfspaces_alg1(R_half_set);
    halfspace_t *hy;
    int numOfQuestion = 0;
    point_t* point_result = NULL;
    while (returned_set.size()==0)
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
        get_extreme_pts_refine_halfspaces_alg1(R_half_set);
        std::vector<point_t *> top_current;
        point_result = NULL;
        bool check_result = find_possible_topk(p_set, R_half_set, k, top_current);

        if (check_result){
            point_result = check_possible_topk(p_set, R_half_set, k, top_current);
            if(point_result!=NULL){
                returned_set.push_back(point_result);
            }
        }
        else if(considered_half_set.size() <= output_size){
            for(int i=0; i<considered_half_set.size();i++){
                point_result=half_set_set[considered_half_set[i]]->represent_point[0];
                if(point_result!=NULL){
                    returned_set.push_back(point_result);
                }
            }
        }
    }
    printf("|%30s |%10d |%10s |\n", "HD-PI_accurate", numOfQuestion, "--");
    printf("|%30s |%10s |%10lu |\n", "Point", "--", returned_set.size());
    printf("---------------------------------------------------------\n");
    // printf("# of wrong answers:%d\n",num_wrong_answer);
    // printf("# of critical wrong answers:%d\n",crit_wrong_answer);
    // printf("regret ratio: %10f \n", dot_prod(u, true_point_result)/top_1_score);

    top_1_found=false;
    for(int i=0; i<returned_set.size();i++){
        rr_ratio = dot_prod(u, returned_set[i])/top_1_score;
        if(rr_ratio>=1){
            top_1_found=true;
            break;
        }
    }
    return num_questions;
}







/**
 * @brief Asking user question and return one of the top-k point
 *        Find the top-1 point accurately convex Hull
 * @param p_set 		 The dataset
 * @param u 			 The linear function
 * @param k 			 The parameter
 */
int HDPI_accurate_Alltopk(std::vector<point_t *> p_set, point_t *u, double theta, int k, std::vector<point_t*> ground_truth)
{
    //p_top1 contains the points which are the convex points
    std::vector<point_t *> p_convex, p_top1, p_refined, p_result;
    int dim = p_set[0]->dim;
    find_top1(p_set, p_convex);
    skyline_c(p_convex, p_top1);
    for(int i = 0; i < p_set.size(); ++i)
        p_refined.push_back(p_set[i]);

    /* half_set_set          contains all the partitions(intersection of halfspaces)
     * considered_half_set   contains all the possible partitions considered
     * choose_item_points    contains all the points used to construce hyperplanes(questions)
     * choose_item_set       contains all the hyperplanes(questions) which can be asked user
     * R_half_set            The utility range R
     */
    halfspace_set_t *R_half_set = R_initial(dim);
    std::vector<halfspace_set_t *> half_set_set;
    std::vector<int> considered_half_set;   //[i] shows the index in half_set_set
    std::vector<point_t *> choose_item_points;
    std::vector<choose_item *> choose_item_set;
    construct_halfspace_set_R(p_top1, R_half_set, choose_item_points, half_set_set, considered_half_set);

    //index the index of the chosen hyperplane(question)
    //v1    the utility of point 1
    //v2    the utility of point 2
    int index = build_choose_item_table(half_set_set, choose_item_points, choose_item_set);
    point_t* p1 = choose_item_set[index]->hyper->point1;
    point_t* p2 = choose_item_set[index]->hyper->point2;

    //initial
    halfspace_t *hy; int numOfQuestion = 0;
    while (p_result.size() < k)
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
        //Find whether there exist point which is the topk point w.r.t any u in R
        R_half_set->halfspaces.push_back(hy);
        get_extreme_pts_refine_halfspaces_alg1(R_half_set);
        std::vector<point_t *> top_current;
        while(considered_half_set.size() < 2 && p_result.size() < k)
        {
            p_convex.clear(); p_top1.clear();
            point_t *point_sure = half_set_set[considered_half_set[0]]->represent_point[0];
            p_result.push_back(point_sure);
            for(int i = 0; i < p_refined.size(); ++i)
            {
                if(is_same_point(p_refined[i], point_sure))
                {
                    p_refined.erase(p_refined.begin() + i);
                    break;
                }
            }
            find_top1(p_refined, p_convex);
            skyline_c(p_convex, p_top1);
            construct_halfspace_set_R(p_top1, R_half_set, choose_item_points, half_set_set, considered_half_set);
            index = build_choose_item_table(half_set_set, choose_item_points, choose_item_set);
        }
        if(p_result.size() < k)
        {
            p1 = choose_item_set[index]->hyper->point1;
            p2 = choose_item_set[index]->hyper->point2;
        }
    }
    printf("|%30s |%10d |%10s |\n", "HDPI-accurate", numOfQuestion, "--");
    for(int i = 0; i < p_result.size(); ++i)
        printf("|%30s |%10d |%10d |\n", "Point", i + 1, p_result[i]->id);
    printf("---------------------------------------------------------\n");

    if(p_result.size()!=ground_truth.size()) top_k_correct=0;
    else{
        top_k_correct=1;
        for(int i = 0; i < p_result.size(); i++){
            if(p_result[i]->id != ground_truth[i]->id){
                top_k_correct=0;
                break;
            }
        }
    }

    return numOfQuestion;
}


