#include <Arduino.h>

#include "limiters.hpp"

#include "state.hpp"

void hardLimiter(int target_rpm, int cut_time, bool* coil_1_cut, bool* coil_2_cut)
{
    // Cut both coils whenever rpm > target, and don't uncut for at least cut_time

    bool in_window = (millis() - last_hard_cut) < cut_time;

    if (rpm > target_rpm)
    {
        if (! in_window) last_hard_cut = millis();

        *coil_1_cut = true;
        *coil_1_cut = true;
    }
    else if (in_window)
    {
        *coil_1_cut = true;
        *coil_1_cut = true;
    }
    else
    {
        *coil_1_cut = false;
        *coil_1_cut = false;
    }
}

void softLimiter(int target_rpm, int soft_cut_region, bool* coil_1_cut, bool* coil_2_cut)
{
    // If is within soft_cut_region below the limit, only cut 1 coil
    // else cut 2 but only as short as needed

    if (rpm > target_rpm)
    {
            *coil_1_cut = true;
            *coil_2_cut = true;
    }
    else if (rpm > target_rpm - soft_cut_region)
    {
        if (soft_cut_was_on_coil_1)
        {
            *coil_1_cut = false;
            *coil_2_cut = true;
        }
        else
        {
            *coil_1_cut = true;
            *coil_2_cut = false;
        }

        soft_cut_was_on_coil_1 = ! soft_cut_was_on_coil_1;
    }
    else
    {
        *coil_1_cut = false;
        *coil_2_cut = false;
    }
}


