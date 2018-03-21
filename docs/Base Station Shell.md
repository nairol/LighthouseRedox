# LHTX Shell Commands

Lighthouse Base Stations can be configured, tested and calibrated using an embedded debugging shell reachable via the UART header on the PCB (requires opening the box or drilling hole at the right place) or via USB CDC (requires flashing a hacked firmware).

The following commands are available:

## id
Shows name, serial number, software and hardware version.

```
lhtx> id
Device Name: BEC08E38
Serial Number: BEC08E38
Firmware Build: 436 2016-09-20 19:00:20 <steamservices@firmware-win32>
Hardware ID: 82020300
```

Device Name is the same as parameter `sys.name`.  
Serial Number is the same (pseudo-)random device-specific constant that is also sent via OOTX.  
The first number of the Firmware Build is the combined firmware (bit 15..6) and protocol version (bit 5..0).  
Hardware ID is the same as the contents of EEPROM address [0xE08..0xE0F] (8 bytes).


## uptime
Shows the time that the base is powered, including time in standby mode.
```
lhtx> uptime
0 day 00:20:26.216
```


## reboot
Reboots the ARM Cortex-M0 MCU. (Sets SYSRESETREQ in register AIRCR.)


## isp
Disables all light emmissions, stops the motors and enters the ISP mode (in-system programming) provided by the MCU boot ROM. The same can be achieved by holding the mode button while plugging in the base station power connector.


## crash
Crashes the MCU by trying to call a non-existing function at address 0xDEAD0000 in an unmapped memory region.

```
lhtx> crash
cmd_crash: 0x000038F9
func: 0xDEAD0000
```


## stats
Shows statistical information about rotor period times and the optical sync receiver.  
Same as changing the parameter `stats.enable`.  

```
                    Number of samples (rotations)
                    |           Mean rotation period
                    |           |                  Standard deviation of rotation period
                    |           |                  |         sigma/mu in parts per million
                    |           |                  |         |        ?
                    |           |                  |         |        |      L=Locked / X=Unlocked
...                 |           |                  |         |        |      |
rotor 0 period (k = 6681): mu = 16.667 ms, sigma = 0.120 us (7.2 ppm) 2050.2 L
rotor 1 period (k = 6682): mu = 16.667 ms, sigma = 0.131 us (7.9 ppm) 2046.5 L
corr optical (k = 11511): mu = 16.667 ms, sigma = 0.347 us (20.8 ppm) 406.9 locked
...               |            |                  |         |         |     |
                  |            |                  |         |         |     Optical sync state
                  |            |                  |         |         ?
                  |            |                  |         sigma/mu in parts per million
                  |            |                  Standard deviation of optical sync period
                  |            Mean optical sync period
				  Number of samples (sync flashes)
```


## clear
Resets rotor statistics. (see command `stats`)  
Done automatically when rotor PLL regains lock after desynchronization.


## dump
Toggles rotor.N.debug for both rotors which shows debug output for each completed rotation.

```
%R 0 1692999925 1574 2053 -75
%R 1 1693399925 1533 2055 -74
   | |          |    |    |
   | |          |    |    PID input (measured error)
   | |          |    PID output (inverted, centered at 2048, saturation limits 0..4095)
   | |          Value of parameter rotor.N.pwm
   | Timestamp
   Rotor
```


## pll
Toggles PLL motor control. Same as changing `rotor.0.pll.enable` and `rotor.1.pll.enable` manually.
The effects of open-loop vs. closed-loop motor control can best be seen with the command `stats`.


## plldebug
Toggles PLL debugging output. Same as changing rotor.0.pll.debug and rotor.1.pll.debug manually.

```
           Rotor
           | Edge direction: 0=Neg. / 1=Pos.
           | | Timestamp of the edge event
...        | | |
DPLL: edge 1 1 1714539916
DPLL: edge 0 0 1714739920
DPLL: edge 1 0 1714739920
DPLL: 1 -4 2041 -6
DPLL: edge 0 1 1714939943
DPLL: 0 23 2038 -9
...   | |  |    |
      | |  |    PID output (raw)
      | |  PID output (centered at 2047, saturation limits 0..4095)
      | PID input (measured error)
      Rotor
```


