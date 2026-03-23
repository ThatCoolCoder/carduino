#include "rpm.hpp"

#include "config.hpp"
#include "confighelpers.hpp"
#include "limiters.hpp"
#include "state.hpp"


void logRpmPulse()
{
    rpm_pulse_idx ++;
    rpm_pulse_times[rpm_pulse_idx] = millis();
}

void updateRpm()
{
    int total_time = rpm_pulse_times[rpm_pulse_idx] - rpm_pulse_times[(rpm_pulse_idx + 1) % (RPM_SMOOTHNESS + 1)];
    float avg_time = (float) total_time / RPM_SMOOTHNESS;

    float rpm_pulse_hz = 1.0f / avg_time * 1000.0;

    rpm = (int) (rpm_pulse_hz / (float) RPM_PULSES_PER_REVOLUTION * 60.0f);
}
