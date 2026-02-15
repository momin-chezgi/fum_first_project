#ifndef DRAFTSMAN_H
#define DRAFTSMAN_H
#include "includer.h"

struct draftsman
{
    int id = -1;
    int x = -1;
    int y = -1;
    int temp_token = 0;
    int token_limit = k;
    bool winned = false;
    bool defeated = false;
};


#endif //DRAFTSMAN_H