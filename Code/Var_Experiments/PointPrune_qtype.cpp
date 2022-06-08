#include "PointPrune.h"
#include "PointPrune_qtype.h"
#include <sys/time.h>
#include <utility>
#include <cmath>
#include <algorithm>


bool sort_by_descend_score(std::pair<int, double> i, std::pair<int, double> j){
    return i.second > j.second;
}


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

bool choose_k_best_item(std::vector<choose_item*> choose_item_set, std::vector<point_t *> &chosen_point_set, int k){
    
    std::vector< std::pair<int,double> > chosen_hyper_idx_score;

    for(int i=0; i<k; i++){ //initialize idx : score pair
        chosen_hyper_idx_score.push_back(std::make_pair(-1,-1000));
    }



    int item_count = 0;                         //the index for scanning the choose_item_set
    int ES_h = -1000;                           //Even score

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

    // After find out k best hyperplanes, we return k points associated to them
    // first, put the two points related to the best hyperplane in the return set
    int hyper_idx = chosen_hyper_idx_score[0].first;
    chosen_point_set.push_back(choose_item_set[hyper_idx]->hyper->point1);
    chosen_point_set.push_back(choose_item_set[hyper_idx]->hyper->point2);
    
    point_t *cur_point = NULL;
    bool same_point = false;

    for(int i=1; i<chosen_hyper_idx_score.size(); i++){
        hyper_idx = chosen_hyper_idx_score[i].first;
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

        point_result = NULL;
        //index: the index of the chosen hyperplane(question)
        //p1:    the first point
        //p2:    the second point
        int index = choose_best_item(choose_item_set);

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

            std::vector<point_t *> chosen_point_set;

            choose_k_best_item(choose_item_set, chosen_point_set, point_num);

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

        if(iter_num==1){
            i1_p1 += num_questions-cur_quest_num;
        }
        else if(iter_num==2){
            i2_p1 += num_questions-cur_quest_num;
        }
        else if(iter_num==3){
            i3_p1 += num_questions-cur_quest_num;
        }
        //=================================================================================================================================
        //End of phase 1


        //start of phase 2
        //==========================================================================================================================================

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