```
DPLL: 0 timestamp nonmonotonicity: 803883814 803932368
      |                            |         |
      |                            |         Previous timestamp
      Rotor                        Current timestamp
```


## auto
Toggles PWM auto-tuning on and off. (Default is on)  
Same as changing the params `rotor.0.pwm.auto` and `rotor.1.pwm.auto` manually.  
Enable the parameter `sys.warnings` to see autotune status messages.

```
param set sys.warnings 1
...
autotune: 0 step 1 1675.0
autotune: 0 step 1 1676.0
autotune: 1 step 1 1661.0
autotune: 1 step -1 1660.0
...       |      |  |
          |      |  New PWM value
          |      PWM value difference (new-old)
          Rotor
```


## autodebug
Doesn't seem to do anything...  
The command toggles two variables (0x100004C9 and 0x10000531) but they are not used anywhere(?).


## freq <Hz>
Change rotor spin frequency. Range 10-100 Hz. Stable range ~40-90 Hz.


## carrier <Hz>
Change modulation frequency. Range 500000-12000000 Hz.  
Same as changing parameter `carrier.frequency`, but it checks the range limits.


## param

### param list [filter]
Lists all available parameters.  
If a filter is specified the list only contains parameters starting with that expression. (No wildcard support)  
[A better list of all parameters](../data/cparams/Vive%20Base%20Station%20436.md) is available in the data folder.

```
lhtx> param list mode.0
mode.0.period 800000
mode.0.timebase.mode slave-cable
mode.0.timebase.offset 0
mode.0.ootx medium
mode.0.label 65
mode.0.carrier 1843200
```


### param info <name>
Shows information about the given parameter.
```
lhtx> param info timebase.mode
name: timebase.mode
type: enumerated
offset: 0x21
value: slave-optical
default: slave-cable
unsaved: false
description: timebase mode
values:
        0 master-tdm
        1 master-sync
        2 master-async
        3 slave-cable
        4 slave-optical
```
Offset is the address in each EEPROM partition that the parameter resides in.  
Unsaved is true if the parameter has been changed in RAM but hasn't been stored to EEPROM yet.


### param get <name>
Prints the current value of the specified parameter.
```
lhtx> param get carrier.frequency
carrier.frequency       1843200
```


### param set <name> <value>
Changes the value of the specified parameter.
```
lhtx> param set sys.standby true
```
For boolean parameters the value 0 or 1 can be used instead of false or true.


### param default <name>
Resets the specified parameter to its default value.
```
lhtx> param default sys.standby
```


### param default-all
Resets all parameters to their corresponding default value if they have one.


### param part
Checks the CRC of all partitions and shows a list with some information about them.

```
lhtx> param part
Partition 0 @ 0x0000 NORM: 0x31E4D7CB 0x31E4D7CB CRC-OK 515
Partition 1 @ 0x0300 NORM: 0xA1E3A005 0xA1E3A005 CRC-OK 514
Partition 2 @ 0x0600 FACT: 0x49B10AA8 0x49B10AA8 CRC-OK 4
              |      |     |          |              |  |
              |      |     |          |              |  Partition write counter
              |      |     |          |              Intregrity check OK/BAD
              |      |     |          CRC32 (read from partition header)
              |      |     CRC32 (actual)
              |      Partition type: Normal or Factory
              Partition base address in EEPROM
```


### param erase <partition>
Erases the specified partition by overwriting it with zero-bytes.  
**DO NOT erase partition 2** ("factory") unless you have a **VERY** good reason, **a backup** of the partition **and** know how to restore it.


### param load [partition]
Loads all params from the currently active (or the specified) EEPROM partition.
```
lhtx> param load 0
551 bytes read from partition 0.
```


