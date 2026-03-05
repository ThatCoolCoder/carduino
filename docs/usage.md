# Carduino - usage

Below is usage information for each of the features, plus information on which features require which other features.

## Security

The system interrupts the fuel pump signal or similar when trying to start unless a hidden unlock button is pressed at the same time. If you try to start without pressing this, the horn (or another alarm) will instead sound.

On a failed start attempt, the system will lock out until you turn the ignition all the way off.

## Global master switch

It's designed for the main control panel to be quickly removable so it can be hidden if needed. A pin can be set up to act as the master switch, which puts the system into a safe mode where it does nothing except allow security functionality.

## Status LED

Single-color LED that flashes info to show current settings etc.

## Spark cut

2 relays - one interrupting the spark signal for each coil.

## Manual spark cut (requires spark cut)

Completely cuts spark whenever a button is pressed. Main motivation for adding was to reduce rev hang on shifts.

## Pedal input

It's possible to wire up 2 binary switches for the accelerator and clutch pedal. The clutch pedal checks whether the clutch is at the top of its stroke (ie released), and the accelerator switch checks if it's fully depressed.

## No-lift shifting (requires pedal input & spark cut)

Whenever the accelerator is floored and the clutch begins to be pressed, spark is cut. It only returns when the clutch is fully released (this prevents power coming back if accelerator is released slightly).

There is also a switch to enable/disable it while driving as it may be undesired in some circumstances.

## RPM reading

Reads tachometer signal to determine engine RPM, to be used in other features.

## Limiters (all require rpm reading and spark cuts)

Various RPM limiters are present, using either a soft or hard limiter.

Hard limiter cuts spark whenever RPM goes over a threshold, and keeps it cut for a certain duration.

Soft limiter cuts one coil (alternating between them rapidly) whenever it gets within a certain

The limiter parameters can be configured individually for each sub-feature, and the limiter type can be switched from soft to hard while driving for each sub-feature.

### Global limiter

RPM limiter that is always active (providing system is not in safe mode). Useful if your engine has no limiter, or you would like to have something lower. Can cycle through preset levels (including disabled) while driving and switch type.

### 2 step (optionally requires pedals)

Not an amazing 2 step since it doesn't retard timing or anything to help spool turbos, but it still holds it at a specified level.

This is activated when a button is pressed and adds a temporary limiter until it is deactivated. Deactivation only occurs if the button is released and the clutch is out.

Can cycle through preset levels (including disabled) while driving and switch type.

### Rolling limiter

Again, not amazing since it only cuts instead of retarding timing. Consider brake boosting in addition.

When pressed, sets a RPM value, and limits RPM to this value until the button is released.

Can switch type while driving.