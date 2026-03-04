# lancer arduino controller

https://forum.arduino.cc/t/reading-tach-output-wire-12v-to-5v/469365/4


see also the other markdown file for early overall design info

## features

#### global master enable

it's designed for most of the controls to be located on a removable (ie hideable) panel. when the panel is detached, the master wire is detached and this disables all features except security (as security needs active input from this to start the car. if this wasn't implemented you would have to rewire things to get it to start w no panel)

#### security

car won't start and instead will sound horn if you try to start without pressing a hidden unlock button at the same time

once secured, remains secured until ignision is turned off (so doesn't hinder quick starting after a stall or anything)

#### manual spark cut

ie keybanging

implemented purely to make revs drop quicker on shifts

cuts spark whenever you hold down a button

#### no-lift shifting

there is a switch to enable/disable it (may change to a momentary button)

when accelerator is floored and clutch begins to be pressed, spark is cut, and only returns when clutch is released (accelerator can be released without regaining spark)

#### 2 step (not tested - no rpm input yet)

not a perfect 2 step as it just cuts, doesn't adjust timing. so probably doesn't spool turbos as much

if the 2 step button is pressed, it limits rpm to a preselected value until it deactivates

it stays on until the clutch is released AND the button is no longer held (so you can use it without pressing the clutch if you just hold it)

there's a button to cycle between off and the different rpm limits

can switch between a softer and harder limiter

#### rolling rpm limiter (not tested - no rpm input yet)

not perfect as it just cuts, doesn't adjust timing. so probably doesn't spool turbos as much

when button is pressed, holds rpm at the initial value from when you pressed the button

like the 2 step, you can switch between hard and soft cut

## conventions

snake case variables and camel case functions for some reason

Have used the following terms:
- "enabled" is a compile/flash-time switch to add or remove functionality
- "selected" long-duration on-off thing, set through a switch (whether it *could* do stuff)
- "active" whether it's currently doing stuff, set through a momentary button 

Configuration-checking guards have been moved to inside functions where possible