### param save [<partition> <newWriteCounterValue>]
The implicit variant of this command (`param save`) selects one of the first two partitions, increments its writeCounter value and writes all EEPROM-backed parameters into it. This variant is safe to use. It is also automatically called each time the mode button is pressed.  

When using the other variant of this command where the partition and new write counter value must be specified and you want to save to partition 2 ("factory"), ensure that you have a backup of the partition AND know how to restore it.


### param raw <r|w> <address>
Reads/writes all parameters from/to EEPROM starting at the specified base address.  
The only sensible addresses for this command are the 3 partition base addresses plus the partition header size: 0x000C, 0x030C, 0x060C  
Do not use addresses in the range [0x03C0..0x083F] for writing or the factory partition will be damaged/overwritten!  
Also do not use addresses bigger than 0x0B80 for writing or the hardware ID will be overwritten!
```
lhtx> param raw r 0x000C
551 bytes loaded from 0xC.
```


## step
Doesn't seem to do anything...


## pwmscan
Tests the rotors at different speeds. Prints statistical information at each step.

```
lhtx> pwmscan
Going dark...
Rotor PWM/speed sweep (any key to stop).
400.0 ppm stability tolerance.
4000 stabilising time-out spins. 100 measurement spins.
Rotor 0 answering PWM = 0: 119849.0 ppm 23528.7 ppm 7659.0 ppm 2797.6 ppm 1075.7 ppm 451.8 ppm 177.9 ppm
Rotor 0 @ PWM = 0 (k = 100) 9.776 ms 102.295 Hz, sigma = 1.239 us (126.8 ppm)
Rotor 0 answering PWM = 40: 34.2 ppm
Rotor 0 @ PWM = 40 (k = 100) 9.772 ms 102.337 Hz, sigma = 0.582 us (59.5 ppm)
Rotor 0 answering PWM = 80: 29.4 ppm
Rotor 0 @ PWM = 80 (k = 100) 9.769 ms 102.364 Hz, sigma = 0.369 us (37.8 ppm)
Rotor 0 answering PWM = 120: 433.5 ppm 215.1 ppm
... [10 minutes later] ...
Rotor 1 @ PWM = 2800 (k = 100) 30.779 ms 32.490 Hz, sigma = 1.255 us (40.8 ppm)
Rotor 1 answering PWM = 2840: 7933.5 ppm 273.3 ppm
Rotor 1 @ PWM = 2840 (k = 100) 31.729 ms 31.517 Hz, sigma = 1.137 us (35.8 ppm)
Rotor 1 answering PWM = 2880: 672618.3 ppm *time-out*
Rotor 1 @ PWM = 2880 (k = 0) 0.000 ms 0.000 Hz, sigma = 0.000 us (0.0 ppm)

Rotor 1 stalled at PWM = 2880.
Waiting for rotor lock...
Rotor sweep complete.
```


## pwmcal
Tries to find a good approximation function that maps rotation frequency to PWM value.
In order to quickly reach the user-selected rotation frequency the firmware needs to have a good first guess at the pulse width it needs to configure the motor driver with.
A linear function is constructed using two measurements and the parameters `rotor.N.pwm.m` and `rotor.N.pwm.b` are updated with the results. The PWM parameters `rotor.N.pwm` is also updated according to the model.  
(If auto-tuning is activated this parameter will be adjusted automatically over time.)

