#include "./Others/used_function.h"
#include "./Others/user_mode.h"
#include "./HDPI/partition_func.h"

extern int num_questions;
extern int crit_wrong_answer;
extern int num_wrong_answer;
extern double top_1_score;
extern double rr_ratio;
extern int top_1_found;

extern int i1_p1;
extern int i1_p2;
extern int i2_p1;
extern int i2_p2;
extern int i3_p1;
extern int i3_p2;

/**
 * @brief           PointPrune, with some modifications (e.g., end prematurely) 
 */
int PointPrune_v2(std::vector<point_t *> p_set, point_t *u, int checknum, double theta);


int PointPrune_containTop1(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int output_size);


int PointPrune_Alltopk(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int k);


int PointPrune_Alltopk_acc(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int k);


int PointPrune_Alltopk_V3(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int output_size);

// int PointPrune_Alltopk(std::vector<point_t *> p_set, point_t *u, int checknum, double theta, int k)
// {


//     //reset statistics
//     num_questions=0;
//     num_wrong_answer=0;
//     crit_wrong_answer=0;

    
//     int iter_num = 0;
//     i1_p1 = 0;
//     i1_p2 = 0;
//     i2_p1 = 0;
//     i2_p2 = 0;
//     i3_p1 = 0;
//     i3_p2 = 0;

//     //p_set_1 contains the points which are not dominated by >=1 points
//     //p_set_k contains the points which are not dominated by >=k points
//     //p_top_1 contains the points which are the convex points
//     // top = p_convex
//     // p_set_1 = p_top1
//     std::vector<point_t *> p_set_1, top, p_refined, true_p_refined;
//     //onion(p_set, p_set_1, 1);
//     int dim = p_set[0]->dim;
//     find_top1(p_set, top);
//     skyline_c(top, p_set_1);
//     for(int i = 0; i < p_set.size(); ++i)
//         p_refined.push_back(p_set[i]);
//     true_p_refined = p_refined;

//     //half_set_set          contains all the partitions(intersection of halfspaces)
//     //considered_half_set   contains all the possible partitions considered
//     //choose_item_points    contains all the points used to construct hyperplanes(questions) (set C in the paper)
//     //choose_item_set       contains all the hyperplanes(questions) which can be asked user
//     //R_half_set            The utility range R
//     std::vector<halfspace_set_t *> half_set_set, half_set_set_cp;
//     std::vector<int> considered_half_set, considered_half_set_cp;   //[i] shows the index in half_set_set
//     std::vector<point_t *> choose_item_points, choose_item_points_cp;
//     std::vector<choose_item *> choose_item_set,  choose_item_set_cp;
//     std::vector<halfspace_t*> selected_halfspaces;
//     halfspace_set_t *R_half_set_cp = R_initial(dim);
//     halfspace_set_t *R_half_set = R_initial(dim);
//     get_extreme_pts_refine_halfspaces_alg1(R_half_set_cp);
//     get_extreme_pts_refine_halfspaces_alg1(R_half_set);
//     construct_halfspace_set_with_copy(p_set_1, choose_item_points, choose_item_points_cp, half_set_set, 
//                                     half_set_set_cp, considered_half_set, considered_half_set_cp);

//     build_choose_item_table(half_set_set, choose_item_points, choose_item_set);
//     for(int i =0 ; i<choose_item_set.size();i++){
//         choose_item * c_i=deepcopy_choose_item(choose_item_set[i]);
//         choose_item_set_cp.push_back(c_i);
//     }

//     std::vector<point_t *> p_result, true_p_result;
    
//     // point_t* point_result = NULL, *true_point_result = NULL;
//     halfspace_t *hy=NULL, *hy_cp=NULL;
//     bool encounter_err = false, end_premature=false;

//     while(true_p_result.size()<k){
//         iter_num++;
//         int cur_quest_num = num_questions;

//         //index: the index of the chosen hyperplane(question)
//         //p1:    the first point
//         //p2:    the second point
//         int index = choose_best_item(choose_item_set);

//         point_t* p1 = choose_item_set[index]->hyper->point1;
//         point_t* p2 = choose_item_set[index]->hyper->point2;
//         point_t* user_choice = user_rand_err(u,p1,p2,theta);



//         //start of phase 1 
//         //==========================================================================================================================================


//         while (p_result.size() < k)
//         {
//             if (user_choice==p1)
//             {
//                 hy = alloc_halfspace(p2, p1, 0, true);
//                 index = modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, true);
//             }
//             else
//             {
//                 hy = alloc_halfspace(p1, p2, 0, true);
//                 index = modify_choose_item_table(choose_item_set, half_set_set, considered_half_set, index, false);
//             }
//             p1 = choose_item_set[index]->hyper->point1;
//             p2 = choose_item_set[index]->hyper->point2;
//             user_choice = user_rand_err(u,p1,p2,theta);

