#include "user_mode.h"


bool sort_desc_score(std::pair<int, double> i, std::pair<int, double> j){
    return i.second > j.second;
}

/**
 * @brief The user makes random error and returns less favored point with probability err_rate
 * 
 * @param u             The utility vector
 * @param p1            Index of the first point
 * @param p2            Index of the second point
 * @param err_rate      The user's error rate
 * @return point_t* 
 */
int user_rand_err(const std::vector<point_t *> &p_set, point_t* u, int p1, int p2, double err_rate){
    num_questions++;
    if((double) rand()/RAND_MAX > err_rate){ //user answers correctly
        return (dot_prod(u,p_set[p1])>dot_prod(u,p_set[p2])? p1 : p2) ;
    }
    else{ //user answers wrongly
        num_wrong_answer++;
        return (dot_prod(u,p_set[p1])>dot_prod(u,p_set[p2])? p2 : p1) ;    
    }

}

/**
 * @brief The user makes random error and returns less favored point with probability err_rate
 * 
 * @param u             The utility vector
 * @param p1            The first point
 * @param p2            The second point
 * @param err_rate      The user's error rate
 * @return point_t* 
 */
point_t* user_rand_err(point_t* u, point_t* p1, point_t* p2, double err_rate){
    num_questions++;
    if((double) rand()/RAND_MAX > err_rate){ //user answers correctly
        return (dot_prod(u,p1)>dot_prod(u,p2)? p1 : p2) ;
    }
    else{ //user answers wrongly
        num_wrong_answer++;
        return (dot_prod(u,p1)>dot_prod(u,p2)? p2 : p1) ;    
    }

}


/**
 * @brief User choose his favorite point from a set of points. Choose the wrong point with prob. err_rate.
 * 
 * @param u 
 * @param point_set 
 * @param err_rate 
 * @return int          The index of the selected point(wrong with some prob.)
 */
int user_rand_err_k_points(point_t* u, std::vector<point_t *> point_set, double err_rate){
    num_questions++;
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
    if((double) rand()/RAND_MAX < err_rate){ //user answer wrongly
        int return_indx = rand()%(size-1);
        if(return_indx==max_ind){
            return_indx = size -1;
        }
        return return_indx;
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
void user_rand_err_choose_desired_undesired(point_t* u, std::vector<point_t *> point_set, std::vector<point_t *> & desired, std::vector<point_t *> & undesired, double err_rate){

    int size = point_set.size();
    if(size < 2){
        printf("Only 1 point is displayed ! \n");
        return;
    }

    num_questions++;
    std::vector< std::pair<int,double> > point_idx_util_pair;
    for(int i=0; i<size; i++){
        point_idx_util_pair.push_back(std::make_pair(i, dot_prod(u, point_set[i])));
    }
    std::sort(point_idx_util_pair.begin(), point_idx_util_pair.end(), sort_desc_score);

    int part_ind = rand() % (size-1) + 1;

    for(int i = 0; i < part_ind; i++){
        if((double) rand()/RAND_MAX < err_rate){ //user answer wrongly
            undesired.push_back(point_set[point_idx_util_pair[i].first]);
        }
        else{
            desired.push_back(point_set[point_idx_util_pair[i].first]);
        }
    }

    for(int i = part_ind; i < size; i++){
        if((double) rand()/RAND_MAX < err_rate){ //user answer wrongly
            desired.push_back(point_set[point_idx_util_pair[i].first]);
        }
        else{
            undesired.push_back(point_set[point_idx_util_pair[i].first]);
        }
    }
}


/**
 * @brief               Find out which point is more prefered using majority vote
 *                      p1 is preferred to p2 in user's first answer
 * 
 * @param u             The utility vector
 * @param p1            Index of the first point (preferred in first answer)
 * @param p2            Index of the second point
 * @param err_rate      The user's error rate
 * @param k             Number of rounds needed by the checking subroutine
 * @return point_t*     The checking result
 */

int checking(const std::vector<point_t *> &p_set, point_t* u, int p1, int p2, double err_rate, int k){
    int p1_count=1, p2_count=0; //p1_count set to 1 since it is preferred in the first round
    int result_this_round;
    int num_asked=1;
    while (p1_count<=k/2 && p2_count<=k/2 && num_asked<k){
        result_this_round = user_rand_err(p_set, u, p1, p2, err_rate); 
        if(result_this_round==p1){
            p1_count++;
        }
        else{
            p2_count++;
        }
        num_asked++;
    }

    return (p1_count>k/2 ? p1 : p2);
}


/**
 * @brief               Find out which point is more prefered using majority vote
 *                      p1 is preferred to p2 in user's first answer
 * 
 * @param u             The utility vector
 * @param p1            The first point (preferred in first answer)
 * @param p2            The second point
 * @param err_rate      The user's error rate
 * @param k             Number of rounds needed by the checking subroutine
 * @return point_t*     The checking result
 */

point_t* checking(point_t* u, point_t* p1, point_t* p2, double err_rate, int k){
    int p1_count=1, p2_count=0; //p1_count set to 1 since it is preferred in the first round
    point_t *result_this_round;
    int num_asked=1;
    while (p1_count<=k/2 && p2_count<=k/2 && num_asked<k){
        result_this_round = user_rand_err(u, p1, p2, err_rate); 
        if(result_this_round==p1){
            p1_count++;
        }
        else{
            p2_count++;
        }
        num_asked++;
    }

    point_t* final_res = p1_count>k/2 ? p1 : p2;
    point_t* true_res = dot_prod(u, p1) > dot_prod(u,p2)? p1: p2;
    if(final_res!=true_res){
        crit_wrong_answer++ ;
    }
    return final_res;
}



/**
 * @brief               Find out which point is more prefered using majority vote
 *                      vary k with skip_rate
 * 
 * @param u             The utility vector
 * @param p1            The first point (preferred in first answer)
 * @param p2            The second point
 * @param err_rate      The user's error rate
 * @param k             Number of rounds needed by the checking subroutine
 * @return point_t*     The checking result
 */

point_t* checking_varyk(point_t* u, point_t* p1, point_t* p2, double err_rate, int k, double skip_rate){
    int p1_count=1, p2_count=0; //p1_count set to 1 since it is preferred in the first round
    point_t *result_this_round;
    int num_asked=1;
    
    if((double) rand()/RAND_MAX < skip_rate){ //skip checking with prob skip_rate
        point_t* true_res = dot_prod(u, p1) > dot_prod(u,p2)? p1: p2;
        if(p1!=true_res){
            crit_wrong_answer++ ;
        }
        return p1;
    }

    while (p1_count<=k/2 && p2_count<=k/2 && num_asked<k){
        result_this_round = user_rand_err(u, p1, p2, err_rate); 
        if(result_this_round==p1){
            p1_count++;
        }
        else{
            p2_count++;
        }
        num_asked++;
    }

    point_t* final_res = p1_count>k/2 ? p1 : p2;
    point_t* true_res = dot_prod(u, p1) > dot_prod(u,p2)? p1: p2;
    if(final_res!=true_res){
        crit_wrong_answer++ ;
    }
    return final_res;
}