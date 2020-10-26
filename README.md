# Arduino Greenhouse
Simple Arduino Greenhouse that handles light and soil moisture levels.

README will only cover Arduino code and gives expected hardware requirements. Pump, light, greenhouse, and wiring are not covered here as they're independent of the code and too variable. 

## Requirements
### Arduino
Tested on an Arduino Nano. 

| Pin   | Used  |
|:-----:|:-----:|
|3.3v|2|
|5v|2|
|Analog In|1|
|Digital I/O|3|
|Ground|4|

### Relays
2 x Relays rated 5VDC, 15A 125VAC

### Sensor
1 x Capacitive soil sensor

### Other
1 x water pump with water source (small AC pump used, others should be fine)

1 x grow light

Jumpers, extra 18-12 gauge wire, and power supply for arduino. 

## Setup
All user configuration is in header of [arduino.ino](arduino/arduino.ino) file. 

### Values
`relayLight` : Digital I/O pin for light controlling relay

`relayLightState` : State of pin controlling the light relay on startup. Default of LOW is so light is off on startup, allowing for light to be easily shutoff if unwanted (i.e. night)
|Value|State|
|:---:|:---:|
|HIGH|ON|
|LOW|OFF|

`lightPin` : Pin used to inverse light state on startup. If a digitalRead on this pin returns LOW, the effect of `relayLightState` will be inversed. Primarily used for testing when the default behavior isn't wanted but reprogramming the arduino is not desired. 

`relayPump` : Digital I/O pin for pump controlling relay 

`AirValue` : Value returned by soil moisture sensor when it is suspended in air. (acts as 0 for % calculations)

`WaterValue` : Value returned by soil moisture sensor when it is suspended in water. (acts as 100 for % calculations)

`targetMoistureLow` : Lower bound of soil moisture % to target
`targetMoistureHigh` : Upper bound of soil moisture % to target

## TODO
- [ ] Add output to an LCD screen with current status 
- [ ] Wiring diagram
- [ ] Better switch system for light
  - [ ] toggle momentary switch (switch light state and reset 12 hour timer)
  - [ ] toggle switch (will light be on vs off at next reset)