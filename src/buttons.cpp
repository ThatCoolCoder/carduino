#pragma once

#include <debounce.h>

#include "state.cpp"

void two_step_level_cycle_handler(uint8_t btnId, uint8_t btnState)
{
    // for some reason all these are 
    if (btnState == BTN_PRESSED)
    {
        two_step_level_idx ++;
        if (two_step_level_idx >= TWO_STEP_LEVEL_COUNT) two_step_level_idx = 0;
        blinkCode(two_step_level_idx + 1, 100, 100);
    }
}

void two_step_cut_mode_handler(uint8_t btnId, uint8_t btnState)
{
    if (btnState == BTN_PRESSED)
    {
        two_step_cut_mode = !two_step_cut_mode;
        blinkCode(two_step_cut_mode + 1, 100, 100);
    }
}

void rolling_cut_mode_handler(uint8_t btnId, uint8_t btnState)
{
    if (btnState == BTN_PRESSED)
    {
        rolling_cut_mode = !rolling_cut_mode;
        blinkCode(rolling_cut_mode + 1, 100, 100);
    }
}

void rolling_cut_handler(uint8_t btnId, uint8_t btnState)
{

    if (btnState == BTN_PRESSED && rolling_cut_target_rpm > 0)
    {
        rolling_cut_target_rpm = rpm;
    }
    else
    {
        rolling_cut_target_rpm = 0;
    }
}

void buttonConfig()
{
    myButton.setPushDebounceInterval(1000);
}

Button two_step_level_button(0, two_step_level_cycle_handler);
Button two_step_cut_mode_button(0, two_step_cut_mode_handler);
Button rolling_cut_mode_button(0, rolling_cut_mode_handler);

Button rolling_cut_button(0, rolling_cut_handler); // needs to be a button as we persist rpm on initial press

void updateButtons()
{
    if (TWO_STEP_ENABLED)
    {
        two_step_level_cycle_button.update(digitalRead(IN_TWO_STEP_LEVEL_CYCLE));
        two_step_cut_mode_button.update(digitalRead(IN_TWO_STEP_CUT_MODE));
    }
    if (ROLLING_CUT_ENABLED)
    {
        rolling_cut_mode_button.update(digitalRead(IN_ROLLING_CUT_MODE));
        rolling_cut_button.update(digitalRead(IN_ROLLING_CUT_BUTTON));
    }
    if (GLOBAL_LIMITER_ENABLED)
    {
        // todo: read global limi button
    }
}
