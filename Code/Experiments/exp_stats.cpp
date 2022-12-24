#include "exp_stats.h"

double best_score = 0;
double question_num = 0;
double return_size = 0;
double correct_count = 0;
double tot_time = 0;
std::vector<double> round_times;

int cur_round;

using namespace std::chrono;
steady_clock::time_point begin, end;

void reset_timer(){
    tot_time = 0;
}

void reset_stats(){
    best_score = 0;
    question_num = 0;
    return_size = 0;
    correct_count = 0;
    tot_time = 0;
}

void start_timer(){
    begin = steady_clock::now();
}

void start_timer(int round){
    begin = steady_clock::now();
    cur_round = round;
}

void stop_timer(){
    end = steady_clock::now();
    round_times.push_back((double) duration_cast<std::chrono::microseconds>(end - begin).count());
    incre_total_time_microsec();
}

void stop_timer(int round){
    end = steady_clock::now();
    int num_rounds = round - cur_round;
    cur_round = round;
    if(num_rounds > 0){
        double avg_time_these_rounds = (double) duration_cast<std::chrono::microseconds>(end - begin).count() / num_rounds;
        for(int i = 0; i < num_rounds; i++){
            round_times.push_back(avg_time_these_rounds);
        }
    }
    incre_total_time_microsec();
}

void incre_total_time_microsec(){
    tot_time += duration_cast<std::chrono::microseconds>(end - begin).count();
}

double avg_time(){
    return tot_time / question_num;
}

void sort_time(){
    std::sort(round_times.begin(), round_times.end());
}

void print_time_stats(){
    sort_time();
    int size = round_times.size();
    std::cout << "avg :" << avg_time() << std::endl;
    std::cout << "P95 :" << round_times[0.95 * size] << std::endl;  
    std::cout << "P75 :" << round_times[0.75 * size] << std::endl; 
    std::cout << "Median :" << round_times[0.5 * size] << std::endl; 
    std::cout << "P25 :" << round_times[0.25 * size] << std::endl; 
    std::cout << "Low :" << round_times[0.05 * size] << std::endl; 
}