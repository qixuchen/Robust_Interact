#include "HDPI.h"
#include <sys/time.h>

/**
 * @brief Asking user question and return one of the top-k point
 *        For find the point which is the top-k point w.r.t any u in R, we search the whole set
 *        Find the top-1 point by sampling
 * @param p_set 		 The original dataset
 * @param u 			 The linear function
 */
int HDPI_sampling(FILE *wPtr, std::vector<point_t *> p_set, point_set_t *P0, int &questions, int alg_num)
{
    int k=1;
    //p_set_1 contains the points which are not dominated by >=1 points
    //p_set_k contains the points which are not dominated by >=k points
    //p_top_1 contains the points which are the possible top-1 point
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
    int opt = show_to_user(P0, choose_item_set[index]->hyper->point1->id, choose_item_set[index]->hyper->point2->id);

    //initial
    halfspace_set_t *R_half_set = R_initial(dim);
    get_extreme_pts_refine_halfspaces_alg1(R_half_set);
    halfspace_t *hy;
    int numOfQuestion = 0;
    while (considered_half_set.size() > 1 && numOfQuestion <=100)
    {
        numOfQuestion++;
        //printf("\nnumber_question%d index%d", numOfQuestion, index);
        //print_choose_item_situation(choose_item_set, index);
        if (opt == 1)
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
        point_t* p_result = NULL;
        bool check_result = find_possible_topk(p_set, R_half_set, k, top_current);
        //printf("question %d ext_num %lu top_size %lu \n", numOfQuestion, R_half_set->ext_pts.size(), top_current.size());
        if (check_result)
        {
            p_result = check_possible_topk(p_set, R_half_set, k, top_current);
        }
        //printf("question %d ext_num %lu top_size %lu \n", numOfQuestion, R_half_set->ext_pts.size(), top_current.size());
        if (p_result!=NULL)
        {
            print_result(wPtr, "HDPI_sampling", numOfQuestion, P0->points[p_result->id], alg_num);
            questions = numOfQuestion;
            return p_result->id;
        }
        if(considered_half_set.size() > 1)
            opt = show_to_user(P0, choose_item_set[index]->hyper->point1->id, choose_item_set[index]->hyper->point2->id);
    }

    point_t* p_result = half_set_set[considered_half_set[0]]->represent_point[0];
    print_result(wPtr, "HDPI_sampling", numOfQuestion, P0->points[p_result->id], alg_num);
    questions = numOfQuestion;
    return p_result->id; 
}

/**
 * @brief Asking user question and return one of the top-k point
 *        For find the point which is the top-k point w.r.t any u in R, we search the whole set
 *        Find the top-1 point accurately convex Hull
 * @param p_set 		 The original dataset
 * @param u 			 The linear function
 */
int HDPI_accurate(FILE *wPtr, std::vector<point_t *> p_set, point_set_t *P0, int &questions,int alg_num)
{
    int k=1;
    //p_set_1 contains the points which are not dominated by >=1 points
    //p_set_k contains the points which are not dominated by >=k points
    //p_top_1 contains the points which are the possible top-1 point
    std::vector<point_t *> p_set_1, top;
    //onion(p_set, p_set_1, 1);
    int dim = p_set[0]->dim;
    //find_top1_accurate(p_set_1, p_top_1);
    find_top1_qhull(p_set, top);
    skyline_c(top, p_set_1);
    //half_set_set          contains all the partitions(intersection of halfspaces)
    //considered_half_set   contains all the possible partitions considered
    //choose_item_points    contains all the points used to construct hyperplanes(questions)
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
    int opt = show_to_user(P0, choose_item_set[index]->hyper->point1->id, choose_item_set[index]->hyper->point2->id);

    //initial
    halfspace_set_t *R_half_set = R_initial(dim);
    get_extreme_pts_refine_halfspaces_alg1(R_half_set);
    halfspace_t *hy;
    int numOfQuestion = 0;
    while (considered_half_set.size() > 1 && numOfQuestion <=100)
    {
        numOfQuestion++;
        //printf("\nnumber_question%d index%d", numOfQuestion, index);
        //print_choose_item_situation(choose_item_set, index);
        if (opt == 1)
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
        point_t* p_result = NULL;
        bool check_result = find_possible_topk(p_set, R_half_set, k, top_current);
        //printf("question %d ext_num %lu top_size %lu \n", numOfQuestion, R_half_set->ext_pts.size(), top_current.size());
        if (check_result)
        {
            p_result = check_possible_topk(p_set, R_half_set, k, top_current);
        }
        //printf("question %d ext_num %lu top_size %lu \n", numOfQuestion, R_half_set->ext_pts.size(), top_current.size());
        if (p_result != NULL)
        {
            print_result(wPtr, "HDPI_accurate", numOfQuestion, P0->points[p_result->id], alg_num);
            questions = numOfQuestion;
            return p_result->id; 
        }
        if(considered_half_set.size() > 1)
            opt = show_to_user(P0, choose_item_set[index]->hyper->point1->id, choose_item_set[index]->hyper->point2->id);
    }

    point_t* p_result = half_set_set[considered_half_set[0]]->represent_point[0];
    print_result(wPtr, "HDPI_accurate", numOfQuestion, P0->points[p_result->id], alg_num);
    questions = numOfQuestion;
    return p_result->id; 
}