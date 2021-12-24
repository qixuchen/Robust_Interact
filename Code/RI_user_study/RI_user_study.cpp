#include "Others/data_utility.h"
#include "Others/data_struct.h"
#include "Others/read_write.h"
#include "Others/pruning.h"
#include "maxUtility.h"
#include "preferLearn.h"
#include "HDPI.h"
#include "RH.h"
#include "active_ranking.h"

#include "RI/PointPrune.h"
#include "RI/SamplePrune.h"
#include "RI/STMD.h"
#include "Others/qhull_build.h"
#include <vector>
#include <ctime>
#include <sys/time.h>




void enter_to_continue(){
    printf("\nPress enter to continue\n");

    char line[1024];
    fgets(line, 1024, stdin);

    char enter = 0;
    while (enter != '\n') { enter = getchar(); }
}

void create_final_list(int *returned_point, int r_size, std::vector<int> &final_list){
    final_list.push_back(returned_point[0]);
    for(int i=0; i<r_size; i++)
    {
        bool is_same=false;
        for(int j=0; j<final_list.size(); j++){
            if(returned_point[i] == final_list[j]){
                is_same = true;
                break;
            }
            
        }
        if(!is_same){
            final_list.push_back(returned_point[i]);
        }
    }
}

std::vector<point_t *> random_choose(point_set_t *p_set)
{
    std::vector<point_t *> p;
    int count = 0, index = 0, dim = p_set->points[0]->dim;
    while(count < 1000)
    {
        index = rand() % p_set->numberOfPoints;
        if(1000 < p_set->points[index]->coord[0] && p_set->points[index]->coord[0] < 100000
            && 2001 < p_set->points[index]->coord[1] && p_set->points[index]->coord[1]< 2017
            && 50 < p_set->points[index]->coord[2] && p_set->points[index]->coord[2]< 1000
            && 5000 < p_set->points[index]->coord[3] && p_set->points[index]->coord[3]< 150000
        )
        {
            bool is_same = false;
            for (int j = 0; j < p.size(); j++)
            {
                if (is_same_point(p_set->points[index], p[j]))
                {
                    is_same = true;
                    break;
                }
            }
            if (!is_same)
            {
                point_t *pp = alloc_point(dim);
                for (int i = 0; i < dim; i++)
                {
                    pp->coord[i] = p_set->points[index]->coord[i];
                }
                pp->dim = p_set->points[index]->dim;
                pp->id = p_set->points[index]->id;
                p.push_back(pp);
                count++;
            }
        }
    }
    return p;
}

void normalize_set(std::vector<point_t *> p_set, double *max, double *min)
{
    int dim = p_set[0]->dim;
    for (int j = 0; j < dim; j++)
    {
        max[j] = -1;
        min[j] = 999999;
    }
    for (int i = 0; i < p_set.size(); i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (max[j] < p_set[i]->coord[j])
            {
                max[j] = p_set[i]->coord[j];
            }
            if (min[j] > p_set[i]->coord[j])
            {
                min[j] = p_set[i]->coord[j];
            }
        }
    }
    for (int i = 0; i < p_set.size(); i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (j == 0 || j == 3)
            {
                p_set[i]->coord[j] = 1 - (p_set[i]->coord[j] - min[j]) / (max[j] - min[j]);
            }
            else
            {
                p_set[i]->coord[j] = (p_set[i]->coord[j] - min[j]) / (max[j] - min[j]);
            }
        }
    }
}

std::vector<point_t *> scale_up(std::vector<point_t *> p_set)
{
    int dim = p_set[0]->dim;
    std::vector<point_t *> P;
    for (int i = 0; i < p_set.size(); i++)
    {
        point_t *p = alloc_point(dim);
        for (int j = 0; j < dim; j++)
        {
            p->coord[j] = p_set[i]->coord[j] * 1000;
        }
        p->id = p_set[i]->id;
        p->dim = p_set[i]->dim;
        P.push_back(p);
    }
    return P;
}