```
lhtx> pwmcal
Going dark...
Calibrating coarse PWM slope/intercept model (any key to stop).
400.0 ppm stability tolerance.
4000 stabilising time-out spins. 400 measurement spins.
Rotor 0 answering PWM = 500: 106732.2 ppm 812.3 ppm 99.8 ppm
Rotor 0 @ PWM = 500 (k = 400) 10.791 ms 92.671 Hz, sigma = 1.692 us (156.8 ppm)
Rotor 0 answering PWM = 2500: 257109.8 ppm 339.3 ppm
Rotor 0 @ PWM = 2500 (k = 400) 24.427 ms 40.938 Hz, sigma = 2.071 us (84.8 ppm)
Rotor 0 frequency[500] = 92.670685, frequency[2500] = 40.937935.
Rotor 0 PWM intercept = 105.603874 (was 96.644699).
Rotor 0 PWM slope = -0.025866 (was -0.023679).
Rotor 0 PWM computed value = 1763 (was 1756).
Rotor 1 answering PWM = 500: 102559.8 ppm 714.9 ppm 116.6 ppm
Rotor 1 @ PWM = 500 (k = 400) 10.927 ms 91.519 Hz, sigma = 1.375 us (125.8 ppm)
Rotor 1 answering PWM = 2500: 254863.9 ppm 241.7 ppm
Rotor 1 @ PWM = 2500 (k = 400) 24.793 ms 40.334 Hz, sigma = 3.269 us (131.9 ppm)
Rotor 1 frequency[500] = 91.519402, frequency[2500] = 40.333836.
Rotor 1 PWM intercept = 104.315796 (was 95.056046).
Rotor 1 PWM slope = -0.025593 (was -0.023317).
Rotor 1 PWM computed value = 1731 (was 1722).
Waiting for rotor lock...
PWM calibration complete.
```


## pwmopt
Tries to find the optimal PWM value for the currently selected rotation frequency.  
Other than `pwmcal` which only computes a good approximation for the optimal pulse width, `pwmopt` actually tries spinning the rotors with different pulse widths (binary search) until the selected rotation frequency has been found. The parameters `rotor.N.pwm` are then updated with the result.  
(If auto-tuning is activated this parameter will be adjusted automatically over time.)


```
lhtx> pwmopt
Going dark...
Optimising coarse PWM set-points (any key to stop).
400.0 ppm stability tolerance.
4000 stabilising time-out spins. 200 measurement spins.
Target rotor frequency: 60.0 Hz
Rotor 0 answering PWM = 2048: 37810.1 ppm 989.3 ppm 5.9 ppm
Rotor 0 @ PWM = 2048 (bit 11) (k = 200) 51.964 Hz, (19.0 ppm) DOWN
Rotor 0 answering PWM = 1024: 105412.9 ppm 4693.8 ppm 376.8 ppm
Rotor 0 @ PWM = 1024 (bit 10) (k = 200) 78.767 Hz, (103.9 ppm) UP
Rotor 0 answering PWM = 1536: 47528.7 ppm 3291.0 ppm 155.5 ppm
Rotor 0 @ PWM = 1536 (bit 9) (k = 200) 65.561 Hz, (24.8 ppm) UP
Rotor 0 answering PWM = 1792: 29159.4 ppm 1146.2 ppm 33.9 ppm
Rotor 0 @ PWM = 1792 (bit 8) (k = 200) 58.697 Hz, (14.4 ppm) DOWN
Rotor 0 answering PWM = 1664: 14542.7 ppm 626.4 ppm 42.5 ppm
Rotor 0 @ PWM = 1664 (bit 7) (k = 200) 62.111 Hz, (13.6 ppm) UP
Rotor 0 answering PWM = 1728: 7360.6 ppm 236.1 ppm
Rotor 0 @ PWM = 1728 (bit 6) (k = 200) 60.411 Hz, (12.3 ppm) UP
... [3 minutes later] ...
Rotor 1 @ PWM = 1706 (bit 1) (k = 200) 60.037 Hz, (22.9 ppm) UP
Rotor 1 answering PWM = 1707: 121.2 ppm
Rotor 1 @ PWM = 1707 (bit 0) (k = 200) 60.008 Hz, (23.3 ppm) UP
Rotor 1 centre coarse pwm = 1707 (was 1708).
Waiting for rotor lock...
PWM optimisation complete.
```


## ram [address] [length]
Print a hex dump of the specified RAM range or of the whole RAM.  
Address must be in 0x0000 .. 0x1FFF.  
Length must be in 0x0000 .. (0x2000-address).

