#include "RI_user_study.h"
#include <ctime>
#include <stdlib.h>

int num_checking = 0;
int num_wrong_checking = 0;

int main(int argc, char *argv[])
{
    // the main method for the demo system
    srand(time(0));
    RI_user_study();

    return 0;
}
