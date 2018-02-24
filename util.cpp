#include "util.h"

double randomDouble()
{
    //srand( (int) time(0));
    return (rand() / double(RAND_MAX )) * 2 - 1;
}