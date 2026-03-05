# carduino - car controller with arduino

https://forum.arduino.cc/t/reading-tach-output-wire-12v-to-5v/469365/4

Car controller designed for 4 cylinder engines with wasted spark systems.

Security and spark cut shenanigans.

## Features

These are all of the features, which can be enabled independently (although some require others):

- Security
- Global master switch
- Status LED
- Manual spark cut/keybang
- Accelerator/clutch input
- No-lift shifting
- RPM reading
- Global RPM limiter
- 2 step limiter
- Rolling RPM limiter

## Info

See docs/features.md detail on each feature (including how to use) docs/wiring.md for wiring & hardware info, docs/config.md for how to configure/install

## Coding conventions

snake case variables and camel case functions for some reason

Have used the following terms:
- "enabled" is a compile/flash-time switch to add or remove functionality. as these are set at compile-time, we don't need to worry about transient state caused when these change.
- "selected" long-duration on-off thing, set through a switch (whether it *could* do stuff)
- "active" whether it's currently doing stuff, set through a momentary button 

Configuration-checking guards have been moved to inside functions where possible (rather than staying with the caller)