int RI_user_study()
{
    point_set_t *P0 = read_points((char*)"car.txt");
    std::vector<point_t *> p_set, p_1, p_2;
    p_1 = random_choose(P0);
    int dim = p_1[0]->dim;
    double max[dim], min[dim];
    normalize_set(p_1, max, min);
    skyband(p_1, p_set, 1, P0->numberOfPoints);
    p_2 = scale_up(p_set);
    point_set_t *P = point_reload(p_set);
    //obatin the point set in different forms

    // the welcome message
    printf("-------------------------Welcome to the recommending car system--------------------------------\n");
    printf("Imagine that you have been saving for years to buy a car in the second-hand market. There are \n"
           "thousands of cars in the market. \n\n"
           "Since this is a big decision in your life, you want to make every penny count and pick your \n"
           "favorite car.\n"
           "\n"
           "In our research project, we want to study the performance of several recommendation algorithms \n"
           "to help you make a good decision.\n"
           "This survey consists of 2 parts. Based on your answers in part 1, there is a small chance that \n"
           "part 2 is skipped.\n\n" );


    FILE *wPtr;
    wPtr = (FILE *)fopen("../output/result.txt", "w");


    int r_size = 2;
    int returned_car[r_size], questions_asked[r_size], dissat_level[r_size], hit[r_size];

    //initialize dissatisfactory level to -1;
    for(int i=0; i<r_size; i++){ 
        dissat_level[i]=-1;
    }

    //initialize hit to false;
    for(int i=0; i<r_size; i++){ 
        hit[i]=0;
    }

    printf("===============================================================================================\n"
        "\n"
        "                                     Beginning of part 1\n"
        "\n"
        "===============================================================================================\n\n"
        "In part 1, there are 7 algorithms and each will ask you a list of questions.\n\n"
        "In each question, you will be presented 2 option of cars and you need to pick the one that you\n"
        "favor more. For example, enter 1 if you think car 1 is more preferred than car 2.\n\n"
        "Each car is described by the following 4 atrributes:\n\n"
       "    Price               Range from 1049-95400 USD\n"
       "    Year                Range from 2002-2015\n"
       "    Power               Range from 52-575 PS\n"
       "    Used kilometer      Range from 10000-125000 km\n\n\n");
    enter_to_continue();

    int check_num=3;
    double theta = 0.05;
    // HDPI_sampling(wPtr, p_set, P0, questions[3]);

    printf("========================= Algorithm 1 ==========================\n");
    // Algorithm PointPrune
    returned_car[0] = PointPrune_v2(wPtr, p_set, P0, check_num, questions_asked[0], max, min);

    printf("\n==================== End of  Algorithm 1 =======================\n");
    enter_to_continue();

    printf("\n========================= Algorithm 2 ==========================\n");
    //Algorithm HRI
    returned_car[1] = STMD(wPtr, p_set, P0, theta, questions_asked[1]);

    printf("\n==================== End of  Algorithm 2 =======================\n");
    enter_to_continue();

    // printf("========================= Algorithm 3 ==========================\n");
    // // Algorithm SpacePrune
    // returned_car[2] = SamplePrune(wPtr, p_set, P0, check_num, questions_asked[2], max, min);

    // // HDPI_sampling(wPtr, p_set, P0, questions[3]);
    // printf("========================= Algorithm 4 ==========================\n");
    // returned_car[3] = HDPI_accurate(wPtr, p_set, P0, questions_asked[3]);

    // printf("========================= Algorithm 5 =========================\n");
    // printf("waiting...\n");
    // //Algorithm: Preference Learning
    // returned_car[4] = Preference_Learning(wPtr, p_set, P0, questions_asked[4]);

    // printf("=========================Algorithm 6=========================\n");
    // // the UH-Simplex algorithm
    // returned_car[5] = max_utility(wPtr, P, P0, SIMPLEX, questions_asked[5]);

    // printf("=========================Algorithm 7=========================\n");
    // //Algorithm: Active Ranking
    // returned_car[1] = Active_Ranking(wPtr, p_2, P0, questions_asked[2]);



    printf("===============================================================================================\n"
        "\n"
        "                                     End of part 1\n"
        "\n"
        "===============================================================================================\n");

    std::vector<int> final_list;
    create_final_list(returned_car, r_size, final_list);

    int best_car = 0;
    if(final_list.size()>1){ // ask for user's favorite car if more than 1 car is in the final list
        printf("===============================================================================================\n"
        "\n"
        "                                  Beginning of part 2\n"
        "\n"
        "===============================================================================================\n");
        printf("In part 2, you come down to a final list that consists of cars recommended by previous algorithms.\n");
        printf("Now you need to decide your final choice of car.\n");
        printf("\n");
        printf("Again, This is a big decision in your life and you want to spend the money wisely.\n");
        printf("\n\n");
        printf("Below, you will see %d cars recommeneded by previous algorithms.\n", (int) final_list.size());
        printf("===============================================================================================\n");
        enter_to_continue();
        display_final_list(P0, final_list);
        best_car = ask_favorite_item(final_list.size());
    }

    // set the hit of the best car to true;
    for(int i=0;i<r_size;i++){
        if(returned_car[i]==final_list[best_car]){
            hit[i] = 1;
        }
    }

    // set the dissat level of the best car to 0;
    for(int i=0;i<r_size;i++){
        if(returned_car[i]==final_list[best_car]){
            dissat_level[i] = 0;
        }
    }

    if(final_list.size()>1){
        printf("\n\n==========================================================================\n\n");
        printf("For each of the rest of cars in the final list, please also tell us how \n");
        printf("**dissatisfied** you felt compared with the final choice you made above.\n");
        printf("\n\n");
    }
    for(int i=0;i<r_size;i++){
        if(dissat_level[i]<0){
            // ask the dissat level
            int dissat_score = ask_dissat_score(P0,returned_car[i]);

            // update  the dissat score of the same car
            for(int j=0; j<r_size; j++){
                if(returned_car[i]==returned_car[j]){
                    dissat_level[j]=dissat_score;
                }
            }
        }
    }

    release_point_set(P, true);
    fprintf(wPtr, "\n");
    fclose(wPtr);

    // write results to files
    FILE *q_file, *h_file, *d_file, *e_file;
    q_file = (FILE *)fopen("../output/questions.txt", "w");
    h_file = (FILE *)fopen("../output/hit.txt", "w");
    d_file = (FILE *)fopen("../output/dissat.txt", "w");
    e_file = (FILE *)fopen("../output/error.txt", "w");

    record_to_file(q_file, questions_asked, r_size);
    record_to_file(h_file, hit, r_size);
    record_to_file(d_file, dissat_level, r_size);

    fprintf(e_file, "%d ", num_checking);
    fprintf(e_file, "%d ", num_wrong_checking);
    fprintf(e_file, "\n");

    fclose(q_file);
    fclose(h_file);
    fclose(d_file);
    fclose(e_file);

    if(final_list.size()>1){
        printf("===============================================================================================\n"
        "\n"
        "                                     End of part 2\n"
        "\n"
        "===============================================================================================\n\n");
    }

    printf("===============================================================================================\n"
        "\n"
        "                                     End of survey\n"
        "                            Thank you for your participation\n"
        "\n"
        "===============================================================================================\n\n");

    printf("Please submit the zip file \"output.zip\" to us.\n\n");

    return 0;
}



    // point_t* r[7];
    // int questions[7];

    // printf("=========================Round 1=========================\n");
    // // the UH-Simplex algorithm
    // r[6] = max_utility(wPtr, P, P0, SIMPLEX, questions[6]);

    // printf("=========================Round 2=========================\n");
    // printf("waiting...\n");
    // //Algorithm: Preference Learning
    // r[0] = Preference_Learning(wPtr, p_set, P0, questions[0]);

    // printf("=========================Round 3=========================\n");
    // //Algorithm: RH
    // r[2] = RH(wPtr, p_set, P0, questions[2]);

    // printf("=========================Round 4=========================\n");
    // //the UH-Random algorithm
    // r[5] = max_utility(wPtr, P, P0, RANDOM, questions[5]);

    // printf("=========================Round 5=========================\n");
    // //Algorithm: Active Ranking
    // r[1] = Active_Ranking(wPtr, p_2, P0, questions[1]);

    // printf("=========================Round 6=========================\n");
    // //Algorithm HDPI
    // r[3] = HDPI_sampling(wPtr, p_set, P0, questions[3]);

    // printf("=========================Round 7=========================\n");
    // r[4] = HDPI_accurate(wPtr, p_set, P0, questions[4]);

    // int Rindex[7] = {2, 5, 3, 6, 7, 4, 1};
    // printf("Now comes to the comparison section\n");
    // for(int i = 2; i < 6; i++)
    // {
    //     for(int j = i + 1; j< 7; j++)
    //     {
    //         printf("=========================== Round %d ===========================\n", Rindex[i]);
    //         fprintf(wPtr, "=========================== Round %d ===========================\n", Rindex[i]);
    //         printf("No. of questions asked: %d \n", questions[i]);
    //         fprintf(wPtr, "No. of questions asked: %d \n", questions[i]);
    //         printf("Recommended cars:\n");
    //         fprintf(wPtr, "Recommended cars:\n");
    //         printf("--------------------------------------------------------\n");
    //         fprintf(wPtr, "--------------------------------------------------------\n");
    //         printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    //         fprintf(wPtr, "|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    //         printf("---------------------------------------------------------\n");
    //         fprintf(wPtr, "---------------------------------------------------------\n");
    //         printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[r[i]->id]->coord[0], P0->points[r[i]->id]->coord[1],
    //                 P0->points[r[i]->id]->coord[2], P0->points[r[i]->id]->coord[3]);
    //         fprintf(wPtr, "|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[r[i]->id]->coord[0], P0->points[r[i]->id]->coord[1],
    //                 P0->points[r[i]->id]->coord[2], P0->points[r[i]->id]->coord[3]);
    //         printf("---------------------------------------------------------\n");
    //         fprintf(wPtr, "---------------------------------------------------------\n");
    //         printf("=========================== Round %d ===========================\n", Rindex[j]);
    //         fprintf(wPtr, "=========================== Round %d ===========================\n", Rindex[j]);
    //         printf("No. of questions asked: %d \n", questions[j]);
    //         fprintf(wPtr, "No. of questions asked: %d \n", questions[j]);
    //         printf("Recommended cars:\n");
    //         fprintf(wPtr, "Recommended cars:\n");
    //         printf("--------------------------------------------------------\n");
    //         fprintf(wPtr, "--------------------------------------------------------\n");
    //         printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    //         fprintf(wPtr, "|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    //         printf("---------------------------------------------------------\n");
    //         fprintf(wPtr, "---------------------------------------------------------\n");
    //         printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[r[j]->id]->coord[0], P0->points[r[j]->id]->coord[1],
    //                 P0->points[r[j]->id]->coord[2], P0->points[r[j]->id]->coord[3]);
    //         fprintf(wPtr, "|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P0->points[r[j]->id]->coord[0], P0->points[r[j]->id]->coord[1],
    //                 P0->points[r[j]->id]->coord[2], P0->points[r[j]->id]->coord[3]);
    //         printf("---------------------------------------------------------\n");
    //         fprintf(wPtr, "---------------------------------------------------------\n");
    //         printf("Which round are you satisfied more? \n"
    //                "Please enter:\n"
    //                "  (1) 1 for Round %d \n"
    //                "  (2) 2 for Round %d \n"
    //                "  (3) 3 for both Round %d and Round %d.\n"
    //                "Note that all car(s) returned by each round must be one of your 20 most favorite cars\n"
    //                "in our used car database. Please answer these based on two criteria at the same time:\n"
    //                "(a) No. of questions asked and (b) The recommended car(s)\n"
    //                "You answer: ", Rindex[i], Rindex[j], Rindex[i], Rindex[j]);
    //         int sat = 4;
    //         while(sat!= 1 && sat!= 2 && sat!= 3)
    //         {
    //             scanf("%d", &sat);
    //         }
    //         fprintf(wPtr, "Which round are you satisfied more?  %d\n\n", sat);
    //         printf("\n\n");
    //     }
    // }