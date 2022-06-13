#include "alg_one.h"
#include "../Others/user_mode.h"
#include <sys/time.h>

/**
 * @brief Ask user question and find one of the top-k point
 *        The input set is refined by skyband
 *        For find the point which is the top-k point w.r.t any u in R, we search the whole set
 * @param original_set 		The refined dataset
 * @param u 				The linear function
 * @param k 				The threshold top-k
 */
int Random_half(std::vector<point_t*> p_set, point_t* u, double theta)
{
    int k = 1;
    point_random(p_set);
    int dim = p_set[0]->dim, M = p_set.size(), numOfQuestion = 0;

    //initial: add the basic halfspace into R_hyperplane
    halfspace_set_t* R_half_set = R_initial(dim);
    std::vector<point_t*> top_current;
    std::vector<point_t*> current_point, current_use;
    point_t* point_result = NULL;
    current_use.push_back(p_set[0]);
    //compare: p_set contains all the points
    for(int i=1; i < M; i++)
    {
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
                    point_t* user_choice = user_rand_err(u, p_set[i], current_point[p_index], theta);
                    if(user_choice==p_set[i])
                    {
                        halfspace_t* half=alloc_halfspace(current_point[p_index], p_set[i], 0, true);
                        R_half_set->halfspaces.push_back(half);
                    }
                    else
                    {
                        halfspace_t* half=alloc_halfspace(p_set[i], current_point[p_index], 0, true);
                        R_half_set->halfspaces.push_back(half);
                    }
                    current_point.erase(current_point.begin() + p_index);
                    get_extreme_pts_refine_halfspaces_alg1(R_half_set);

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
                if(point_result != NULL){
                    break;
                }
            }
        }
        if(point_result != NULL){
            break;
        }
    }
    printf("|%30s |%10d |%10s |\n", "RH", numOfQuestion, "--");
    printf("|%30s |%10s |%10d |\n", "Point", "--", point_result->id);
    printf("---------------------------------------------------------\n");
    rr_ratio = dot_prod(u, point_result)/top_1_score;
    top_1_found= (rr_ratio>=1);
    return numOfQuestion;
}



int Random_half_Alltopk(std::vector<point_t*> p_set, point_t* u, double theta, int output_size, std::vector<point_t*> ground_truth)
{
    point_random(p_set);
    int dim = p_set[0]->dim, M = p_set.size(), numOfQuestion = 0;

    //initial: add the basic halfspace into R_hyperplane
    halfspace_set_t* R_half_set = R_initial(dim);
    std::vector<point_t*> top_current;
    std::vector<point_t*> current_point, current_use;
    point_t* point_result = NULL;
    current_use.push_back(p_set[0]);

    //compare: p_set contains all the points
    for(int i=1; i < M; i++)
    {
        bool same_exist=false, topk_found = false, topk_checked = false;
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
                    point_t* user_choice = user_rand_err(u, p_set[i], current_point[p_index], theta);
                    if(user_choice==p_set[i])
                    {
                        halfspace_t* half=alloc_halfspace(current_point[p_index], p_set[i], 0, true);
                        R_half_set->halfspaces.push_back(half);
                    }
                    else
                    {
                        halfspace_t* half=alloc_halfspace(p_set[i], current_point[p_index], 0, true);
                        R_half_set->halfspaces.push_back(half);
                    }
                    current_point.erase(current_point.begin() + p_index);
                    get_extreme_pts_refine_halfspaces_alg1(R_half_set);


                    //check if we can find top-k points in R_half_set
                    top_current.clear();
                    topk_found = find_possible_Alltopk(p_set, R_half_set, output_size, top_current);
                    topk_checked = check_possible_alltopk(p_set, R_half_set, output_size, top_current);
                    if(topk_found && topk_checked)
                    {
                       break;
                    }
                }
            }

        }
    }

    printf("|%30s |%10d |%10s |\n", "RH", numOfQuestion, "--");
    for(int i = 0; i < output_size; ++i)
        printf("|%30s |%10d |%10d |\n", "Point", i + 1, top_current[i]->id);
    printf("---------------------------------------------------------\n");

    top_k_correct=1;
    for(int i = 0; i < output_size; i++){
        if(top_current[i]->id != ground_truth[i]->id){
            top_k_correct=0;
            break;
        }
    }
    return numOfQuestion;
}