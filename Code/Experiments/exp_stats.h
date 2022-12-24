#include <chrono>
#include <vector>
#include <algorithm>
#include <iostream>

#ifndef EXP_STATS
#define EXP_STATS

extern double best_score;
extern double correct_count;
extern double question_num;
extern double return_size;
extern double tot_time;
extern std::vector<double> round_times;

void reset_timer();

void start_timer();

void start_timer(int round);

void stop_timer();

void stop_timer(int round);

void incre_total_time_microsec();

double avg_time();

void sort_time();

void print_time_stats();

#endif