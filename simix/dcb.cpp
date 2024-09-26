#include "simix.h"

namespace Simix
{

static const uint8_t DBTable[] =
{
    0,  0,  0,  0,  0,  1,  1,  1,  1,   1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,   1,
    1,  1,  1,  1,  2,  2,  2,  2,  3,   3,
    4,  4,  5,  5,  6,  7,  8,  9,  10,  11,
    12, 14, 16, 18, 20, 22, 25, 28, 32,  36,
    40, 45, 50, 57, 64, 71, 80, 90, 101, 114, 
    128
};


int32_t DCBToVol(int32_t db)
{
    db += 100;

    if (db < 40)
        return 0;
    
    if (db > 100)
        return 128;
    
    return DBTable[db - 40];
}


}