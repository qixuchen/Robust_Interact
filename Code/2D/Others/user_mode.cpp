#include "user_mode.h"




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