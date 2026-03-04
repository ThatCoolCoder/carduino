# carduino - general design document

## controlled functions
- killswitch
- spark cut (no-lift, rpm drop, 2 step/rolling at some point)
- varex, possibly

all controls except for killswitch and manual spark cut can be on a single removable panel


## inputs (all digital)
- master enable for everything except killswitch (wired so it disables if you disconnect control panel)
- killswitch: secure button pressed
- killswitch: attempting to start car (12v forced)
- spark cut: clutch pressed at all
- spark cut: accel fully down
- spark cut: manual button pressed
- spark cut: no-lift enabled
- spark cut, later: 2 step mode cycle
- spark cut, later: rolling on
- spark cut, later: rpm input (single thing i think, just pulsed?)
- varex: open
- varex: close

total: 12 (10 if not varex)
could save a pin by making no-lift enable actually just interrupt clutch or accel signal
as many signals as possible should be ground-interrupting so can not worry about shorts
for safety makes sense to run all switches using arduino ground/power


## outputs
- killswitch: horn (relay) - either directly drive horn relay, or relay for gnd for relay
- killswitch: fuel pump (relay) - probably best to allow/cut og relay input, so that we don't turn on when shouldn't. get-home measure is just to stick wire in to relay
- spark cut: coil 1 (relay, nc) - if using not a relay board these two could be combined, or could also use a dpdt. but having both controllable might be cool
- spark cut: coil 2 (relay, nc)
- varex: open (relay, dpdt)
- varex: close (relay, dpdt)
- spark cut: 2 step mode light

total: 7 (5 if not varex)

yes I know relays are not needed since they're all driving low power things or relays already, but we are on a different voltage from the rest of the car so it's easier to just interrupt/clear the functions than directly power
and nothing is high-freq




## Physical organisation:
- control board (removable, cord goes through ashtray and it mounts to it)
    - no lift-enabled
    - 2 step mode cycle
    - rolling antilag
- arduino board (shoved in or maybe tied, below radio aah)
    - I think this is purely a breakout board for arduino to robustly organise inputs into connectors

- relay board (near to arduino board but separate unless using


Plug standards
- 2 wires or less: some small thing
- 3 or more: standard automotive type that jaycar has


control board
  (plug)
    |
    |
    |
  (plug)
arduino board (plug)---------(optional plug) other inputs




## Logic:

spark cut: no-lift:
- trigger on when clutch and accel pressed
- if enabled and trigger on: cut both coils
- trigger off when clutch released

spark cut: 2 step:
- on a mode button press, cycle mode
- if mode is an on one, throttle down, clutch down and button pressed, lock on
- while on, do 2 step thingo
- if throttle up or clutch up, lock off

spark cut: rolling:
- on button first down: lock rpm
- if button down, perform similar controllinator to 2 step

killswitch:
- secure is false by default
- set secure if start signal on and secure button pressed
- if secure: fuel pump on
- if start signal and not secure: horn and lock out


## Configurables (so not including "active" settings)



- Global limi level
- Global limi cut mode
- Global limi hard cut duration
- Global limi soft cut size


- plus all the pins

## Configurables

A list of all the things that can be configured. Some of them need to be flashed on, some can be updated live. A configurable is anything that's set before the functionality is used

#### Configurables (flash)
- Security used

- Min safe rpm
- Max safe rpm

- Global limi used
- Global limi list of levels
- Global limi default level
- Global limi default cut mode
- Global limi hard cut duration
- Global limi soft cut size

- 2 step used
- 2 step default cut mode
- 2 step levels available
- 2 step default level
- 2 step hard cut duration
- 2 step soft cut size

- rolling cut used
- rolling cut default cut mode
- rolling hard cut duration
- rolling soft cut size

- pin assignments

#### Configurables (live)
- Global limi cut mode
- Global limi level

- 2 step cut mode
- 2 step current level

- no lift selected

- rolling cut mode

#### Control panel
- (global master switch - integrated)
- common ground
- LED
- global limi setting (short/long for level/cut mode)
- 2 step setting (short/long for level/cut mode)
- rolling cut mode
- no lift enabled