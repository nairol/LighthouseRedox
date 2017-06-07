# Base Station

A Lighthouse Base Station is a light-emitting device that acts as a reference point for Lighthouse-compatible receivers.



## Block Diagram

![Base Station Block Diagram](images/Base%20Station%20-%20Block%20Diagram.png)

## Modes

Base stations can be operated in a number of different modes, sometimes also called channels.

### TDM slave with cable sync (mode A)

### TDM master (mode B)

### TDM slave with optical sync (mode C)


## Base Station Info Block

[TODO: Name is made up. Find real name...]

Tracked objects need to know some information about each base station they see. 
This data structure is sent continuously via the OOTX data channel of each base station.

Its main use cases are identification of base stations, providing base station factory calibration data to the receivers and error/status reporting.


Offset | Type    | Name             | Description
-------|---------|------------------|------------
0x00   | uint16  | fw_version       | Firmware version (bit 15..6), protocol version (bit 5..0)
0x02   | uint32  | ID               | Unique identifier of the base station
0x06   | float16 | fcal.0.phase     | "phase" for rotor 0
0x08   | float16 | fcal.1.phase     | "phase" for rotor 1
0x0A   | float16 | fcal.0.tilt      | "tilt" for rotor 0
0x0C   | float16 | fcal.1.tilt      | "tilt" for rotor 1
0x0E   | uint8   | sys.unlock_count | Lowest 8 bits of the rotor desynchronization counter
0x0F   | uint8   | hw_version       | Hardware version
0x10   | float16 | fcal.0.curve     | "curve" for rotor 0
0x12   | float16 | fcal.1.curve     | "curve" for rotor 1
0x14   | int8    | accel.dir_x      | "orientation vector"
0x15   | int8    | accel.dir_y      | "orientation vector"
0x16   | int8    | accel.dir_z      | "orientation vector"
0x17   | float16 | fcal.0.gibphase  | "gibbous phase" for rotor 0 (normalized angle)
0x19   | float16 | fcal.1.gibphase  | "gibbous phase" for rotor 1 (normalized angle)
0x1B   | float16 | fcal.0.gibmag    | "gibbous magnitude" for rotor 0
0x1D   | float16 | fcal.1.gibmag    | "gibbous magnitude" for rotor 1
0x1F   | uint8   | mode.current     | Currently selected mode (default: 0=A, 1=B, 2=C)
0x20   | uint8   | sys.faults       | "fault detect flags" (should be 0)

All integer data types are sent/received in Little-Endian byte order (LSB first).

The current **protocol version** is 6. For older protocol versions the data structure might look different.  
E.g. In version 4 and 5 the data block is only 31 bytes long. The last two fields are missing.

The **ID** is a (pseudo-)random 32-bit number. The current implementation uses the CRC32 of the 128-bit MCU UID.

The meaning of the **factory calibration data (fcal.\*)** is currently not known.  
Phase probably describes the phase difference between the real rotor angle and the rotor angle measurement using the optical rotation sensor.  
Tilt and curve are probably related to the rotation and distortion of the projected laser line.

The **orientation vector accel.dir_[xyz]** is not a unit vector and its length has no meaning. It is scaled in a way that its biggest/smallest component always is +127/-127.

## Bluetooth LE communications

The Base stations can be put to sleep and woken up via a Bluetooth LE GATT
characteristic. Also they can receive radio firmware updates.  The "Name" and
"Alias" characteristics contain a string "HTC BS xyABCD" where xy is the last
byte of the Bluetooth address and ABCD is half of the base station unique
identifier.

### Wake up and set sleep timeout

To wake up a base station or to set its sleep timeout, write 20 bytes to the
GATT characteristic with UUID 0000cb01-0000-1000-8000-00805f9b34fb (handle =
0x0035):

```
gatttool --device=xx:xx:xx:xx:xx:xy -I
[xx:xx:xx:xx:xx:xy][LE]> connect
[xx:xx:xx:xx:xx:xy][LE]> char-write-req 0x0035 1202012cffffffff000000000000000000000000
```

Offset | Type   | Name             | Description
-------|--------|------------------|------------
0x00   | uint8  | unknown          | 0x12
0x01   | uint8  | unknown          | 0x02
0x02   | uint16 | timeout?         | big-endian, 300 s
0x04   | uint32 | ID               | Unique identifier of the base station or 0xffffffff
0x08   | uint8  |                  | 0x00
...    | ...    |                  | ...
0x13   | uint8  |                  | 0x00

Waking up an already sleeping base station works with the ID set to 0xffffffff.
To set the sleep timeout, ID must be set to the correct value.

```
[xx:xx:xx:xx:xx:xy][LE]> char-read-hnd 0x0035
Characteristic value/descriptor: 00 12 01 2c 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
```

Offset | Type   | Name             | Description
-------|--------|------------------|------------
0x00   | uint8  | unknown          | 0x00
0x01   | uint8  | unknown          | 0x12
0x02   | uint16 | timeout?         | big-endian, 300 s
0x04   | uint8  |                  | 0x00
...    | ...    |                  | ...
0x13   | uint8  |                  | 0x00

### Query radio firmware version

```
[xx:xx:xx:xx:xx:xy][LE]> char-write-req 0x0035 1503000000000000000000000000000000000000
```

Offset | Type   | Name             | Description
-------|--------|------------------|------------
0x00   | uint8  | unknown          | 0x15
0x01   | uint8  | unknown          | 0x03
0x02   | uint8  |                  | 0x00
...    | ...    |                  | ...
0x13   | uint8  |                  | 0x00

```
[xx:xx:xx:xx:xx:xy][LE]> char-read-hnd 0x0035
Characteristic value/descriptor: 00 15 03 02 0b 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
```

Offset | Type   | Name             | Description
-------|--------|------------------|------------
0x00   | uint8  | unknown          | 0x00
0x01   | uint8  | unknown          | 0x15
0x02   | uint8  | unknown          | 0x03
0x03   | uint8  | version_major    | 0x02
0x04   | uint8  | version_minor    | 0x0b
0x05   | uint8  |                  | 0x00
...    | ...    |                  | ...
0x13   | uint8  |                  | 0x00

```
[xx:xx:xx:xx:xx:xy][LE]> disconnect
```
