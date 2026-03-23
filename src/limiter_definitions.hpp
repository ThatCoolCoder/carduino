#ifndef LIMITER_MACROS
#define LIMITER_MACROS

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
#define HARD_TIME(cut_duration) LimiterPreset { LimiterType::HardTime, cut_duration }
#define HARD_RPM_HYSTERESIS(rpm_hysteresis) LimiterPreset { LimiterType::HardRpmHysteresis, rpm_hysteresis }
#define SOFT_SIMPLE LimiterPreset { LimiterType::SoftSimple }
#define SOFT_TIME(hard_cut_duration) LimiterPreset { LimiterType::SoftTime, hard_cut_duration }
#define SOFT_RPM_HYSTERESIS(hard_cut_rpm_hysteresis) LimiterPreset { LimiterType::SoftRpmHysteresis, hard_cut_rpm_hysteresis }

#endif
