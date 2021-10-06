#include "ST2D.h"

int ST2D(std::vector<point_t *> p_convh, point_t *u, int k, double theta){
    int numOfQuestion=0;
    int size = p_convh.size();
    if(size==0){
        printf("Error: empty input.");
        return -1;
    }
    if(size==1){
        return 0;
    }
    int L=0, U=size-1;

    while(L<U){
        int search_size = U-L+1;
        int m = std::max(int(ceil(theta * search_size)), 1);
        int sL=L, sU=U;

        //perform binary search as if error-free
        while(sU-sL+1>m){
            int p_i = (sU+sL)/2;
            int p_j = p_i+1;
            if(user_rand_err(p_convh,u,p_i,p_j,theta)==p_i){
                sU = p_i;
            }
            else{
                sL = p_j;
            }
            numOfQuestion++;
        }

        // check the correctness of boundaries
        int bound_L = sL, bound_U=sU;
        if(sL != L){
            bound_L = checking(p_convh, u, sL-1, sL, theta, k);
            numOfQuestion+=k;
        }
        if(sU != U){
            bound_U = checking(p_convh, u, sU+1, sU, theta, k);
            numOfQuestion+=k;
        }
        
        //case 1: Both boundaries are correct
        if(bound_L == sL && bound_U==sU){
            L = sL;
            U = sU;
        }
        else if(bound_L == sL){ //case 2: left boundary is correct but right boundary is wrong
            L = sL;
            if(sU + m < U){
                bound_U = checking(p_convh, u, sU + m, sU + m + 1, theta, k);
                numOfQuestion+=k;
                if(bound_U==sU+m){
                    U = sU + m;
                }
            }
        }
        else{
            U = sU;
            if(sL - m > L){
                bound_L = checking(p_convh, u, sL - m, sL - m - 1, theta, k);
                numOfQuestion+=k;
                if(bound_L==sL-m){
                    L = sL - m;
                }
            }
        }
    }

    printf("|%30s |%10d |%10s |\n", "ST2D", numOfQuestion, "--");
    printf("|%30s |%10s |%10d |\n", "Point", "--", p_convh[L]->id);
    printf("---------------------------------------------------------\n");
    return numOfQuestion;
}