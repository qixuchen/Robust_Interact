#include "variant_func.h"



bool sort_by_descend_score(std::pair<int, double> i, std::pair<int, double> j){
    return i.second > j.second;
}

/**
 * @brief check if the returned set contains identical points.
 * 
 * @param point_set 
 * @return true 
 * @return false 
 */
bool contain_same_point(std::vector<point_t *> point_set){
    int size = point_set.size();
    for(int i=0; i<size; i++){
        for(int j = i + 1; j<size; j++){
            if(is_same_point(point_set[i], point_set[j])){
                return true;
            }
        }
    }
    return false;
}


/**
 * @brief user choose one point from the displayed points.
 * 
 * @param u             the utiliy vector
 * @param point_set     the set of displayed point
 * @return int     the idx of the point chosen by the user
 */
int user_choose_point(point_t* u, std::vector<point_t *> point_set){
    double max_util = -1;
    int max_ind = -1;
    int size = point_set.size();
    for(int i=0; i<size; i++){
        double cur_util = dot_prod(u, point_set[i]);
        if(cur_util > max_util){
            max_util = cur_util;
            max_ind = i;
        }
    }
    return max_ind;
}


/**
 * @brief user partition the points into desired group and undesired group
 * 
 * @param u             the utiliy vector
 * @param point_set     the set of displayed point
 * @param desired       the set containing all desried points
 * @param undesired     the set containing all undesried points
 * @return int     the idx of the point chosen by the user
 */
void user_choose_desired_undesired(point_t* u, std::vector<point_t *> point_set, std::vector<point_t *> & desired, std::vector<point_t *> & undesired){

    std::vector< std::pair<int,double> > point_idx_util_pair;
    int size = point_set.size();
    if(size < 2){
        printf("Only 1 point is displayed ! \n");
        return;
    }
    for(int i=0; i<size; i++){
        point_idx_util_pair.push_back(std::make_pair(i, dot_prod(u, point_set[i])));
    }
    std::sort(point_idx_util_pair.begin(), point_idx_util_pair.end(), sort_by_descend_score);

    int part_ind = rand() % (size-1) + 1;

    for(int i = 0; i < part_ind; i++){
        desired.push_back(point_set[point_idx_util_pair[i].first]);
    }

    for(int i = part_ind; i < size; i++){
        undesired.push_back(point_set[point_idx_util_pair[i].first]);
    }
}




/**
 * @brief   Check if hp is a valid hyperplan in choose_item_set.
 *          If yes, return the index in choose_item_set.
 *          If not, return -1
 * 
 * @param choose_item_set 
 * @param hp 
 * @return int 
 */
int check_hyperplane_validity(std::vector<choose_item*> choose_item_set, hyperplane_t *hp){
    int idx = -1;
    int size = choose_item_set.size();
    for(int i=0; i<size; i++){
        if(is_same_hyperplane(hp, choose_item_set[i]->hyper)){
            idx = i;
            break;
        }
    }
    return idx;
}


bool choose_k_best_item(std::vector<choose_item*> choose_item_set, std::vector<point_t *> &chosen_point_set, int k){
    
    std::vector< std::pair<int,double> > chosen_hyper_idx_score;

    for(int i=0; i<k; i++){ //initialize idx : score pair
        chosen_hyper_idx_score.push_back(std::make_pair(-1,-1000));
    }



    int item_count = 0;                         //the index for scanning the choose_item_set
    double ES_h = -1000;                           //Even score

    while(item_count < choose_item_set.size())
    {
        ES_h = chosen_hyper_idx_score[chosen_hyper_idx_score.size()-1].second;
        choose_item_t *c_item = choose_item_set[item_count];
        int M_positive = choose_item_set[item_count]->positive_side.size();
        int M_negative = choose_item_set[item_count]->negative_side.size();
        int M_intersect = choose_item_set[item_count]->intersect_case.size();
        if (M_negative + M_intersect == 0 || M_positive + M_intersect == 0)// || M_positive + M_negative == 0)
        {
            choose_item_set.erase(choose_item_set.begin() + item_count);
        }
        else
        {
            if (M_positive < M_negative)
            {
                double ES_i = M_positive - (Beta * M_intersect);
                if (ES_h < ES_i)
                {
                    chosen_hyper_idx_score.pop_back();
                    chosen_hyper_idx_score.push_back(std::make_pair(item_count, ES_i));
                    std::sort(chosen_hyper_idx_score.begin(), chosen_hyper_idx_score.end(), sort_by_descend_score);
                }
            }
            else
            {
                double ES_i = M_negative - (Beta * M_intersect);
                if (ES_h < ES_i)
                {
                    chosen_hyper_idx_score.pop_back();
                    chosen_hyper_idx_score.push_back(std::make_pair(item_count, ES_i));
                    std::sort(chosen_hyper_idx_score.begin(), chosen_hyper_idx_score.end(), sort_by_descend_score);
                }
            }
            item_count++;
        }
    }

    ES_h = chosen_hyper_idx_score[0].second; //debug purpose

    point_t *cur_point = NULL;
    bool same_point = false;
    for(int i=0; i<chosen_hyper_idx_score.size(); i++){
        int hyper_idx = chosen_hyper_idx_score[i].first;
        if(hyper_idx==-1) break;
        // point 1
        same_point = false;
        cur_point = choose_item_set[hyper_idx]->hyper->point1;
        for(int j=0; j<chosen_point_set.size(); j++){
            if(is_same_point(cur_point, chosen_point_set[j])){
                same_point = true;
                break;
            }
        }
        if(!same_point){
            chosen_point_set.push_back(cur_point);
        }
        if(chosen_point_set.size()>=k){
            break;
        }

        // point 2
        same_point = false;
        cur_point = choose_item_set[hyper_idx]->hyper->point2;
        for(int j=0; j<chosen_point_set.size(); j++){
            if(is_same_point(cur_point, chosen_point_set[j])){
                same_point = true;
                break;
            }
        }
        if(!same_point){
            chosen_point_set.push_back(cur_point);
        }
        if(chosen_point_set.size()>=k){
            break;
        }
    }

    return (chosen_point_set.size()==k);
}
