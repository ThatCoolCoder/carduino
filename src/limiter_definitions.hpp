#ifndef LIMITER_DEFINITIONS
#define LIMITER_DEFINITIONS

typedef enum LimiterType {
    HardSimple,
    HardTime,
    HardRpmHysteresis,
    SoftSimple,
    SoftTime,
    SoftRpmHysteresis
};

// Can't be bothered to do classes or polymorphism or delegation properly sooo
struct LimiterPreset {
    LimiterType type;
    int param1;
    int param2;
    int param3;
    int param4;
    int param5;
};

#define HARD_SIMPLE LimiterPreset { LimiterType::HardSimple }
#define HARD_TIME(cut_duration, use_early_timing) LimiterPreset { LimiterType::HardTime, cut_duration, use_early_timing }
#define HARD_RPM_HYSTERESIS(rpm_hysteresis) LimiterPreset { LimiterType::HardRpmHysteresis, rpm_hysteresis }
#define SOFT_SIMPLE(speed, soft_size) LimiterPreset { LimiterType::SoftSimple, speed, soft_size }
#define SOFT_TIME(speed, soft_size, hard_cut_duration, use_early_timing) LimiterPreset { LimiterType::SoftTime, speed, soft_size, hard_cut_duration, use_early_timing }
#define SOFT_RPM_HYSTERESIS(speed, soft_size, hard_cut_rpm_hysteresis) LimiterPreset { LimiterType::SoftRpmHysteresis, speed, soft_size, hard_cut_rpm_hysteresis }

#endif
