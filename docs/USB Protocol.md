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
0x01   | ?      | 3    | ?                   | unknown
0x04   | uint16 | 2    | lensSeparationA     | Lens separation potentiometer reading
0x06   | ?      | 2    | ?                   | unknown
0x08   | uint8  | 1    | pressedButtons      | 1: System button pressed / 0: no button pressed
0x09   | ?      | 3    | ?                   | unknown
0x0C   | uint8  | 1    | faceProximityChange | 0: No change / 1: Neg. change / 2: Pos. change
0x0D   | ?      | 1    | ?                   | unknown
0x0E   | uint16 | 2    | faceProximity       | Typical values: 40 - 1900
0x10   | uint16 | 2    | lensSeparationB     | Lens separation potentiometer reading
0x12   | ?      | 46   | ?                   | unknown

Both lens separation potentiometer readings scale linearly with the lens
separation. There is a calibration value "lens_separation" in the config store.

### HMD: IMU

* Report ID: 5
* Report Length: 64 Bytes
* Device -> Host
* Control Transfer

Offset | Type   | Size | Name             | Description
-------|--------|------|------------------|-----------------------------------
0x00   | uint8  | 1    | reportID         | HID report identifier (=5)
0x01   | uint32 | 4    | firmwareVersion  | Firmware version
0x09   | string | 16   | firmwareProvider | "steamservices"
0x19   | string | 16   | firmwareName     | "firmware-win32"
0x29   | uint8  | 1    | microRevision    | Hardware micro revision
0x2A   | uint8  | 1    | minorRevision    | Hardware minor revision
0x2B   | uint8  | 1    | majorRevision    | Hardware major revision
0x2C   | uint8  | 1    | productID        | Hardware product ID: 128
0x31   | uint8  | 1    | minorFPGA        | FPGA minor version
0x32   | uint8  | 1    | majorFPGA        | FPGA major version

This report returns information about the HMD firmware, FPGA, and hardware version.

* Report ID: 8
* Report Length: 64 Bytes
* Device -> Host
* Control Transfer

Offset | Type   | Size | Name           | Description
-------|--------|------|----------------|-----------------------------------
0x00   | uint8  | 1    | reportID       | HID report identifier (=8)
0x01   | uint32 | 4    | offset         | Current read offset
0x05   | uint32 | 4    | size           | Size of the userdata partition (3997696 bytes)

This report returns the current read offset and total size of the userdata
partition containing the two mura correction PNG images.

* Report ID: 8
* Report Length: 9 Bytes
* Host -> Device
* Control Transfer

Offset | Type   | Size | Name           | Description
-------|--------|------|----------------|-----------------------------------
0x00   | uint8  | 1    | reportID       | HID report identifier (=8)
0x01   | uint32 | 4    | offset         | Read offset to be set
0x05   | uint32 | 4    | ignored(?)     | Write 0

This report sets the read offset for the following Report ID 9 transfer.
Wait for a millisecond between seeking and the first read.

* Report ID: 9
* Report Length: 64 Bytes
* Device -> Host
* Control Transfer

Offset | Type   | Size | Name           | Description
-------|--------|------|----------------|-----------------------------------
0x00   | uint8  | 1    | reportID       | HID report identifier (=9)
0x01   | uint8  | 63   | data           | Userdata bytes starting at offset

This report returns 63 bytes of userdata and increments the read offset by 63
bytes. Repeatedly request Report ID 9, to read from the userdata partition.
Wait for a millisecond between reads.

* Report ID: 16
* Report Length: 64 Bytes
* Device -> Host
* Control Transfer

Offset | Type   | Size   | Name           | Description
-------|--------|--------|----------------|-----------------------------------
0x00   | uint8  | 1      | reportID       | HID report identifier (=16)
0x01   | ?      | 63     | ?              | unknown

This report preceeds repeated transfers of Report ID 17 to read out the
configuration store. Maybe it just puts the firmware into config read mode
and resets the read offset to 0.

* Report ID: 17
* Report Length: 64 Bytes
* Device -> Host
* Control Transfer

Offset | Type   | Size   | Name           | Description
-------|--------|--------|----------------|-----------------------------------
0x00   | uint8  | 1      | reportID       | HID report identifier (=17)
0x01   | uint8  | 1      | length         | Length (up to 62)
0x02   | uint8  | length | data           | Config data starting at offset

This report returns up to 62 bytes of config data and increments the read
offset by 62. Repeatedly request Report ID 17 until it returns 0 in the length
byte. The configuration store contains a zlib-compressed JSON file.

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

