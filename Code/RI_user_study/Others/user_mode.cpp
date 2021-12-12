#include "user_mode.h"



/**
 * @brief               Find out which point is more prefered using majority vote
 *                      vary k with skip_rate
 * 
 * @param p1            The first point (preferred in first answer)
 * @param p2            The second point
 * @param k             Number of rounds needed by the checking subroutine
 * @return point_t*     The checking result
 */

point_t* checking_varyk(point_t* p1, point_t* p2, int k, double skip_rate){
    int p1_count=1, p2_count=0; //p1_count set to 1 since it is preferred in the first round
    point_t *result_this_round;
    int num_asked=0;
    
    if((double) rand()/RAND_MAX < skip_rate){ //skip checking with prob skip_rate
        return p1;
    }

    while (p1_count<=k/2 && p2_count<=k/2 && num_asked<k){

        result_this_round = (show_to_user(p1,p2)==1) ? p1: p2;
        num_questions++;
        if(result_this_round==p1){
            p1_count++;
        }
        else{
            p2_count++;
        }
        num_asked++;
    }

    return p1_count>k/2 ? p1 : p2;
}