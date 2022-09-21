#include "HDPI.h"
#include <sys/time.h>


/**
 * @brief Asking user question and return one of the top-k points
 *        Find the top-1 point by sampling
 * @param p_set 		 The dataset
 * @param u 			 The linear function
 * @param k 			 The parameter
 */
int HDPI_sampling(std::vector<point_t *> p_set, point_t *u, double theta)
{
    int k = 1;
    //p_set_1 contains the points which are not dominated by >=1 points
    //p_set_k contains the points which are not dominated by >=k points
    //p_top_1 contains the points which are the convex points
    std::vector<point_t *> p_set_1, p_top_1;
    int dim = p_set[0]->dim;
    point_t *uk = alloc_point(dim);
    find_top1_sampling(p_set, p_top_1, uk, 0, 0);//use sampling method
    release_point(uk);

    /* half_set_set          contains all the partitions(intersection of halfspaces)
     * considered_half_set   contains all the possible partitions considered
     * choose_item_points    contains all the points used to construce hyperplanes(questions)
     * choose_item_set       contains all the hyperplanes(questions) which can be asked user
     * R_half_set            The utility range R
     */
    std::vector<halfspace_set_t *> half_set_set;
    std::vector<int> considered_half_set;   //[i] shows the index in half_set_set
    std::vector<point_t *> choose_item_points;
    std::vector<choose_item *> choose_item_set;
    construct_halfspace_set(p_top_1, choose_item_points, half_set_set, considered_half_set);

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
            printf("|%30s |%10d |%10s |\n", "HD-PI_sampling", numOfQuestion, "--");
            printf("|%30s |%10s |%10d |\n", "Point", "--", point_result->id);
            printf("---------------------------------------------------------\n");
            rr_ratio = dot_prod(u, point_result)/top_1_score;
            top_1_found= (rr_ratio>=1);
            return numOfQuestion;
        }
    }

    printf("|%30s |%10d |%10s |\n", "HD-PI_sampling", numOfQuestion, "--");
    printf("|%30s |%10s |%10d |\n", "Point", "--",
           half_set_set[considered_half_set[0]]->represent_point[0]->id);
    printf("---------------------------------------------------------\n");
    rr_ratio = dot_prod(u, half_set_set[considered_half_set[0]]->represent_point[0])/top_1_score;
    top_1_found= (rr_ratio>=1);
    return numOfQuestion;
}

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
 * @brief Run HDPI k times and return the major answer
 * @param p_set 		 The dataset
 * @param u 			 The linear function
 * @param theta          The user error rate
 * @param n_time 		 Number of repeating times
 */
int HDPI_Naive(std::vector<point_t *> p_set, point_t *u, double theta, int n_times){
    int total_question = 0;
    vector<int> answer_vec;
    for(int i = 0; i < n_times; i++){
        top_1_found = false;
        total_question += HDPI_accurate(p_set, u, theta);
        answer_vec.push_back(top_1_found==true ? 1 : 0);
    }

    int sum = 0;
    for(int i=0; i < answer_vec.size(); i++){
        sum += answer_vec[i];
    }

    if(sum > n_times/2){
        top_1_found = true;
    }
    else{
        top_1_found = answer_vec[rand() % answer_vec.size()] == 1 ? true : false;
    }
    
    return total_question;

}