Axes: When wearing the HMD +X points to the right, +Y points downwards, +Z points forwards.

The right-hand rule is used.

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
0x02   | uint16 | 2    | reportType(?)       | 0x0B04: Ping (every second) / 0x3C01: User input
0x04   | uint32 | 4    | reportCount         | Counter that increases with every report
0x08   | uint32 | 4    | pressedButtons      | Bit field, see below for individual buttons
0x0C   | uint16 | 2    | triggerOrBattery    | Analog trigger value (user input) / Battery voltage? (ping)
0x0E   | uint8  | 1    | batteryCharge       | Bit 7: Charging / Bit 6..0: Battery charge in percent
0x0F   | ?      | 1    | ?                   | unknown
0x10   | uint32 | 4    | hardwareID          | Hardware ID (user input) / 0x00000000 (ping)
0x14   | int16  | 2    | touchpadHorizontal  | Horizontal thumb position (Left: -32768 / Right: 32767)
0x16   | int16  | 2    | touchpadVertical     | Vertical thumb position (Bottom: -32768 / Top: 32767)
0x18   | ?      | 2    | ?                   | unknown
0x1A   | uint16 | 2    | triggerHighRes      | Analog trigger value with higher resolution
0x1C   | ?      | 24   | ?                   | unknown
0x34   | uint16 | 2    | triggerRawMaybe     | Analog trigger value, maybe raw sensor data
0x36   | ?      | 8    | ?                   | unknown
0x3E   | uint8  | 1    | someBitFieldMaybe   | 0x00: ping / 0x64: user input
0x3F   | ?      | 1    | ?                   | unknown

Buttons:

Bit  0: Trigger Button  
Bit  3: Grip Buttons  
Bit 12: Application Button  
Bit 13: System Button
Bit 18: Touchpad pressed
Bit 20: Thumb on touchpad


## Wireless Controller Dongle

Work in progress...

* Report ID: 5
* Report Length: 64 Bytes
* Device -> Host
* Control Transfer

Offset | Type   | Size | Name             | Description
-------|--------|------|------------------|-----------------------------------
0x00   | uint8  | 1    | reportID         | HID report identifier (=5)
0x01   | uint32 | 4    | firmwareVersion  | Firmware version
0x09   | string | 16   | firmwareProvider | "htcvrsoftware"
0x19   | string | 16   | firmwareName     | "firmware-win32"
0x29   | uint8  | 1    | microRevision    | Hardware micro revision
0x2A   | uint8  | 1    | minorRevision    | Hardware minor revision
0x2B   | uint8  | 1    | majorRevision    | Hardware major revision
0x2C   | uint8  | 1    | productID        | Hardware product ID: 129
0x31   | uint8  | 1    | minorFPGA        | FPGA minor version
0x32   | uint8  | 1    | majorFPGA        | FPGA major version

This report returns information about the controller firmware, FPGA, and
hardware version. This is the same as Report ID 5 in the IMU section.

* Report ID: 16
* Report Length: 64 Bytes
* Device -> Host
* Control Transfer

See Report ID 16 in the IMU section.

* Report ID: 17
* Report Length: 64 Bytes
* Device -> Host
* Control Transfer

See Report ID 17 in the IMU section.

* Report ID: 35
* Report Length: 30 Bytes
* Device -> Host
* Interrupt Transfer

Offset | Type   | Size | Name                | Description
-------|--------|------|---------------------|------------------------------------------------------------
0x00   | uint8  | 1    | reportID            | HID report identifier (=35)
0x01   | uint8  | 1    | time1               | Bits 31-24 of a 48 MHz counter
0x02   | uint8  | 1    | type1(?)            | Bits 15-8 of the type value(?)
0x03   | uint8  | 1    | time2               | Bits 23-16 of a 48 MHz counter
0x04   | uint8  | 1    | type2(?)            | Bits 7-0 of the type value(?)

Depending on the value of ((type1 << 8) | type2), the remaining up to 25
bytes contain a different type of event:

### Types: 0x3f1 (Button), 0x4f5 (Trigger button)

Offset | Type   | Size | Name                | Description
-------|--------|------|---------------------|------------------------------------------------------------
0x05   | uint8  | 1    | pressedButtons      | Bit field, seel below for individual buttons
0x06   | ?      | 4    | ?                   | unknown

Buttons:

Bit  0: Trigger button
Bit  1: Touchpad pressed
Bit  2: Thumb on touchpad
Bit  3: System button
Bit  4: Grip button
Bit  5: Menu button

### Type: 0x3f4 (Analog trigger value)

