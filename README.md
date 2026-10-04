# carduino - car controller with arduino

Car controller designed for 4 cylinder engines with wasted spark systems, or more cylinder engines with 

Security and spark cut shenanigans.

## Features

These are all of the features, which can be enabled independently (although some require others):

- Security (killswitch+alarm)
- Global master switch
- Status LED
- Manual spark cut/keybang
- Accelerator/clutch input
- No-lift shifting
- RPM reading
- Global RPM limiter
- 2 step limiter
- Rolling launch control limiter

## Info

See docs/features.md detail on each feature (including how to use) docs/wiring.md for wiring & hardware info, docs/config.md for how to configure/install

## Coding conventions

Baud rate - 38400

snake case variables and camel case functions for some reason

Have used the following terms:
- "enabled" is a compile/flash-time switch to add or remove functionality. as these are set at compile-time, we don't need to worry about transient state caused when these change.
- "selected" long-duration on-off thing, set through a switch (whether it *could* do stuff)
- "active" whether it's currently doing stuff, set through a momentary button 

Configuration-checking guards have been moved to inside functions where possible (rather than staying with the caller)