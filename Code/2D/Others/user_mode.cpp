#include "user_mode.h"




/**
 * @brief The user makes random error and returns less favored point with probability err_rate
 * 
 * @param u             The utility vector
 * @param p1            The first point
 * @param p2            The second point
 * @param err_rate      The user's error rate
 * @return point_t* 
 */
int user_rand_err(const std::vector<point_t *> &p_set, point_t* u, int p1, int p2, double err_rate){
    if((double) rand()/RAND_MAX > err_rate){ //user answers correctly
        return (dot_prod(u,p_set[p1])>dot_prod(u,p_set[p2])? p1 : p2) ;
    }
    else{ //user answers wrongly
        num_wrong_answer++;
        return (dot_prod(u,p_set[p1])>dot_prod(u,p_set[p2])? p2 : p1) ;    
    }

}

/**
 * @brief               Find out which point is more prefered using majority vote
 * 
 * @param u             The utility vector
 * @param p1            The index of the first point
 * @param p2            The index of the second point
 * @param err_rate      The user's error rate
 * @param k             Number of rounds needed by the checking subroutine
 * @return point_t*     The checking result
 */

int checking(const std::vector<point_t *> &p_set, point_t* u, int p1, int p2, double err_rate, int k){
    int p1_count=0, p2_count=0;
    int result_this_round;
    for(int i=0; i<k; i++){
        result_this_round = user_rand_err(p_set, u, p1, p2, err_rate); 
        if(result_this_round==p1){
            p1_count++;
        }
        else{
            p2_count++;
        }
    }
    return (p1_count>p2_count ? p1 : p2);
}