Offset | Type   | Size | Name                | Description
-------|--------|------|---------------------|------------------------------------------------------------
0x05   | uint8  | 1    | triggerValue        | Analog trigger value
0x06   | ?      | 4    | ?                   | unknown

### Type: 0x6f2 (Touch movement)

Offset | Type   | Size | Name                | Description
-------|--------|------|---------------------|------------------------------------------------------------
0x05   | int16  | 2    | touchpadHorizontal  | Horizontal thumb position
0x07   | int16  | 2    | touchpadVertical    | Vertical thumb position
0x09   | ?      | 4    | ?                   | unknown

### Type: 0x7f3 (Touch start/stop event)

Offset | Type   | Size | Name                | Description
-------|--------|------|---------------------|------------------------------------------------------------
0x05   | int16  | 1    | pressedButtons      | Bit field, see above for individual buttons
0x06   | int16  | 2    | touchpadHorizontal  | Horizontal thumb position
0x08   | int16  | 2    | touchpadVertical    | Vertical thumb position
0x0A   | ?      | 4    | ?                   | unknown

### Type: 0xfe8 (IMU sample)

Offset | Type   | Size | Name                | Description
-------|--------|------|---------------------|------------------------------------------------------------
0x05   | uint8  | 1    | time3               | Bits 15-8 of a 48 MHz counter
0x06   | int16  | 2    | accX                | Acceleration along the X axis
0x08   | int16  | 2    | accY                | Acceleration along the Y axis
0x0A   | int16  | 2    | accZ                | Acceleration along the Z axis
0x0C   | int16  | 2    | rotX                | Angular velocity along the X axis
0x0E   | int16  | 2    | rotY                | Angular velocity along the Y axis
0x10   | int16  | 2    | rotZ                | Angular velocity along the Z axis
0x12   | ?      | 4    | ?                   | unknown

### Type: 0x11e1 (Ping, battery charge)

Offset | Type   | Size | Name                | Description
-------|--------|------|---------------------|------------------------------------------------------------
0x05   | uint8  | 1    | chargeStatus        | Bit 8 is charging, bits 7-0 are battery charge in percent
0x06   | ?      | 2    | ?                   | unknown
0x08   | int16  | 2    | accX                | Acceleration along the X axis
0x0A   | int16  | 2    | accY                | Acceleration along the Y axis
0x0C   | int16  | 2    | accZ                | Acceleration along the Z axis
0x0E   | int16  | 2    | rotX                | Angular velocity along the X axis
0x10   | int16  | 2    | rotY                | Angular velocity along the Y axis
0x12   | int16  | 2    | rotZ                | Angular velocity along the Z axis
0x14   | ?      | 4    | ?                   | unknown

Other observed but not yet understood types are:

* Type: 0x7f6, 0x10f9, 0x10fc, 0x11fd, 0x13fa, 0x14fb, 0x14f9 (Unknown)
* Type: 0xae1, 0x15ff, 0x8f7 (Unknown, sent during poweron)

* Report ID: 36
* Report Length: 59 Bytes
* Device -> Host
* Interrupt Transfer

This is basically the same as Report ID 35, except that it contains the payload
of two events. The first starts at offset 0x01, the second at offset 0x30.

As soon as the controller can see a lighthouse base station, lighthouse events
are sent with wildly varying values in the type1 and type2 bytes.

* Report ID: 255
* Host -> Device
* Control Transfer

Offset | Type   | Size | Name           | Description
-------|--------|------|----------------|-----------------------------------
0x00   | uint8  | 1    | reportID       | HID report identifier (=255)
0x01   | uint8  | 1    | command        | Command to send to the controller
0x02   | uint8  | 1    | length         | Length of the following command payload

Depending on the command, the payload and with it the sent report has a
different length. Known commands:

### Command: 0x8f (Haptic pulse)

* Report Length: 10

Offset | Type   | Size | Name           | Description
-------|--------|------|----------------|-----------------------------------
0x02   | uint8  | 1    | length         | Payload length: 7
0x03   | ?      | 7    | ?              | unknown

### Command: 0x9f (Power off)

* Report Length: 7

Offset | Type   | Size | Name           | Description
-------|--------|------|----------------|-----------------------------------
0x02   | uint8  | 1    | length         | Payload length: 4
0x03   | uint8  | 1    | magic0         | Constant: 'o'
0x04   | uint8  | 1    | magic1         | Constant: 'f'
0x05   | uint8  | 1    | magic2         | Constant: 'f'
0x06   | uint8  | 1    | magic3         | Constant: '!'
