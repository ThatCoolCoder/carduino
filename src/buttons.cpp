#include <Arduino.h>

#include <debounce.h>

#include "blink.cpp"
#include "state.cpp"

void two_step_level_handler(uint8_t btnId, uint8_t btnState)
{ 
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

Button two_step_level_btn(0, two_step_level_handler);
Button two_step_cut_mode_btn(0, two_step_cut_mode_handler);

void global_limiter_level_handler(uint8_t btnId, uint8_t btnState)
{ 
    if (btnState == BTN_PRESSED)
    {
        global_limiter_level_idx ++;
        if (global_limiter_level_idx >= GLOBAL_LIMITER_LEVEL_COUNT) global_limiter_level_idx = 0;
        blinkCode(global_limiter_level_idx + 1, 100, 100);
    }
}

void global_limiter_cut_mode_handler(uint8_t btnId, uint8_t btnState)
{
    if (btnState == BTN_PRESSED)
    {
        global_limiter_cut_mode = !global_limiter_cut_mode;
        blinkCode(global_limiter_cut_mode + 1, 100, 100);
    }
}

Button global_limiter_level_btn(0, global_limiter_level_handler);
Button global_limiter_cut_mode_btn(0, global_limiter_cut_mode_handler);

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

Button rolling_cut_mode_btn(0, rolling_cut_mode_handler);
Button rolling_cut_btn(0, rolling_cut_handler); // needs to be a button as we persist rpm on initial press



void buttonConfig()
{
    // myButton.setPushDebounceInterval(1000);
}

void updateButtons()
{
    if (TWO_STEP_ENABLED)
    {
        two_step_level_btn.update(digitalRead(IN_TWO_STEP_SETTING));
        two_step_cut_mode_btn.update(digitalRead(IN_TWO_STEP_SETTING));
    }
    if (GLOBAL_LIMITER_ENABLED)
    {
        global_limiter_level_btn.update(digitalRead(IN_GLOBAL_LIMITER_SETTING));
        global_limiter_cut_mode_btn.update(digitalRead(IN_GLOBAL_LIMITER_SETTING));
    }
    if (ROLLING_CUT_ENABLED)
    {
        rolling_cut_mode_btn.update(digitalRead(IN_ROLLING_CUT_SETTING));
        rolling_cut_btn.update(digitalRead(IN_ROLLING_CUT_SETTING));
    }
}
