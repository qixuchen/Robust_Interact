#include "HDPI.h"
#include <sys/time.h>

/**
 * @brief Asking user question and return one of the top-k point
 *        For find the point which is the top-k point w.r.t any u in R, we search the whole set
 *        Find the top-1 point by sampling
 * @param p_set 		 The original dataset
 * @param u 			 The linear function
 */
point_t* HDPI_sampling(FILE *wPtr, std::vector<point_t *> p_set, point_set_t *P0, int &questions)
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
            printf("\n--------------------------------------------------------\n");
            printf("%s %10d \n", "No. of questions asked:", numOfQuestion);
            printf("--------------------------------------------------------\n");
            printf("Recommended cars:\n");
            printf("--------------------------------------------------------\n");
            printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
            printf("---------------------------------------------------------\n");
            printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[p_result->id]->coord[0], P0->points[p_result->id]->coord[1],
                       P0->points[p_result->id]->coord[2], P0->points[p_result->id]->coord[3]);
            printf("---------------------------------------------------------\n");
            printf("Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate \n"
                   "how bored you feel when you are asked with %d questions for this round \n"
                   "in order to obtain one of your 20 most favorite cars (Note: 10 denotes\n"
                   "that you feel the most bored and 1 denotes that you feel the least bored.)\n"
                   "You answer: ", numOfQuestion);
            int sat = 0;
            while(sat < 1 || sat > 10)
            {
                scanf("%d", &sat);
            }
            printf("\n\n");


            fprintf(wPtr, "--------------------------------------------------------\n");
            fprintf(wPtr, "|%30s |%10d |\n", "HDPI_sampling", numOfQuestion);
            fprintf(wPtr, "--------------------------------------------------------\n");
            fprintf(wPtr, "Recommended cars:\n");
            fprintf(wPtr, "--------------------------------------------------------\n");
            fprintf(wPtr, "|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
            fprintf(wPtr,"---------------------------------------------------------\n");
            fprintf(wPtr,"|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[p_result->id]->coord[0], P0->points[p_result->id]->coord[1],
                       P0->points[p_result->id]->coord[2], P0->points[p_result->id]->coord[3]);
            fprintf(wPtr, "---------------------------------------------------------\n");
            fprintf(wPtr, "Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate \n"
                          "how bored you feel when you are asked with %d questions for this round \n"
                          "in order to obtain one of your 20 most favorite cars (Note: 10 denotes\n"
                          "that you feel the most bored and 1 denotes that you feel the least bored.) %d\n\n\n", numOfQuestion, sat);
            questions = numOfQuestion;
            return p_result;
        }
        if(considered_half_set.size() > 1)
            opt = show_to_user(P0, choose_item_set[index]->hyper->point1->id, choose_item_set[index]->hyper->point2->id);
    }

    printf("\n--------------------------------------------------------\n");
    printf("%s %10d \n", "No. of questions asked:", numOfQuestion);
    printf("--------------------------------------------------------\n");
    printf("Recommended cars:\n");
    printf("--------------------------------------------------------\n");
    printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    printf("---------------------------------------------------------\n");
    int r_id = half_set_set[considered_half_set[0]]->represent_point[0]->id;
    printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[r_id]->coord[0], P0->points[r_id]->coord[1],
           P0->points[r_id]->coord[2], P0->points[r_id]->coord[3]);
    printf("---------------------------------------------------------\n");
    printf("Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate \n"
           "how bored you feel when you are asked with %d questions for this round \n"
           "in order to obtain one of your 20 most favorite cars (Note: 10 denotes\n"
           "that you feel the most bored and 1 denotes that you feel the least bored.)\n"
           "You answer: ", numOfQuestion);
    int sat = 0;
    while(sat < 1 || sat > 10)
    {
        scanf("%d", &sat);
    }
    printf("\n\n");


    fprintf(wPtr, "--------------------------------------------------------\n");
    fprintf(wPtr, "|%30s |%10d |\n", "HDPI_sampling", numOfQuestion);
    fprintf(wPtr, "--------------------------------------------------------\n");
    fprintf(wPtr, "Recommended cars:\n");
    fprintf(wPtr, "--------------------------------------------------------\n");
    fprintf(wPtr, "|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    fprintf(wPtr,"---------------------------------------------------------\n");
    r_id = half_set_set[considered_half_set[0]]->represent_point[0]->id;
    fprintf(wPtr, "|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[r_id]->coord[0], P0->points[r_id]->coord[1],
           P0->points[r_id]->coord[2], P0->points[r_id]->coord[3]);
    fprintf(wPtr, "---------------------------------------------------------\n");
    fprintf(wPtr, "Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate \n"
                  "how bored you feel when you are asked with %d questions for this round \n"
                  "in order to obtain one of your 20 most favorite cars (Note: 10 denotes\n"
                  "that you feel the most bored and 1 denotes that you feel the least bored.) %d\n\n\n", numOfQuestion, sat);
    questions = numOfQuestion;
    return half_set_set[considered_half_set[0]]->represent_point[0];
}

