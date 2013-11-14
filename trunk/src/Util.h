#ifndef UtilH
#define UtilH

namespace Gibdd
{
    static unsigned int g_seed = 0;

    inline void random_seed(int seed = 0)
    {
        if(!seed)
            g_seed = GetTickCount();
        else
            g_seed = seed;
    }

    inline int random_int(int min, int max)
    {
        g_seed=214013*g_seed+2531011;
        return min+(g_seed ^ g_seed>>15)%(max-min+1);
    }

    inline float random_float(float min, float max)
    {
        g_seed = 214013*g_seed + 2531011;
        return min + (g_seed >> 16) * (1.0f / 65535.0f)*(max - min);
    }
}

#endif