//             //Find whether there exist point which is the topk point w.r.t any u in R
//             R_half_set->halfspaces.push_back(hy);
//             halfspace_t *hy2 = deepcopy_halfspace(hy);
//             selected_halfspaces.push_back(hy2);

//             get_extreme_pts_refine_halfspaces_alg1(R_half_set);
//             std::vector<point_t *> top_current;
//             // point_result = NULL;
//             bool check_result = find_possible_topk(p_set, R_half_set, k, top_current);
//             while(considered_half_set.size() < 2 && p_result.size() < k)
//             {
//                     // top = p_convex
//                     // p_set_1 = p_top1
//                 top.clear(); p_set_1.clear();
//                 point_t *point_sure = half_set_set[considered_half_set[0]]->represent_point[0];
//                 p_result.push_back(point_sure);
//                 for(int i = 0; i < p_refined.size(); ++i)
//                 {
//                     if(is_same_point(p_refined[i], point_sure))
//                     {
//                         p_refined.erase(p_refined.begin() + i);
//                         break;
//                     }
//                 }
//                 find_top1(p_refined, top);
//                 skyline_c(top, p_set_1);
//                 construct_halfspace_set(p_set_1, choose_item_points, half_set_set, considered_half_set);


//                 index = build_choose_item_table(half_set_set, choose_item_points, choose_item_set);
//             }
//             if(p_result.size() < k)
//             {
//                 p1 = choose_item_set[index]->hyper->point1;
//                 p2 = choose_item_set[index]->hyper->point2;
//                 user_choice = user_rand_err(u,p1,p2,theta);

//             }
//         }



//         // //top 1 determined, now top k
//         // std::vector<point_t *> top_k_current, point_pair;
//         // bool top_k_determined = checkTopkDetermined(p_set, R_half_set, output_size, top_k_current, point_pair);
//         // while(!top_k_determined){
//         //     point_t* p1 = point_pair[0];
//         //     point_t* p2 = point_pair[1];
//         //     point_t* user_choice = user_rand_err(u,p1,p2,theta);
//         //     if (user_choice==p1)
//         //     {
//         //         hy = alloc_halfspace(p2, p1, 0, true);
//         //     }
//         //     else
//         //     {
//         //         hy = alloc_halfspace(p1, p2, 0, true);
//         //     }

//         //     R_half_set->halfspaces.push_back(hy);
//         //     halfspace_t *hy2 = deepcopy_halfspace(hy);
//         //     selected_halfspaces.push_back(hy2);

//         //     get_extreme_pts_refine_halfspaces_alg1(R_half_set);

//         //     top_k_determined = checkTopkDetermined(p_set, R_half_set, output_size, top_k_current, point_pair);
//         // }


//         if(iter_num==1){
//             i1_p1 += num_questions-cur_quest_num;
//         }
//         else if(iter_num==2){
//             i2_p1 += num_questions-cur_quest_num;
//         }
//         else if(iter_num==3){
//             i3_p1 += num_questions-cur_quest_num;
//         }
//         //=================================================================================================================================
//         //End of phase 1


//         //start of phase 2
//         //==========================================================================================================================================

//         cur_quest_num = num_questions;

//         encounter_err = false, end_premature=false;
//         while(true_p_result.size()<k && selected_halfspaces.size()>0){
//             // IMPORTANT: The order of point recorded in choose_item does not imply user preference
//             // The user preference is indicated in selected_halfspaces: p2 > p1
//             point_t *best_p1=NULL, *best_p2=NULL;
//             double ratio=0;
//             int best_index = find_best_hyperplane(choose_item_set_cp,selected_halfspaces, best_p1, best_p2, ratio);
//             p1 = choose_item_set_cp[best_index]->hyper->point1;
//             p2 = choose_item_set_cp[best_index]->hyper->point2;
            
//             double skip_rate = 0.2;
//             if(best_p1==p1){
//                 //user_choice = checking(u,p2,p1,theta,checknum);
//                 user_choice = checking_varyk(u,p2,p1,theta,checknum,skip_rate);
//             }
//             else{
//                 //user_choice = checking(u,p1,p2,theta,checknum);
//                 user_choice = checking_varyk(u,p1,p2,theta,checknum, skip_rate);
//             }
//             //printf("ratio %10f\n",ratio);
//             if(user_choice!=best_p2){
//                 encounter_err = true;
//             }
//             if(encounter_err==true && ratio<0.15){
//                 //printf("enc err\n");
//                 end_premature=true;
//             }

//             if (user_choice==p1)
//             {
//                 hy_cp = alloc_halfspace(p2, p1, 0, true);
//                 best_index=modify_choose_item_table(choose_item_set_cp, half_set_set_cp, considered_half_set_cp, best_index, true);
//             }
//             else
//             {
//                 hy_cp = alloc_halfspace(p1, p2, 0, true);
//                 modify_choose_item_table(choose_item_set_cp, half_set_set_cp, considered_half_set_cp, best_index, false);
//             }

