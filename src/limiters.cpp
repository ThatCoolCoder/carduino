#include <Arduino.h>

#include "limiters.hpp"

#include "state.hpp"

void hardSimple(int target_rpm, bool* c1, bool* c2)
{
    if (rpm > target_rpm)
    {
        *c1 = true;
        *c2 = true;
    }
}

void hardTime(int target_rpm, int cut_time, bool use_early_timing, bool* c1, bool* c2)
{
    unsigned long now = millis();

    bool in_window = (now - last_cut_time) < cut_time;

    if (rpm > target_rpm || in_window)
    {
        *c1 = true;
        *c2 = true;
    }

    if (use_early_timing)
    {
        if (rpm > target_rpm && ! in_window) last_cut_time = now; // count from when enters limiter
    }
    else if (rpm > target_rpm) last_cut_time = now; // count from when leaves limiter
}

void hardRpmHysteresis(int target_rpm, int rpm_range, bool* c1, bool* c2)
{
    if (rpm > target_rpm) hysteresis_cut_active = true;

    if (hysteresis_cut_active)
    {
        *c1 = true;
        *c2 = true;
    }

    if (hysteresis_cut_active && rpm < (target_rpm - rpm_range)) hysteresis_cut_active = false;
}

void softRaw(int target_rpm, int speed, int size, bool* c1, bool* c2)
{
    if (rpm > target_rpm - size)
    {
        if (soft_cut_coil_2) *c2 = true;
        else *c1 = true;
    }

    unsigned long now = millis();
    if (now - soft_cut_switch_time > speed)
    {
        soft_cut_switch_time = now;
        soft_cut_coil_2 = ! soft_cut_coil_2;
    }
}

void softSimple(int target_rpm, int speed, int size, bool* c1, bool* c2)
{
    hardSimple(target_rpm, c1, c2);

    softRaw(target_rpm, speed, size, c1, c2);
}

void softTime(int target_rpm, int speed, int size, int cut_time, bool use_early_timing, bool* c1, bool* c2)
{
    hardTime(target_rpm, cut_time, use_early_timing, c1, c2);

    softRaw(target_rpm, speed, size, c1, c2);
}

void softRpmHysteresis(int target_rpm, int speed, int size, int hard_cut_rpm_hysteresis, bool* c1, bool* c2)
{
    hardRpmHysteresis(target_rpm, hard_cut_rpm_hysteresis, c1, c2);

    softRaw(target_rpm, speed, size, c1, c2);
}

void delegateLimiter(int target_rpm, LimiterPreset* p, bool* c1, bool* c2)
{
    if (p->type == HardSimple) hardSimple(target_rpm, c1, c2);
    if (p->type == HardTime) hardTime(target_rpm, p->param1, p->param2, c1, c2);
    if (p->type == HardRpmHysteresis) hardRpmHysteresis(target_rpm, p->param1, c1, c2);
    if (p->type == SoftSimple) softSimple(target_rpm, p->param1, p->param2, c1, c2);
    if (p->type == SoftTime) softTime(target_rpm, p->param1, p->param2, p->param3, p->param4, c1, c2);
    if (p->type == SoftRpmHysteresis) softRpmHysteresis(target_rpm, p->param1, p->param2, p->param3, c1, c2);
}
