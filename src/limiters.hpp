#ifndef LIMITERS
#define LIMITERS

void hardLimiter(int target_rpm, int cut_time, bool* coil_1_cut, bool* coil_2_cut);

void softLimiter(int target_rpm, int soft_cut_region, bool* coil_1_cut, bool* coil_2_cut);

#endif
