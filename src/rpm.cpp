#include "rpm.hpp"

#include "config.hpp"
#include "confighelpers.hpp"
#include "limiters.hpp"
#include "state.hpp"


void logRpmPulse()
{
    rpm_pulse_idx ++;
    rpm_pulse_idx %= RPM_SMOOTHNESS + 1;
    rpm_pulse_times[rpm_pulse_idx] = (long) micros();
}

void updateRpm()
{
    long total_time = rpm_pulse_times[rpm_pulse_idx] - rpm_pulse_times[(rpm_pulse_idx + 1) % (RPM_SMOOTHNESS + 1)];
    float avg_time = (float) total_time / RPM_SMOOTHNESS;

    int new_rpm = 1000000.0f / avg_time / (float) RPM_PULSES_PER_REVOLUTION * 60.0f;

    long now = micros();

    int delta = new_rpm - rpm;

    if (abs(delta) > RPM_MAX_JUMP && (now - last_valid_rpm_time) < RPM_MAX_STALE_DURATION_MS)
    {
        if (LOG_RPM)
        {
            Serial.print("rpm: ");
            Serial.print(new_rpm);
            Serial.print(" (bad) - using stale reading of ")
            Serial.println(rpm);
        }
        return;
    }

    if (LOG_RPM)
    {
        Serial.print("rpm: ")
        Serial.println(new_rpm);
    }
    last_valid_rpm_time = now;
    rpm = new_rpm;
}