/**
 * @brief Asking user question and return one of the top-k point
 *        For find the point which is the top-k point w.r.t any u in R, we search the whole set
 *        Find the top-1 point accurately convex Hull
 * @param p_set 		 The original dataset
 * @param u 			 The linear function
 */
point_t* HDPI_accurate(FILE *wPtr, std::vector<point_t *> p_set, point_set_t *P0, int &questions)
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
            printf("\n--------------------------------------------------------\n");
            printf("%s %10d \n", "No. of questions asked:", numOfQuestion);
            printf("--------------------------------------------------------\n");
            printf("Recommended cars:\n");
            printf("--------------------------------------------------------\n");
            printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
            printf("---------------------------------------------------------\n");
            printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[p_result->id]->coord[0], P0->points[p_result->id]->coord[1],
                       P0->points[p_result->id]->coord[2], P0->points[p_result->id]->coord[3]);
            printf("---------------------------------------------------------\n");
            printf("Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate \n"
                   "how bored you feel when you are asked with %d questions for this round \n"
                   "in order to obtain one of your 20 most favorite cars (Note: 10 denotes\n"
                   "that you feel the most bored and 1 denotes that you feel the least bored.)\n"
                   "You answer: ", numOfQuestion);
            int sat = 0;
            while(sat < 1 || sat > 10)
            {
                scanf("%d", &sat);
            }
            printf("\n\n");


            fprintf(wPtr, "--------------------------------------------------------\n");
            fprintf(wPtr, "|%30s |%10d |\n", "HDPI_accurate", numOfQuestion);
            fprintf(wPtr, "--------------------------------------------------------\n");
            fprintf(wPtr, "Recommended cars:\n");
            fprintf(wPtr, "--------------------------------------------------------\n");
            fprintf(wPtr, "|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
            fprintf(wPtr, "---------------------------------------------------------\n");
            fprintf(wPtr, "|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[p_result->id]->coord[0], P0->points[p_result->id]->coord[1],
                       P0->points[p_result->id]->coord[2], P0->points[p_result->id]->coord[3]);
            fprintf(wPtr, "---------------------------------------------------------\n");
            fprintf(wPtr, "Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate \n"
                          "how bored you feel when you are asked with %d questions for this round \n"
                          "in order to obtain one of your 20 most favorite cars (Note: 10 denotes\n"
                          "that you feel the most bored and 1 denotes that you feel the least bored.) %d\n\n\n", numOfQuestion, sat);
            questions = numOfQuestion;
            return p_result;
        }
        if(considered_half_set.size() > 1)
            opt = show_to_user(P0, choose_item_set[index]->hyper->point1->id, choose_item_set[index]->hyper->point2->id);
    }

    printf("\n--------------------------------------------------------\n");
    printf("%s %10d \n", "No. of questions asked:", numOfQuestion);
    printf("--------------------------------------------------------\n");
    printf("Recommended cars:\n");
    printf("--------------------------------------------------------\n");
    printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    printf("---------------------------------------------------------\n");
    int r_id = half_set_set[considered_half_set[0]]->represent_point[0]->id;
    printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[r_id]->coord[0], P0->points[r_id]->coord[1],
           P0->points[r_id]->coord[2], P0->points[r_id]->coord[3]);
    printf("---------------------------------------------------------\n");
    printf("Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate \n"
           "how bored you feel when you are asked with %d questions for this round \n"
           "in order to obtain one of your 20 most favorite cars (Note: 10 denotes\n"
           "that you feel the most bored and 1 denotes that you feel the least bored.)\n"
           "You answer: ", numOfQuestion);
    int sat = 0;
    while(sat < 1 || sat > 10)
    {
        scanf("%d", &sat);
    }
    printf("\n\n");


    fprintf(wPtr, "--------------------------------------------------------\n");
    fprintf(wPtr, "|%30s |%10d |\n", "HDPI_accurate", numOfQuestion);
    fprintf(wPtr, "--------------------------------------------------------\n");
    fprintf(wPtr, "Recommended cars:\n");
    fprintf(wPtr, "--------------------------------------------------------\n");
    fprintf(wPtr, "|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    fprintf(wPtr, "---------------------------------------------------------\n");
    r_id = half_set_set[considered_half_set[0]]->represent_point[0]->id;
    fprintf(wPtr,"|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[r_id]->coord[0], P0->points[r_id]->coord[1],
           P0->points[r_id]->coord[2], P0->points[r_id]->coord[3]);
    fprintf(wPtr, "---------------------------------------------------------\n");
    fprintf(wPtr, "Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate \n"
                  "how bored you feel when you are asked with %d questions for this round \n"
                  "in order to obtain one of your 20 most favorite cars (Note: 10 denotes\n"
                  "that you feel the most bored and 1 denotes that you feel the least bored.) %d\n\n\n", numOfQuestion, sat);
    questions = numOfQuestion;
    return half_set_set[considered_half_set[0]]->represent_point[0];
}