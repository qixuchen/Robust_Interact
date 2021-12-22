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

int checking_varyk(point_t* p1, point_t* p2, int k, double skip_rate, int &num_questions, double * max, double * min){
    int p1_count=1, p2_count=0; //p1_count set to 1 since it is preferred in the first round
    point_t *result_this_round;
    int num_asked=0;
    int dim = p1->dim;
    
    if((double) rand()/RAND_MAX < skip_rate){ //skip checking with prob skip_rate
        return 1;
    }

    point_t *scaled_p1 = alloc_point(dim), *scaled_p2 = alloc_point(dim);

    while (p1_count<=k/2 && p2_count<=k/2 && num_asked<k){
        // printf("checking \n");
        double scale = (double)(90 + (rand()%10)) / 100; //generate a random number from 0.91-0.99
        for(int i = 0; i < dim; i++){
            if(i==0 || i==3){ // a small value is preferred
                scaled_p1->coord[i] = (1 - scale * p1->coord[i]) * (max[i]-min[i]) + min[i];
                scaled_p2->coord[i] = (1 - scale * p2->coord[i]) * (max[i]-min[i]) + min[i];
            }
            else{ // a larger value is preferred
                scaled_p1->coord[i] = scale * p1->coord[i] * (max[i]-min[i]) + min[i];
                scaled_p2->coord[i] = scale * p2->coord[i] * (max[i]-min[i]) + min[i];
            }
        }
        
        result_this_round = (show_to_user(scaled_p1, scaled_p2)==1) ? p1: p2;
        num_questions++;
        if(result_this_round==p1){
            p1_count++;
        }
        else{
            p2_count++;
        }
        num_asked++;

        // update total num of checkings
        num_checking++;
    }

    release_point(scaled_p1);
    release_point(scaled_p2);

    // update number of wrong interactions
    if(p1_count>k/2){
        num_wrong_checking+=p2_count;
    }
    else{
        num_wrong_checking+=p1_count;
    }

    return p1_count>k/2 ? 1 : 2;
}