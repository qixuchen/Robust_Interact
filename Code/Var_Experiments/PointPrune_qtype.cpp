#include "PointPrune.h"
#include "PointPrune_qtype.h"
#include <sys/time.h>
#include <utility>
#include <cmath>
#include <algorithm>


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





int PointPrune_morethan2points(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int point_num)
{
    int k = 1;

    //reset statistics
    num_questions=0;
    num_wrong_answer=0;
    crit_wrong_answer=0;

    
    int iter_num = 0;
    i1_p1 = 0;
    i1_p2 = 0;
    i2_p1 = 0;
    i2_p2 = 0;
    i3_p1 = 0;
    i3_p2 = 0;

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
    std::vector<halfspace_set_t *> half_set_set, half_set_set_cp;
    std::vector<int> considered_half_set, considered_half_set_cp;   //[i] shows the index in half_set_set
    std::vector<point_t *> choose_item_points, choose_item_points_cp;
    std::vector<choose_item *> choose_item_set,  choose_item_set_cp;
    std::vector<halfspace_t*> selected_halfspaces;
    halfspace_set_t *R_half_set_cp = R_initial(dim);
    halfspace_set_t *R_half_set = R_initial(dim);
    get_extreme_pts_refine_halfspaces_alg1(R_half_set_cp);
    get_extreme_pts_refine_halfspaces_alg1(R_half_set);

    construct_halfspace_set_R(p_set_1,R_half_set, choose_item_points, half_set_set, considered_half_set);
    build_choose_item_table(half_set_set, choose_item_points, choose_item_set);
    

    construct_halfspace_set_R(p_set_1,R_half_set_cp, choose_item_points_cp, half_set_set_cp, considered_half_set_cp);
    build_choose_item_table(half_set_set_cp, choose_item_points_cp, choose_item_set_cp);

    point_t* point_result = NULL, *true_point_result = NULL;
    halfspace_t *hy=NULL, *hy_cp=NULL;
    bool encounter_err = false, end_premature=false;

    while(true_point_result==NULL){
        iter_num++;
        int cur_quest_num = num_questions;

        point_t *p1, *p2, *user_choice;


        // //start of phase 1
        // //==========================================================================================================================================
        point_result = NULL;
        while (point_result==NULL)
        {
            //IMPORTANT : check this in case of counting more question than necessary!!
            num_questions++;
            std::vector<point_t *> displayed_point_set;
            choose_k_best_item(choose_item_set, displayed_point_set, point_num);
            int idx = user_rand_err_k_points(u, displayed_point_set, theta);
            point_t *p_chosen = displayed_point_set[idx];
            //displayed_point_set.erase(displayed_point_set.begin() + idx);

            for(int j = 0; j < displayed_point_set.size(); j++){
                hyperplane_t *hp = alloc_hyperplane(p_chosen, displayed_point_set[j], 0);
                int index = check_hyperplane_validity(choose_item_set, hp);
                if(index < 0){
                    continue;
                }
                
                hy = alloc_halfspace(displayed_point_set[j], p_chosen, 0, true);
                if(is_same_point(p_chosen, choose_item_set[index]->hyper->point1)){
                    modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, true);
                }
                else if(is_same_point(p_chosen, choose_item_set[index]->hyper->point2)){
                    modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, false);
                }
                else{
                    printf("UNEXPECTED\n");
                }
                R_half_set->halfspaces.push_back(hy);
                halfspace_t *hy2 = deepcopy_halfspace(hy);
                selected_halfspaces.push_back(hy2);

                get_extreme_pts_refine_halfspaces_alg1(R_half_set);
            }

            //Find whether there exist point which is the topk point w.r.t any u in R
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

        cur_quest_num = num_questions;

        encounter_err = false, end_premature=false;
        while(true_point_result==NULL && selected_halfspaces.size()>0){
            // IMPORTANT: The order of point recorded in choose_item does not imply user preference
            // The user preference is indicated in selected_halfspaces: p2 > p1
            point_t *best_p1=NULL, *best_p2=NULL;
            double ratio=0;
            int best_index = find_best_hyperplane(choose_item_set_cp,selected_halfspaces, best_p1, best_p2, ratio);
            p1 = choose_item_set_cp[best_index]->hyper->point1;
            p2 = choose_item_set_cp[best_index]->hyper->point2;
            
            double skip_rate = 0.2;
            if(best_p1==p1){
                //user_choice = checking(u,p2,p1,theta,checknum);
                user_choice = checking_varyk(u,p2,p1,theta,checknum,skip_rate);
            }
            else{
                //user_choice = checking(u,p1,p2,theta,checknum);
                user_choice = checking_varyk(u,p1,p2,theta,checknum, skip_rate);
            }
            //printf("ratio %10f\n",ratio);
            if(user_choice!=best_p2){
                encounter_err = true;
            }
            if(encounter_err==true && ratio<0.15){
                //printf("enc err\n");
                end_premature=true;
            }

            if (user_choice==p1)
            {
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

                // printf("end premature\n");
                break;
            }
        }

        
        if(iter_num==1){
            i1_p2 += num_questions-cur_quest_num;
        }
        else if(iter_num==2){
            i2_p2 += num_questions-cur_quest_num;
        }
        else if(iter_num==3){
            i3_p2 += num_questions-cur_quest_num;
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
        R_half_set = deepcopy_halfspace_set(R_half_set_cp);

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
        //=================================================================================================================================
        //End of phase 2

    }

    printf("|%30s |%10d |%10s |\n", "PointPrune_v2", num_questions, "--");
    printf("|%30s |%10s |%10d |\n", "Point", "--", true_point_result->id);
    printf("---------------------------------------------------------\n");
    // printf("# of wrong answers:%d\n",num_wrong_answer);
    // printf("# of critical wrong answers:%d\n",crit_wrong_answer);
    // printf("regret ratio: %10f \n", dot_prod(u, true_point_result)/top_1_score);
    rr_ratio = dot_prod(u, true_point_result)/top_1_score;
    top_1_found= (rr_ratio>=1);
    return num_questions;
    
}