```
lhtx> ram 0x690 52
10000690: 00 00 00 00 00 00 80 3F 42 45 43 30 38 45 33 38  .......?BEC08E38
100006a0: 00 00 00 00 00 00 00 00 00 00 00 01 01 00 00 00  ................
100006b0: 00 00 00 00 9C 04 00 00 9A 5B 06 00 02 00 00 00  ................
100006c0: 73 61 6C 61                                      sala
```


## lis2dh
Shows configuration and status registers of the LIS2DH (3-axis MEMS accelerometer IC).  

```
lhtx> lis2dh
LIS2DH(0x30) {
        WHO_AM_I: 0x33
        STATUS_AUX: 0xFF ( TOR:1 TDA:1 )
        OUT_TEMP_[LH]: 0x2200
        INT_COUNTER: 0x00
        TEMP_CFG_REG: 0xC0 ( TEMP_EN1:1 TEMP_EN0:1 )
        CTRL_REG1: 0x97 ( ODR[0-3]:0x9 LP_EN:0 Z_EN:1 Y_EN:1 X_EN:1 )
        CTRL_REG2: 0x00 ( HPM[01]:0x0 HPCF[12]:0x0 FDS:0 HPCLICK:0 HPIS2:0 HPIS1:0 )
        CTRL_REG3: 0x00 ( I1_CLICK:0x0 I1_AOI1:0x0 I1_AOI2:0x0 I1_DRDY1:0x0 I1_DRDY2:0x0 I1_WTM:0x0 I1_OVERRUN:0x0 )
        CTRL_REG4: 0x80 ( BDU:1 BLE:0 FS[01]:0x0 HR:0 ST[01]:0x0 SIM:0 )
        CTRL_REG5: 0x00 ( BOOT:0 FIFO_EN:0 LIR_INT1:0 D4D_INT1:0 LIR_INT2:0 D4D_INT2:0 )
        CTRL_REG6: 0x00 ( I2_CLICK:0 I2_INT1:0 I2_INT2:0 BOOT_I2:0 P2_ACT:0 H_LACTIVE:0 )
        REFERENCE: 0x00
        STATUS_REG: 0xFF ( ZYXOR:1 ZOR:1 YOR:1 XOR:1 ZYXDA:1 ZDA:1 YDA:1 XDA:1 )
        OUT_X_[LH]: 0x3F00
        OUT_Y_[LH]: 0x0240
        OUT_Z_[LH]: 0xFF40
        FIFO_CTRL_REG: 0x00 ( FM[01]:0x0 TR:0 FTH[0-4]:0x0 )
        FIFO_SRC_REG: 0x20 ( WTM:0 OVRN_FIFO:0 EMPTY:1 FSS[0-4]:0x0 )
}
```
Individual registers can be read with `lis2dh [address]`.
```
lhtx> lis2dh 0x20
LIS2DH(0x30): REG[0x20] == 0x97
```
Registers can be writte to with `lis2dh [address] [data]`.
```
lhtx> lis2dh 0x20 0x90
LIS2DH(0x30): REG[0x20] := 0x90
```


## isl58303
Shows configuration and status registers of the ISL58303 (triple laser diode driver IC).  

