#include "user_study_func.h"


int show_to_user(point_set_t* P, int p_idx, int q_idx)
{

    int option = 0;
    // ask the user for the better car among two given options
    while (option != 1 && option != 2)
    {
        printf("\n\n");
        printf("Please choose the car you favor more:\n");
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Option 1", P->points[p_idx]->coord[0], P->points[p_idx]->coord[1], P->points[p_idx]->coord[2], P->points[p_idx]->coord[3]);
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Option 2", P->points[q_idx]->coord[0], P->points[q_idx]->coord[1], P->points[q_idx]->coord[2], P->points[q_idx]->coord[3]);
        printf("--------------------------------------------------------\n");
        printf("Your choice: ");
        scanf("%d", &option);
    }

    return option;
}


int show_to_user_scale(point_set_t* P, int p_idx, int q_idx)
{
    double scale = (double)(90 + (rand()%10)) / 100; //generate a random number from 0.91-0.99
    int option = 0;
    // ask the user for the better car among two given options
    while (option != 1 && option != 2)
    {
        printf("\n\n");
        printf("Please choose the car you favor more:\n");
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Option 1", P->points[p_idx]->coord[0] * scale, 
                    P->points[p_idx]->coord[1] * scale, P->points[p_idx]->coord[2] * scale, P->points[p_idx]->coord[3] * scale);
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Option 2", P->points[q_idx]->coord[0] * scale, 
                    P->points[q_idx]->coord[1] * scale, P->points[q_idx]->coord[2] * scale, P->points[q_idx]->coord[3] * scale);
        printf("--------------------------------------------------------\n");
        printf("Your choice: ");
        scanf("%d", &option);
    }

    return option;
}


int show_to_user(const point_t* p1, const point_t* p2)
{

    int option = 0;
    // ask the user for the better car among two given options
    while (option != 1 && option != 2)
    {
        printf("\n\n");
        printf("Please choose the car you favor more:\n");
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Option 1", p1->coord[0], p1->coord[1], p1->coord[2], p1->coord[3]);
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Option 2", p2->coord[0], p2->coord[1], p2->coord[2], p2->coord[3]);
        printf("--------------------------------------------------------\n");
        printf("Your choice: ");
        scanf("%d", &option);
    }

    return option;
}

int show_to_user_scale(const point_t* p1, const point_t* p2)
{
    double scale = (double)(90 + (rand()%10)) / 100; //generate a random number from 0.91-0.99
    int option = 0;
    // ask the user for the better car among two given options
    while (option != 1 && option != 2)
    {
        printf("\n\n");
        printf("Please choose the car you favor more:\n");
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Option 1", p1->coord[0] * scale,
                    p1->coord[1] * scale, p1->coord[2] * scale, p1->coord[3] * scale);
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Option 2", p2->coord[0] * scale, 
                    p2->coord[1] * scale, p2->coord[2] * scale, p2->coord[3] * scale);
        printf("--------------------------------------------------------\n");
        printf("Your choice: ");
        scanf("%d", &option);
    }

    return option;
}

void print_result(FILE* wPtr, const char *alg_name, const int num_of_question, const point* p){
    // print result to user
    printf("\n--------------------------------------------------------\n");
    printf("%s %10d \n", "No. of questions asked:", num_of_question);
    printf("--------------------------------------------------------\n");
    printf("Recommended car:\n");
    printf("--------------------------------------------------------\n");
    printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    printf("---------------------------------------------------------\n");
    printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", p->coord[0],p->coord[1],
                p->coord[2], p->coord[3]);
    printf("---------------------------------------------------------\n");


    // print result to file 
    fprintf(wPtr, "--------------------------------------------------------\n");
    fprintf(wPtr, "|%30s |%10d |\n", alg_name, num_of_question);
    fprintf(wPtr, "--------------------------------------------------------\n");
    fprintf(wPtr, "Recommended car:\n");
    fprintf(wPtr, "--------------------------------------------------------\n");
    fprintf(wPtr, "|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    fprintf(wPtr,"---------------------------------------------------------\n");
    fprintf(wPtr,"|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", p->coord[0], p->coord[1],
               p->coord[2], p->coord[3]);
    fprintf(wPtr,"---------------------------------------------------------\n");

};

/**
 * @brief Display cars in the final list
 * 
 * @param final_list 
 */
void display_final_list(const point_set_t* P, const std::vector<int> final_list){
    for(int i=0; i<final_list.size(); i++){
        printf("\n\n");
        printf("Recommended car No. %d:\n", i+1);
        printf("--------------------------------------------------------\n");
        printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
        printf("---------------------------------------------------------\n");
        printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P->points[final_list[i]]->coord[0], P->points[final_list[i]]->coord[1],
                    P->points[final_list[i]]->coord[2], P->points[final_list[i]]->coord[3]);
        printf("---------------------------------------------------------\n");
    }
}