int PointPrune_desired_undesired(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int point_num)
{
    int k = 1;

    //reset statistics
    num_questions=0;
    num_wrong_answer=0;
    crit_wrong_answer=0;

    
    int iter_num = 0;
    i1_p1 = 0;
    i1_p2 = 0;
    i2_p1 = 0;
    i2_p2 = 0;
    i3_p1 = 0;
    i3_p2 = 0;

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
    std::vector<halfspace_set_t *> half_set_set, half_set_set_cp;
    std::vector<int> considered_half_set, considered_half_set_cp;   //[i] shows the index in half_set_set
    std::vector<point_t *> choose_item_points, choose_item_points_cp;
    std::vector<choose_item *> choose_item_set,  choose_item_set_cp;
    std::vector<halfspace_t*> selected_halfspaces;
    halfspace_set_t *R_half_set_cp = R_initial(dim);
    halfspace_set_t *R_half_set = R_initial(dim);
    get_extreme_pts_refine_halfspaces_alg1(R_half_set_cp);
    get_extreme_pts_refine_halfspaces_alg1(R_half_set);

    construct_halfspace_set_R(p_set_1,R_half_set, choose_item_points, half_set_set, considered_half_set);
    build_choose_item_table(half_set_set, choose_item_points, choose_item_set);
    

    construct_halfspace_set_R(p_set_1,R_half_set_cp, choose_item_points_cp, half_set_set_cp, considered_half_set_cp);
    build_choose_item_table(half_set_set_cp, choose_item_points_cp, choose_item_set_cp);

    point_t* point_result = NULL, *true_point_result = NULL;
    halfspace_t *hy=NULL, *hy_cp=NULL;
    bool encounter_err = false, end_premature=false;

    while(true_point_result==NULL){
        iter_num++;
        int cur_quest_num = num_questions;

        point_t *p1, *p2, *user_choice;


        // //start of phase 1
        // //==========================================================================================================================================
        point_result = NULL;
        while (point_result==NULL)
        {
            //IMPORTANT : check this in case of counting more question than necessary!!
            num_questions++;
            std::vector<point_t *> displayed_point_set;
            choose_k_best_item(choose_item_set, displayed_point_set, point_num);
            std::vector<point_t *> desired, undesired;
            //user_choose_desired_undesired(u, displayed_point_set, desired, undesired);
            user_rand_err_choose_desired_undesired(u, displayed_point_set, desired, undesired, theta);

            //displayed_point_set.erase(displayed_point_set.begin() + idx);
            for(int i = 0; i < desired.size(); i++){
                point_t *p1 = desired[i];
                for(int j = 0; j < undesired.size(); j++){
                    point_t *p2 = undesired[j];
                    hyperplane_t *hp = alloc_hyperplane(p1, p2, 0);
                    int index = check_hyperplane_validity(choose_item_set, hp);
                    if(index < 0){
                        continue;
                    }
                    hy = alloc_halfspace(p2, p1, 0, true);
                    if(is_same_point(p1, choose_item_set[index]->hyper->point1)){
                        modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, true);
                    }
                    else if(is_same_point(p1, choose_item_set[index]->hyper->point2)){
                        modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, false);
                    }
                    else{
                        printf("UNEXPECTED\n");
                    }
                    R_half_set->halfspaces.push_back(hy);
                    halfspace_t *hy2 = deepcopy_halfspace(hy);
                    selected_halfspaces.push_back(hy2);

                    get_extreme_pts_refine_halfspaces_alg1(R_half_set);
                }
            }

            //Find whether there exist point which is the topk point w.r.t any u in R
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

        cur_quest_num = num_questions;

        encounter_err = false, end_premature=false;
        while(true_point_result==NULL && selected_halfspaces.size()>0){
            // IMPORTANT: The order of point recorded in choose_item does not imply user preference
            // The user preference is indicated in selected_halfspaces: p2 > p1
            point_t *best_p1=NULL, *best_p2=NULL;
            double ratio=0;
            int best_index = find_best_hyperplane(choose_item_set_cp,selected_halfspaces, best_p1, best_p2, ratio);
            p1 = choose_item_set_cp[best_index]->hyper->point1;
            p2 = choose_item_set_cp[best_index]->hyper->point2;
            
            double skip_rate = 0.2;
            if(best_p1==p1){
                //user_choice = checking(u,p2,p1,theta,checknum);
                user_choice = checking_varyk(u,p2,p1,theta,checknum,skip_rate);
            }
            else{
                //user_choice = checking(u,p1,p2,theta,checknum);
                user_choice = checking_varyk(u,p1,p2,theta,checknum, skip_rate);
            }
            //printf("ratio %10f\n",ratio);
            if(user_choice!=best_p2){
                encounter_err = true;
            }
            if(encounter_err==true && ratio<0.15){
                //printf("enc err\n");
                end_premature=true;
            }

            if (user_choice==p1)
            {
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

                // printf("end premature\n");
                break;
            }
        }

        
        if(iter_num==1){
            i1_p2 += num_questions-cur_quest_num;
        }
        else if(iter_num==2){
            i2_p2 += num_questions-cur_quest_num;
        }
        else if(iter_num==3){
            i3_p2 += num_questions-cur_quest_num;
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
        R_half_set = deepcopy_halfspace_set(R_half_set_cp);

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
        //=================================================================================================================================
        //End of phase 2

    }

    printf("|%30s |%10d |%10s |\n", "PointPrune_v2", num_questions, "--");
    printf("|%30s |%10s |%10d |\n", "Point", "--", true_point_result->id);
    printf("---------------------------------------------------------\n");
    // printf("# of wrong answers:%d\n",num_wrong_answer);
    // printf("# of critical wrong answers:%d\n",crit_wrong_answer);
    // printf("regret ratio: %10f \n", dot_prod(u, true_point_result)/top_1_score);
    rr_ratio = dot_prod(u, true_point_result)/top_1_score;
    top_1_found= (rr_ratio>=1);
    return num_questions;
    
}