```
lhtx> isl58303
ISL58303() {
        CHIP_ID: 0xE2 ( CHIP_ID:0x38 )
        SERIAL_CTRL: 0x00 ( PAGE_SELECT:0x0 )
        STATUS: 0x09 ( POWER_GOOD:1 )
        SDIO: 0x00 ( SDIO_VOLTAGE:0 )
        ENABLE: 0x81 ( CHIP_EN:1 ADC_EN:0 IOUT2_PMOS_EN:0 REGULATOR_EN:1 )
        IOUT1_THRESHOLD_SCALE: 0x40
        IOUT1_COLOR_SCALE: 0x50
        IOUT1_THRESHOLD_DAC: 0x0F
        IOUT1_COLOR_DAC: 0x7D
        IOUT2_THRESHOLD_SCALE: 0x40
        IOUT2_COLOR_SCALE: 0x50
        IOUT2_THRESHOLD_DAC: 0x0B
        IOUT2_COLOR_DAC: 0x7C
        IOUT3_THRESHOLD_SCALE: 0x00
        IOUT3_COLOR_SCALE: 0x00
        IOUT3_THRESHOLD_DAC: 0x00
        IOUT3_COLOR_DAC: 0x00
        ADC_SELECT: 0x00 ( IOUT_SELECT:0x0 CODE_SELECT:0 )
        ADC_RESULT: 0x00
        ADC_CTRL_1: 0x00 ( INPUT_SELECT:0x0 CLOCK_SELECT:0x0 ADC_EN:0 START:0 )
        ADC_CTRL_2: 0x00 ( PEAK_EN:0 PEAK_POLARITY:0 SAMPLE_VSO:0 ATTN_BYPASS:0 ATE_TEST:0 )
}
```
Individual registers can be read with `isl58303 [address]`. There is also a command for writing registers but without proper documentation (which is not publicly available) doing that should be avoided! (I have no doubt that the driver can destroy the laser diodes with ease.)


## eeprom

### eeprom r <sourceAddress> <length>
Print a hex dump of the specified EEPROM range.  
SourceAddress must be in 0x000 .. 0xFC0.  
Length must be in 0x000 .. (0xFC0-sourceAddress).

```
lhtx> eeprom r 0x600 32
00000600: 43 50 52 4D 04 00 00 00 A8 0A B1 49 73 61 6C 61  CPRM.......Isala
00000610: 42 45 43 30 38 45 33 38 00 00 00 00 00 00 00 00  BEC08E38........
```


### eeprom w <destAddress> <length>
Write data to the EEPROM.  
Do not touch the memory regions 0x600-0x8FF ("Factory" partition) and 0xDC0-0xE0F (hardware id) unless you have a backup copy of the original data!  

```
lhtx> eeprom w 0xF00 8
00000f00: _ [enter your data here]
[...]
00000f00: 01 23 45 67 89 ab cd ef
```


### eeprom c <sourceAddress> <length> <destAddress>
Copy data in EEPROM from sourceAddress to destAddress.  
Do not touch the memory regions 0x600-0x8FF ("Factory" partition) and 0xDC0-0xE0F (hardware id) unless you have a backup copy of the original data!
```
lhtx> eeprom c 0xF00 8 0xF08
```


### eeprom e <destAddress> <length>
Erase data in EEPROM. (Overwrite with zero-bytes)  
Do not touch the memory regions 0x600-0x8FF ("Factory" partition) and 0xDC0-0xE0F (hardware id) unless you have a backup copy of the original data!
```
lhtx> eeprom e 0xF00 16
```


## factory

### factory load-cal
Loads all parameters (including calibration) from the "factory" partition.
Use this command to restore the device to the original state that it left the factory with.
If you've changed the contents of the "factory" partition this command won't help you get the original parameters back.


### factory save-cal
Saves the current state of all parameters (including calibration) to the "factory" partition.
Do NOT use this command unless you have a backup of the partition AND know how to restore it.
This command is only intended to be used by the manufacturer.


### factory erase-cal
Deletes the contents of the "factory" partition.
DO NOT USE unless you have a VERY good reason AND a backup of the "factory" partition AND know how to restore it.


### factory laser-test
[NEEDS MORE RESEARCH]  
Tests the laser power output(?) with APC ("automatic power correction"?) enabled and disabled.

```
lhtx> factory laser-test
laser 0: with APC enabled...
laser 0: average power 30.6 mW
laser 0: with APC disabled...
laser 0: average power 30.6 mW
laser 1: with APC enabled...
laser 1: average power 29.4 mW
laser 1: with APC disabled...
laser 1: average power 29.1 mW
```


### factory laser-sweep
[NEEDS MORE RESEARCH]


### factory laser-bias
[NEEDS MORE RESEARCH]


### factory laser-cal
[NEEDS MORE RESEARCH]


### factory led <LED>
[NEEDS MORE RESEARCH]
