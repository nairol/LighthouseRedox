# USB Protocol

The reference implementation of the Lighthouse system uses the USB HID protocol with custom reports to communicate with the PC.

This list is not complete! All "feature reports" are still missing.

All integer data types are stored in Little-Endian byte order (LSB first).

## Device List

Device                                       | Vendor ID | Product ID | Interface |
---------------------------------------------|-----------|------------|-----------|
HMD: Button and face proximity sensor        | 0x0bb4    | 0x2c87     | 0         |
HMD: IMU (acceleration and angular velocity) | 0x28de    | 0x2000     | 0         |
HMD: Lighthouse sensors                      | 0x28de    | 0x2000     | 1         |
Controller (wired): IMU                      | 0x28de    | 0x2012     | 0         |
Controller (wired): Lighthouse sensors       | 0x28de    | 0x2012     | 1         |
Controller (wired): Buttons and touchpad     | 0x28de    | 0x2012     | 2         |
Wireless Controller Dongle                   | 0x28de    | 0x2101     | 0         |


## HMD

### HMD: System Button and Face Proximity Sensor

* Report ID: 3
* Report Length: 64 Bytes
* Device -> Host
* Interrupt Transfer
* Interrupt Resolution: 4ms
* Sent every: 52ms

Offset | Type   | Size | Name                | Description
-------|--------|------|---------------------|------------------------------------------------
0x00   | uint8  | 1    | reportID            | HID report identifier (=3)
0x01   | ?      | 7    | ?                   | unknown
0x08   | uint8  | 1    | pressedButtons      | 1: System button pressed / 0: no button pressed
0x09   | ?      | 3    | ?                   | unknown
0x0C   | uint8  | 1    | faceProximityChange | 0: No change / 1: Neg. change / 2: Pos. change
0x0D   | ?      | 1    | ?                   | unknown
0x0E   | uint16 | 2    | faceProximity       | Typical values: 40 - 1900
0x10   | ?      | 2    | ?                   | Constant: 0x44 0x1B
0x12   | ?      | 46   | ?                   | unknown

### HMD: IMU

* Report ID: 32
* Report Length: 52 Bytes
* Device -> Host
* Interrupt Transfer
* Interrupt Resolution: 1ms
* Sent every: 1ms

Offset | Type   | Size | Name           | Description
-------|--------|------|----------------|-----------------------------------
0x00   | uint8  | 1    | reportID       | HID report identifier (=32)
0x01   | int16  | 2    | sample[0].accX | Acceleration along the X axis
0x03   | int16  | 2    | sample[0].accY | Acceleration along the Y axis
0x05   | int16  | 2    | sample[0].accZ | Acceleration along the Z axis
0x07   | int16  | 2    | sample[0].rotX | Angular velocity along the X axis
0x09   | int16  | 2    | sample[0].rotY | Angular velocity along the Y axis
0x0B   | int16  | 2    | sample[0].rotZ | Angular velocity along the Z axis
0x0D   | uint32 | 4    | sample[0].time | Sample time in ticks
0x11   | uint8  | 1    | sample[0].seq  | Sample sequence number (see below)
0x12   | int16  | 2    | sample[1].accX | 
0x14   | int16  | 2    | sample[1].accY | 
...    | ...    | ...  | ...            | ...
0x33   | uint8  | 1    | sample[2].seq  | 


### HMD: Lighthouse Sensors

* Report ID: 37
* Report Length: 64 Bytes
* Device -> Host
* Interrupt Transfer
* Interrupt Resolution: 1ms
* Sent every: on demand

Offset | Type   | Size | Name              | Description
-------|--------|------|-------------------|---------------------------------------------------
0x00   | uint8  | 1    | reportID          | HID report identifier (=37)
0x01   | uint8  | 1    | pulse[0].sensorID | ID of the sensor that was hit by the optical pulse
0x02   | uint16 | 2    | pulse[0].length   | Length of the pulse in ticks
0x04   | uint32 | 4    | pulse[0].time     | Time of the pulse in ticks
0x08   | uint8  | 1    | pulse[1].sensorID | 
0x09   | uint16 | 2    | pulse[1].length   | 
...    | ...    | ...  | ...               | ...
0x3C   | uint32 | 4    | pulse[8].time     | 


## Controller (wired)

### Controller: IMU

* Report ID: 32
* Report Length: 52 Bytes
* Device -> Host
* Interrupt Transfer
* Interrupt Resolution: 1ms
* Sent every: 4ms

See "HMD: IMU" for the report structure since it uses the same report ID.


### Controller: Lighthouse Sensors

* Report ID: 33
* Report Length: 58 Bytes
* Device -> Host
* Interrupt Transfer
* Interrupt Resolution: 1ms
* Sent every: on demand

Offset | Type   | Size | Name              | Description
-------|--------|------|-------------------|---------------------------------------------------
0x00   | uint8  | 1    | reportID          | HID report identifier (=33)
0x01   | uint16 | 2    | pulse[0].sensorID | ID of the sensor that was hit by the optical pulse
0x03   | uint16 | 2    | pulse[0].length   | Length of the pulse in ticks
0x05   | uint32 | 4    | pulse[0].time     | Time of the pulse in ticks
0x09   | uint16 | 2    | pulse[1].sensorID | 
0x0B   | uint16 | 2    | pulse[1].length   | 
...    | ...    | ...  | ...               | ...
0x35   | uint32 | 4    | pulse[6].time     | 
0x39   | uint8  | 1    | ?                 | unknown


### Controller: Buttons and touchpad

* Report ID: 1
* Report Length: 64 Bytes
* Device -> Host
* Interrupt Transfer
* Interrupt Resolution: 6ms
* Sent every: 1s + on demand

Offset | Type   | Size | Name                | Description
-------|--------|------|---------------------|------------------------------------------------------------
0x00   | uint8  | 1    | reportID            | HID report identifier (=1)
0x01   | ?      | 1    | ?                   | unknown
0x02   | uint16 | 2    | eventType           | 0x0B04: Ping (every second) / 0x3C01: User input
0x04   | uint32 | 4    | eventCount          | Counter that increases with every report
0x08   | uint32 | 4    | pressedButtons      | Bit field, see below for individual buttons
0x0C   | uint16 | 2    | triggerOrBattery    | Analog trigger value (user input) / Battery voltage? (ping)
0x0E   | uint8  | 1    | batteryCharge       | Bit 7: Charging / Bit 6..0: Battery charge in percent
0x0F   | ?      | 1    | ?                   | unknown
0x10   | uint32 | 4    | hardwareID          | Hardware ID (user input) / 0x00000000 (ping)
0x14   | int16  | 2    | touchpadHorizontal  | Horizontal thumb position (Left: -32768 / Right: 32767)
0x16   | int16  | 2    | touchpadVerical     | Vertical thumb position (Bottom: -32768 / Top: 32767)
0x18   | ?      | 2    | ?                   | unknown
0x1A   | uint16 | 2    | triggerHighRes      | Analog trigger value with higher resolution
0x1C   | ?      | 24   | ?                   | unknown
0x34   | uint16 | 2    | triggerRawMaybe     | Analog trigger value, maybe raw sensor data
0x36   | ?      | 8    | ?                   | unknown
0x3E   | uint8  | 1    | someBitFieldMaybe   | 0x00: ping / 0x64: user input
0x3F   | ?      | 1    | ?                   | unknown


## Wireless Controller Dongle

Work in progress...

Basically everything is multiplexed together in some way and I still need to find out how. :)

It seems to depend on the timestamp that is sent in each report...