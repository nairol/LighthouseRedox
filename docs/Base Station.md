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
