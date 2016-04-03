# Light Emissions

[TODO: Overview with a timing diagram]

## Sync Pulse

The synchronization pulse is a short flash of light that base stations emit periodically.
It is used for time synchronization and data transmission between base station and receivers.

The rising edge signals the start of a new rotation period for one of the two rotors.
The length of the sync pulse is used for identifying which rotor just hit its 0° mark, for determining if this rotor will activate its laser in this cycle, and for broadcasting information about the base station that sent this sync pulse.

Since sync pulses are used to synchronize time, they have to hit all reachable sensors of all tracked objects simultaneously. Therefore they use a wide angle light source like LEDs that can flood the entire tracked volume with light. The parts of a base station that generate this light pulse are called OOTX (Omnidirectional Optical Transmitter).

### Pulse Length

The following sync pulse lengths are defined:

Name | skip | rotor | data | length (ticks) | length (µs)
-----|------|-------|------|----------------|------------
j0   | 0    | 0     | 0    | 3000           | 62.5
j1   | 0    | 0     | 1    | 4000           | 83.3
k0   | 0    | 1     | 0    | 3500           | 72.9
k1   | 0    | 1     | 1    | 4500           | 93.8
j2   | 1    | 0     | 0    | 5000           | 104
j3   | 1    | 0     | 1    | 6000           | 125
k2   | 1    | 1     | 0    | 5500           | 115
k3   | 1    | 1     | 1    | 6500           | 135

[TODO: Meaning of the names?]

Each length represents a combination of 3 bit states:  
The *rotor* bit determines which of the two rotors caused this sync pulse.  
The *skip* bit determines if the rotor will skip this period and leave its laser off.  
The *data* bits of consecutive sync pulses of a base station concatenated together yield a data structure called OOTX Frame.

Given a measured pulse length the best match can be chosen from the table above.  
Given the 3 bits the pulse length can be calculated: length = 3000 + rotor\*500 + data\*1000 + skip\*2000

### OOTX Frame

An OOTX Frame is a data structure that base stations broadcast to all tracked objects. Each sync pulse contains one bit of data of the frame.

![OOTX Frame Data Structure](images/OOTX Frame.png)

[WIP]

## Lasers

[TODO]