//             R_half_set_cp->halfspaces.push_back(hy_cp);
//             get_extreme_pts_refine_halfspaces_alg1(R_half_set_cp);

//             // IMPORTANT: Remove the halfspaces that are no longer helpful (lies on one side of R)
//             int m=0;
//             while(m<selected_halfspaces.size()){
//                 hyperplane_t *h = alloc_hyperplane(selected_halfspaces[m]->normal, selected_halfspaces[m]->offset);
//                 if(check_situation_accelerate(h,R_half_set_cp,0)!=0){
//                     selected_halfspaces.erase(selected_halfspaces.begin()+m);
//                 }
//                 else{
//                     m++;
//                 }
//             }

//             std::vector<point_t *> top_current;

//             while(considered_half_set_cp.size() < 2 && true_p_result.size() < k)
//             {
//                     // top = p_convex
//                     // p_set_1 = p_top1
//                 top.clear(); p_set_1.clear();
//                 point_t *point_sure = half_set_set_cp[considered_half_set_cp[0]]->represent_point[0];
//                 true_p_result.push_back(point_sure);
//                 for(int i = 0; i < true_p_refined.size(); ++i)
//                 {
//                     if(is_same_point(true_p_refined[i], point_sure))
//                     {
//                         true_p_refined.erase(true_p_refined.begin() + i);
//                         break;
//                     }
//                 }
//                 find_top1(true_p_refined, top);
//                 skyline_c(top, p_set_1);
//                 construct_halfspace_set(p_set_1, choose_item_points_cp, half_set_set_cp, considered_half_set_cp);


//                 index = build_choose_item_table(half_set_set_cp, choose_item_points_cp, choose_item_set_cp);
//             }

//             if(end_premature){

//                 // printf("end premature\n");
//                 break;
//             }
//         }

        
//         if(iter_num==1){
//             i1_p2 += num_questions-cur_quest_num;
//         }
//         else if(iter_num==2){
//             i2_p2 += num_questions-cur_quest_num;
//         }
//         else if(iter_num==3){
//             i3_p2 += num_questions-cur_quest_num;
//         }


//         if(true_p_result.size()>=k){
//             break;
//         }

//         //  re-initialize all data structures used in the inner loop with the record of the outer loop
//         //  std::vector<halfspace_set_t *> half_set_set
//         //  std::vector<int> considered_half_set
//         //  std::vector<point_t *> choose_item_points
//         //  std::vector<choose_item *> choose_item_set
//         //  std::vector<halfspace_t*> selected_halfspaces
//         //  halfspace_set_t *R_half_set


//         selected_halfspaces.clear();

//         release_halfspace_set(R_half_set);
//         R_half_set = deepcopy_halfspace_set(R_half_set_cp);

//         choose_item_points.clear();
//         for(int i=0; i<choose_item_points_cp.size();i++){
//             choose_item_points.push_back(choose_item_points_cp[i]);
//         }

//         considered_half_set.clear();
//         for(int i=0; i<considered_half_set_cp.size();i++){
//             considered_half_set.push_back(considered_half_set_cp[i]);
//         }
//         //////
//         half_set_set.clear();
//         // while(half_set_set.size()>0){
//         //     release_halfspace_set(half_set_set[0]);
//         //     half_set_set.erase(half_set_set.begin());
//         // }
//         // half_set_set.clear();
//         ////////
//         for(int i=0; i<half_set_set_cp.size();i++){
//             halfspace_set_t *hs = deepcopy_halfspace_set(half_set_set_cp[i]);
//             half_set_set.push_back(hs);
//         }

//         while(choose_item_set.size()>0){
//             release_choose_item(choose_item_set[0]);
//             choose_item_set.erase(choose_item_set.begin());
//         }
//         choose_item_set.clear();
//         for(int i =0 ; i<choose_item_set_cp.size();i++){
//             choose_item * c_i=deepcopy_choose_item(choose_item_set_cp[i]);
//             choose_item_set.push_back(c_i);
//         }

//         p_refined = true_p_refined;
//         p_result = true_p_result;
//         //=================================================================================================================================
//         //End of phase 2

//     }

//     printf("|%30s |%10d |%10s |\n", "PointPrune_v2", num_questions, "--");
//     printf("|%30s |%10s |%10d |\n", "Point", "--", true_p_result[0]->id);
//     printf("---------------------------------------------------------\n");
//     // printf("# of wrong answers:%d\n",num_wrong_answer);
//     // printf("# of critical wrong answers:%d\n",crit_wrong_answer);
//     // printf("regret ratio: %10f \n", dot_prod(u, true_point_result)/top_1_score);
//     rr_ratio = dot_prod(u, true_p_result[0])/top_1_score;
//     top_1_found= (rr_ratio>=1);
//     return num_questions;
    
// }