/**
 * @brief Ask the user to decide the favorite car in all returned cars
 * 
 * @param l_size Size of the final list
 * @return int 
 */
int ask_favorite_item(int l_size){
    int favorite = 0;
    printf("Please give a number from 1 to %d to indicate which car\n"
            "you like the most in the recommended cars listed above \n"
            "(e.g., 2 means car No. 2 is your favorite car)\n"
            "You answer: ", l_size);
    while(favorite <1 || favorite > l_size){
        scanf("%d", &favorite);
    }
    return favorite-1;
}


/**
 * @brief Aske the dissatisfactory level of a car
 * 
 * @param P 
 * @param car 
 * @return int 
 */
int ask_dissat_score(const point_set_t* P, int car_id){
    printf("\n");
    printf("Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate how \n"
            " **dissatisfied** you feel for the following car recommended to you. \n"
            "(1 means the least dissatisfied and 10 means the most dissatisfied): \n");
    printf("--------------------------------------------------------\n");
    printf("Recommended cars:\n");
    printf("--------------------------------------------------------\n");
    printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    printf("---------------------------------------------------------\n");
    printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P->points[car_id]->coord[0], P->points[car_id]->coord[1],
                P->points[car_id]->coord[2], P->points[car_id]->coord[3]);
    printf("---------------------------------------------------------\n");
    int dissat = 0;
    while(dissat < 1 || dissat > 10)
    {
        printf("You answer: ");
        scanf("%d", &dissat);
        printf("\n");
    }
    printf("\n\n");
    return dissat;
}


/**
 * @brief Record the satisfactory level of the user. The satisfacory level ranges from 1-10
 * 
 * @param P                             point set
 * @param num_questions_asked           Total # questions asked by the algorithm
 * @param p_result_id                   Id of the top-1 point selected by alg
 * @param alg_name                      name of the alg
 * @param w_ptr                         file pointer
 * @return int 
 */
int ask_satisfactory_level(const point_set_t* P, const int num_questions_asked, const int p_result_id, const char *alg_name, FILE *w_ptr){

    printf("\n--------------------------------------------------------\n");
    printf("%s %10d \n", "No. of questions asked:", num_questions_asked);
    printf("--------------------------------------------------------\n");
    printf("Recommended cars:\n");
    printf("--------------------------------------------------------\n");
    printf("|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    printf("---------------------------------------------------------\n");
    printf("|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P->points[p_result_id]->coord[0], P->points[p_result_id]->coord[1],
                P->points[p_result_id]->coord[2], P->points[p_result_id]->coord[3]);
    printf("---------------------------------------------------------\n");
    printf("Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate \n"
            "how bored you feel when you are asked with %d questions for this round \n"
            "in order to obtain one of your 20 most favorite cars (Note: 10 denotes\n"
            "that you feel the most bored and 1 denotes that you feel the least bored.)\n"
            "You answer: ", num_questions_asked);
    int sat = 0;
    while(sat < 1 || sat > 10)
    {
        scanf("%d", &sat);
    }
    printf("\n\n");

    fprintf(w_ptr, "--------------------------------------------------------\n");
    fprintf(w_ptr, "|%30s |%10d |\n", alg_name, num_questions_asked);
    fprintf(w_ptr, "--------------------------------------------------------\n");
    fprintf(w_ptr, "Recommended cars:\n");
    fprintf(w_ptr, "--------------------------------------------------------\n");
    fprintf(w_ptr, "|%10s|%10s|%10s|%10s|%10s|\n", " ", "Price(USD)", "Year", "PowerPS", "Used KM");
    fprintf(w_ptr, "---------------------------------------------------------\n");
    fprintf(w_ptr, "|%10s|%10.0f|%10.0f|%10.0f|%10.0f|\n", "Car", P->points[p_result_id]->coord[0], P->points[p_result_id]->coord[1],
                P->points[p_result_id]->coord[2], P->points[p_result_id]->coord[3]);
    fprintf(w_ptr, "---------------------------------------------------------\n");
    fprintf(w_ptr, "Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate \n"
                    "how bored you feel when you are asked with %d questions for this round \n"
                    "in order to obtain one of your 20 most favorite cars (Note: 10 denotes\n"
                    "that you feel the most bored and 1 denotes that you feel the least bored.) %d\n\n\n", num_questions_asked, sat);
    
    return 0;
}



void record_to_file(FILE *wPtr, int *records, int r_size){
    for(int i = 0; i < r_size; i++){
        fprintf(wPtr, "%d ", records[i]);
    }
    fprintf(wPtr, "\n");
}

