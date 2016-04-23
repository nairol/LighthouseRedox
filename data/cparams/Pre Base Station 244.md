This file contains the base station's "cparams". They are variables that are stored in non-volatile memory (EEPROM).

These variables can be changed at runtime by the firmware or the user (via the USART interface).

EEPROM Offset | Address | Type | Length | Name | Default Value | Description | Writable | Has Default | Handler Address
--------------|---------|------|--------|------|---------------|-------------|----------|-------------|----------------
000 | 0x100006A0 | uint32   | 004 | sys.param_magic | 1634492787 |  | x | x | 0x0000
004 | 0x10000678 | string   | 016 | sys.name | DEFAULT | device name | x | x | 0x0000
014 | 0x1000064C | bool     | 001 | timebase.debug | false | debug enable | x | x | 0x0000
015 | 0x1000064E | enum     | 001 | timebase.mode | slave-cable | timebase mode | x | x | 0x0000
016 | 0x10000650 | uint32   | 004 | timebase.period | 800000 | timebase nominal period (ticks) | x | x | 0x0000
01A | 0x10000654 | int32    | 004 | timebase.offset | 0 | timebase phase offset for rotor PLL injection (ticks) | x | x | 0x0000
01E | 0x1000065F | bool     | 001 | timebase.optical.mask | true | local emission masking enable | x | x | 0x0000
01F | 0x10000664 | uint32   | 004 | timebase.optical.presync | 4000 | local emission masking pre-sync window time (ticks) | x | x | 0x0000
023 | 0x10000660 | uint32   | 004 | timebase.optical.offset | 20000 | optical master's timebase offset (ticks) | x | x | 0x0000
027 | 0x1000066C | float    | 004 | timebase.p | 0.05 | timebase phase filter coefficient | x | x | 0x0000
02B | 0x10000670 | float    | 004 | timebase.i | 0 | timebase phase filter coefficient | x | x | 0x0000
02F | 0x10000674 | float    | 004 | timebase.d | 1.0 | timebase phase filter coefficient | x | x | 0x0000
033 | 0x10000668 | float    | 004 | timebase.k | 0.999 | timebase phase filter coefficient | x | x | 0x0000
037 | 0x1000064D | bool     | 001 | timebase.locked | true | timebase estimator lock indication |  | x | 0x0000
038 | 0x1000064F | enum     | 001 | timebase.source | internal | timebase source |  | x | 0x0000
039 | 0x10000658 | uint32   | 004 | timebase.current | 0 | sensed current timebase period |  | x | 0x0000
03D | 0x1000065C | bool     | 001 | timebase.tdm | false | TDM mode active |  | x | 0x0000
03E | 0x100002A4 | bool     | 001 | corr.enable | true | correlator enable | x | x | 0x0000
03F | 0x100002A5 | bool     | 001 | corr.debug | false | debug enable | x | x | 0x0000
040 | 0x100002B0 | float    | 004 | corr.k | 0.999 | correlator period filter coefficient | x | x | 0x0000
044 | 0x100002AC | uint32   | 004 | corr.tolerance | 400 | correlator period tolerance (ticks) | x | x | 0x0000
048 | 0x100002B4 | uint32   | 004 | corr.settle | 300 | correlator settling (periods) | x | x | 0x0000
04C | 0x100002B8 | uint32   | 004 | corr.timeout | 300 | correlator signal time-out (periods) | x | x | 0x0000
050 | 0x100002BC | enum     | 001 | corr.status | disabled | correlator status |  | x | 0x0000
051 | 0x100002C8 | float    | 004 | corr.period | 0 | master's estimated period (ticks) |  | x | 0x0000
055 | 0x100002C4 | uint32   | 004 | corr.missed | 0 | missed periods |  | x | 0x0000
059 | 0x100005AC | bool     | 001 | carrier.enable | true | subcarrier enable | x | x | 0x0000
05A | 0x100005AD | bool     | 001 | carrier.debug | false | debug enable | x | x | 0x0000
05B | 0x100005B0 | uint32   | 004 | carrier.frequency | 1843200 | Carrier frequency (Hz) | x | x | 0x36E5
05F | 0x100003A5 | bool     | 001 | laser.enable | true | laser enable | x | x | 0x0000
060 | 0x100003A4 | bool     | 001 | laser.debug | false | debug enable | x | x | 0x0000
061 | 0x100003A6 | bool     | 001 | laser.interlock | true | laser safety interlock enable | x | x | 0x0000
062 | 0x100003A7 | bool     | 001 | laser.apc | true | laser APC enable | x | x | 0x0000
063 | 0x100003A8 | float    | 004 | laser.b1 | 0.95 | laser power sense filter coeff | x | x | 0x0000
067 | 0x100003AC | bool     | 001 | laser.0.enable | true | laser enable | x | x | 0x0000
068 | 0x100003AD | bool     | 001 | laser.0.fullspin | false | laser full spin illumination enable | x | x | 0x0000
069 | 0x100003B0 | float    | 004 | laser.0.start | 32.0 | laser turn-on offset (degrees) | x | x | 0x0000
06D | 0x100003B4 | float    | 004 | laser.0.length | 124.0 | laser on-time length (degrees) | x | x | 0x0000
071 | 0x100003B8 | uint8    | 001 | laser.0.bias | 0 | laser threshold current | x | x | 0x0000
072 | 0x100003B9 | uint8    | 001 | laser.0.current | 190 | laser drive current | x | x | 0x0000
073 | 0x100003BC | float    | 004 | laser.0.pwr | 30 | laser power (mW) | x | x | 0x0000
077 | 0x100003C8 | float    | 004 | laser.0.pwr.b | 9.0 | FPD offset | x | x | 0x0000
07B | 0x100003C0 | float    | 004 | laser.0.pwr.m | 0.434 | FPD calibration | x | x | 0x0000
07F | 0x100003BA | uint16   | 002 | laser.0.pwr.gain | 3 | FPD amplifier gain set | x | x | 0x0000
081 | 0x100003CC | float    | 004 | laser.0.pwr.detected | 0 | FPD amplifier output |  | x | 0x0000
085 | 0x100003D0 | float    | 004 | laser.0.pwr.average | 0 | FPD power measurement average |  | x | 0x0000
089 | 0x100003D4 | bool     | 001 | laser.1.enable | true | laser enable | x | x | 0x0000
08A | 0x100003D5 | bool     | 001 | laser.1.fullspin | false | laser full spin illumination enable | x | x | 0x0000
08B | 0x100003D8 | float    | 004 | laser.1.start | 25.0 | laser turn-on offset (degrees) | x | x | 0x0000
08F | 0x100003DC | float    | 004 | laser.1.length | 124.0 | laser on-time length (degrees) | x | x | 0x0000
093 | 0x100003E0 | uint8    | 001 | laser.1.bias | 0 | laser threshold current | x | x | 0x0000
094 | 0x100003E1 | uint8    | 001 | laser.1.current | 190 | laser drive current | x | x | 0x0000
095 | 0x100003E4 | float    | 004 | laser.1.pwr | 30 | laser power (mW) | x | x | 0x0000
099 | 0x100003F0 | float    | 004 | laser.1.pwr.b | 9.0 | FPD offset | x | x | 0x0000
09D | 0x100003E8 | float    | 004 | laser.1.pwr.m | 0.434 | FPD calibration | x | x | 0x0000
0A1 | 0x100003E2 | uint16   | 002 | laser.1.pwr.gain | 3 | FPD amplifier gain set | x | x | 0x0000
0A3 | 0x100003F4 | float    | 004 | laser.1.pwr.detected | 0 | FPD amplifier output |  | x | 0x0000
0A7 | 0x100003F8 | float    | 004 | laser.1.pwr.average | 0 | FPD power measurement average |  | x | 0x0000
0AB | 0x1000049C | bool     | 001 | rotor.0.enable | true | motor enable | x | x | 0x0000
0AC | 0x1000049D | bool     | 001 | rotor.0.debug | false | debug enable | x | x | 0x0000
0AD | 0x1000049E | uint16   | 002 | rotor.0.pwm | 2000 | coarse PWM speed control | x | x | 0x0000
0AF | 0x100004A0 | float    | 004 | rotor.0.pwm.m | -0.0260 | coarse PWM model slope | x | x | 0x0000
0B3 | 0x100004A4 | float    | 004 | rotor.0.pwm.b | 110.0 | coarse PWM model intercept | x | x | 0x0000
0B7 | 0x100004B0 | bool     | 001 | rotor.0.pll.debug | false | pll debug enable | x | x | 0x0000
0B8 | 0x100004B1 | bool     | 001 | rotor.0.pll.enable | true | pll enable | x | x | 0x0000
0B9 | 0x100004B4 | float    | 004 | rotor.0.pll.p | 0.02 | loop coefficient | x | x | 0x0000
0BD | 0x100004B8 | float    | 004 | rotor.0.pll.i | 0.001 | loop coefficient | x | x | 0x0000
0C1 | 0x100004BC | float    | 004 | rotor.0.pll.d | 0.2 | loop coefficient | x | x | 0x0000
0C5 | 0x100004C0 | int32    | 004 | rotor.0.pll.offset | -200000 | phase offset (ticks) | x | x | 0x0000
0C9 | 0x100004C4 | float    | 004 | rotor.0.tolerance | 300.0 | lock tolerance (ppm) | x | x | 0x0000
0CD | 0x100004C8 | uint32   | 004 | rotor.0.settle | 30 | lock settle time (spins) | x | x | 0x0000
0D1 | 0x100004CC | bool     | 001 | rotor.0.autoclear | true | statistics automatic clearing enable | x | x | 0x0000
0D2 | 0x100004A8 | bool     | 001 | rotor.0.pwm.auto | true | auto coarse PWM tuning enable | x | x | 0x0000
0D3 | 0x100004AC | float    | 004 | rotor.0.pwm.i | 0.001 | auto PWM tuning gain | x | x | 0x0000
0D7 | 0x10000504 | bool     | 001 | rotor.1.enable | true | motor enable | x | x | 0x0000
0D8 | 0x10000505 | bool     | 001 | rotor.1.debug | false | debug enable | x | x | 0x0000
0D9 | 0x10000506 | uint16   | 002 | rotor.1.pwm | 2000 | coarse PWM speed control | x | x | 0x0000
0DB | 0x10000508 | float    | 004 | rotor.1.pwm.m | -0.0260 | coarse PWM model slope | x | x | 0x0000
0DF | 0x1000050C | float    | 004 | rotor.1.pwm.b | 110.0 | coarse PWM model intercept | x | x | 0x0000
0E3 | 0x10000518 | bool     | 001 | rotor.1.pll.debug | false | pll debug enable | x | x | 0x0000
0E4 | 0x10000519 | bool     | 001 | rotor.1.pll.enable | true | pll enable | x | x | 0x0000
0E5 | 0x1000051C | float    | 004 | rotor.1.pll.p | 0.02 | loop coefficient | x | x | 0x0000
0E9 | 0x10000520 | float    | 004 | rotor.1.pll.i | 0.001 | loop coefficient | x | x | 0x0000
0ED | 0x10000524 | float    | 004 | rotor.1.pll.d | 0.2 | loop coefficient | x | x | 0x0000
0F1 | 0x10000528 | int32    | 004 | rotor.1.pll.offset | 200000 | phase offset (ticks) | x | x | 0x0000
0F5 | 0x1000052C | float    | 004 | rotor.1.tolerance | 300.0 | lock tolerance (ppm) | x | x | 0x0000
0F9 | 0x10000530 | uint32   | 004 | rotor.1.settle | 30 | lock settle time (spins) | x | x | 0x0000
0FD | 0x10000534 | bool     | 001 | rotor.1.autoclear | true | statistics automatic clearing enable | x | x | 0x0000
0FE | 0x10000510 | bool     | 001 | rotor.1.pwm.auto | true | auto coarse PWM tuning enable | x | x | 0x0000
0FF | 0x10000514 | float    | 004 | rotor.1.pwm.i | 0.001 | auto PWM tuning gain | x | x | 0x0000
103 | 0x100006A4 | bool     | 001 | stats.enable | false | statistics enable | x | x | 0x0000
104 | 0x100006A6 | uint16   | 002 | stats.interval | 1 | statistics dumping interval (seconds) | x | x | 0xBBFD
106 | 0x100005DC | bool     | 001 | ootx.enable | true | sync transmitter enable | x | x | 0x0000
107 | 0x100005DE | bool     | 001 | ootx.debug | false | debug enable | x | x | 0x0000
108 | 0x100005DD | bool     | 001 | ootx.lock_required | true | transmit sync only when rotors locked | x | x | 0x0000
109 | 0x100005E0 | uint16   | 002 | ootx.j0 | 3000 | pulse length (ticks) | x | x | 0x0000
10B | 0x100005E4 | uint16   | 002 | ootx.j1 | 4000 | pulse length (ticks) | x | x | 0x0000
10D | 0x100005E8 | uint16   | 002 | ootx.k0 | 3500 | pulse length (ticks) | x | x | 0x0000
10F | 0x100005EC | uint16   | 002 | ootx.k1 | 4500 | pulse length (ticks) | x | x | 0x0000
111 | 0x100005F0 | uint16   | 002 | ootx.j2 | 5000 | pulse length (ticks) | x | x | 0x0000
113 | 0x100005F4 | uint16   | 002 | ootx.j3 | 6000 | pulse length (ticks) | x | x | 0x0000
115 | 0x100005F8 | uint16   | 002 | ootx.k2 | 5500 | pulse length (ticks) | x | x | 0x0000
117 | 0x100005FC | uint16   | 002 | ootx.k3 | 6500 | pulse length (ticks) | x | x | 0x0000
119 | 0x100005DF | bool     | 001 | ootx.stretch | true | pulse stretching enable | x | x | 0x0000
11A | 0x100006D0 | uint8    | 001 | mode.count | 3 | num channels | x | x | 0x0000
11B | 0x100006D1 | uint8    | 001 | mode.current | 0 | current channel | x | x | 0x0000
11C | 0x100006D8 | uint32   | 004 | mode.0.period | 800000 | channel period (ticks) | x | x | 0x0000
120 | 0x100006EC | uint32   | 004 | mode.1.period | 800000 | channel period (ticks) | x | x | 0x0000
124 | 0x10000700 | uint32   | 004 | mode.2.period | 800000 | channel period (ticks) | x | x | 0x0000
128 | 0x10000714 | uint32   | 004 | mode.3.period | 800000 | channel period (ticks) | x | x | 0x0000
12C | 0x10000728 | uint32   | 004 | mode.4.period | 765053 | channel period (ticks) | x | x | 0x0000
130 | 0x1000073C | uint32   | 004 | mode.5.period | 836543 | channel period (ticks) | x | x | 0x0000
134 | 0x10000750 | uint32   | 004 | mode.6.period | 874755 | channel period (ticks) | x | x | 0x0000
138 | 0x10000764 | uint32   | 004 | mode.7.period | 914713 | channel period (ticks) | x | x | 0x0000
13C | 0x100006E0 | enum     | 001 | mode.0.timebase.mode | slave-cable | timebase mode | x | x | 0x0000
13D | 0x100006F4 | enum     | 001 | mode.1.timebase.mode | master-tdm | timebase mode | x | x | 0x0000
13E | 0x10000708 | enum     | 001 | mode.2.timebase.mode | slave-optical | timebase mode | x | x | 0x0000
13F | 0x1000071C | enum     | 001 | mode.3.timebase.mode | master-sync | timebase mode | x | x | 0x0000
140 | 0x10000730 | enum     | 001 | mode.4.timebase.mode | master-async | timebase mode | x | x | 0x0000
141 | 0x10000744 | enum     | 001 | mode.5.timebase.mode | master-async | timebase mode | x | x | 0x0000
142 | 0x10000758 | enum     | 001 | mode.6.timebase.mode | master-async | timebase mode | x | x | 0x0000
143 | 0x1000076C | enum     | 001 | mode.7.timebase.mode | master-async | timebase mode | x | x | 0x0000
144 | 0x100006DC | int32    | 004 | mode.0.timebase.offset | 0 | phase offset (ticks) | x | x | 0x0000
148 | 0x100006F0 | int32    | 004 | mode.1.timebase.offset | 20000 | phase offset (ticks) | x | x | 0x0000
14C | 0x10000704 | int32    | 004 | mode.2.timebase.offset | 0 | phase offset (ticks) | x | x | 0x0000
150 | 0x10000718 | int32    | 004 | mode.3.timebase.offset | 0 | phase offset (ticks) | x | x | 0x0000
154 | 0x1000072C | int32    | 004 | mode.4.timebase.offset | 0 | phase offset (ticks) | x | x | 0x0000
158 | 0x10000740 | int32    | 004 | mode.5.timebase.offset | 0 | phase offset (ticks) | x | x | 0x0000
15C | 0x10000754 | int32    | 004 | mode.6.timebase.offset | 0 | phase offset (ticks) | x | x | 0x0000
160 | 0x10000768 | int32    | 004 | mode.7.timebase.offset | 0 | phase offset (ticks) | x | x | 0x0000
164 | 0x100006E1 | enum     | 001 | mode.0.ootx | medium | ootx pulse lengths | x | x | 0x0000
165 | 0x100006F5 | enum     | 001 | mode.1.ootx | medium | ootx pulse lengths | x | x | 0x0000
166 | 0x10000709 | enum     | 001 | mode.2.ootx | medium | ootx pulse lengths | x | x | 0x0000
167 | 0x1000071D | enum     | 001 | mode.3.ootx | long | ootx pulse lengths | x | x | 0x0000
168 | 0x10000731 | enum     | 001 | mode.4.ootx | short | ootx pulse lengths | x | x | 0x0000
169 | 0x10000745 | enum     | 001 | mode.5.ootx | short | ootx pulse lengths | x | x | 0x0000
16A | 0x10000759 | enum     | 001 | mode.6.ootx | short | ootx pulse lengths | x | x | 0x0000
16B | 0x1000076D | enum     | 001 | mode.7.ootx | short | ootx pulse lengths | x | x | 0x0000
16C | 0x100006D4 | uint8    | 001 | mode.0.label | 65 | ASCII char label | x | x | 0x0000
16D | 0x100006E8 | uint8    | 001 | mode.1.label | 98 | ASCII char label | x | x | 0x0000
16E | 0x100006FC | uint8    | 001 | mode.2.label | 99 | ASCII char label | x | x | 0x0000
16F | 0x10000710 | uint8    | 001 | mode.3.label | 76 | ASCII char label | x | x | 0x0000
170 | 0x10000724 | uint8    | 001 | mode.4.label | 49 | ASCII char label | x | x | 0x0000
171 | 0x10000738 | uint8    | 001 | mode.5.label | 50 | ASCII char label | x | x | 0x0000
172 | 0x1000074C | uint8    | 001 | mode.6.label | 51 | ASCII char label | x | x | 0x0000
173 | 0x10000760 | uint8    | 001 | mode.7.label | 52 | ASCII char label | x | x | 0x0000
174 | 0x100006E4 | uint32   | 004 | mode.0.carrier | 1843200 | carrier freq (Hz) | x | x | 0x0000
178 | 0x100006F8 | uint32   | 004 | mode.1.carrier | 1843200 | carrier freq (Hz) | x | x | 0x0000
17C | 0x1000070C | uint32   | 004 | mode.2.carrier | 1843200 | carrier freq (Hz) | x | x | 0x0000
180 | 0x10000720 | uint32   | 004 | mode.3.carrier | 1843200 | carrier freq (Hz) | x | x | 0x0000
184 | 0x10000734 | uint32   | 004 | mode.4.carrier | 1843200 | carrier freq (Hz) | x | x | 0x0000
188 | 0x10000748 | uint32   | 004 | mode.5.carrier | 1843200 | carrier freq (Hz) | x | x | 0x0000
18C | 0x1000075C | uint32   | 004 | mode.6.carrier | 1843200 | carrier freq (Hz) | x | x | 0x0000
190 | 0x10000770 | uint32   | 004 | mode.7.carrier | 1843200 | carrier freq (Hz) | x | x | 0x0000
194 | 0x100005B8 | bool     | 001 | accel.enable | true | accelerometer enable | x | x | 0x0000
195 | 0x100005B9 | bool     | 001 | accel.debug | false | debug enable | x | x | 0x0000
196 | 0x100005BC | float    | 004 | accel.b1 | 0.97 | filter coeff | x | x | 0x0000
19A | 0x100005C8 | float    | 004 | accel.x | 0 | accelerometer vector |  | x | 0x0000
19E | 0x100005CC | float    | 004 | accel.y | 0 | accelerometer vector |  | x | 0x0000
1A2 | 0x100005D0 | float    | 004 | accel.z | 0 | accelerometer vector |  | x | 0x0000
1A6 | 0x100005D4 | float    | 004 | accel.magnitude | 0 | acceleration (m/s^2) |  | x | 0x0000
1AA | 0x100005BA | bool     | 001 | accel.is_gravity | false | is acceleration consistent with gravity |  | x | 0x0000
1AB | 0x100005D8 | int8     | 001 | accel.dir_x | 0 | orientation vector |  | x | 0x0000
1AC | 0x100005D9 | int8     | 001 | accel.dir_y | 0 | orientation vector |  | x | 0x0000
1AD | 0x100005DA | int8     | 001 | accel.dir_z | 0 | orientation vector |  | x | 0x0000
1AE | 0x10000688 | bool     | 001 | sys.warnings | false | warnings and info enable | x | x | 0x0000
1AF | 0x00000000 | reserved | 004 |  |  |  | x | x | 0x0000
1B3 | 0x10000689 | bool     | 001 | sys.emission_enable | true | optical output enable |  | x | 0x0000
1B4 | 0x10000694 | uint32   | 004 | sys.uptime | 0 | system uptime (seconds) |  | x | 0x0000
1B8 | 0x1000068A | bool     | 001 | sys.standby | false | standby power down enable |  | x | 0xCEE5
1B9 | 0x1000068B | bool     | 001 | sys.identify | false | identify blink enable |  | x | 0x0000
1BA | 0x100003C4 | float    | 004 | laser.0.pwr.b2 | 0 | FPD intercept | x | x | 0x0000
1BE | 0x100003EC | float    | 004 | laser.1.pwr.b2 | 0 | FPD intercept | x | x | 0x0000
1C2 | 0x00000000 | reserved | 120 |  |  |  | x | x | 0x0000
23A | 0x100006A8 | float    | 004 | fcal.0.tilt | 0 | tilt | x |  | 0x3269
23E | 0x100006AC | float    | 004 | fcal.0.phase | 0 | phase | x |  | 0x3269
242 | 0x100006B0 | float    | 004 | fcal.0.curve | 0 | curve | x |  | 0x3269
246 | 0x100006B4 | float    | 004 | fcal.0.gibphase | 0 | gibbous phase | x |  | 0x3269
24A | 0x100006B8 | float    | 004 | fcal.0.gibmag | 0 | gibbous magnitude | x |  | 0x3269
24E | 0x100006BC | float    | 004 | fcal.1.tilt | 0 | tilt | x |  | 0x3269
252 | 0x100006C0 | float    | 004 | fcal.1.phase | 0 | phase | x |  | 0x3269
256 | 0x100006C4 | float    | 004 | fcal.1.curve | 0 | curve | x |  | 0x3269
25A | 0x100006C8 | float    | 004 | fcal.1.gibphase | 0 | gibbous phase | x |  | 0x3269
25E | 0x100006CC | float    | 004 | fcal.1.gibmag | 0 | gibbous magnitude | x |  | 0x3269

Name | Possible Enum Values
-----|---------------------
timebase.mode | "master-tdm", "master-sync", "master-async", "slave-cable", "slave-optical"
mode.7.timebase.mode | "master-tdm", "master-sync", "master-async", "slave-cable", "slave-optical"
mode.5.ootx | "short", "medium", "long"
corr.status | "disabled", "searching", "syncing", "locked", "lost"
mode.6.ootx | "short", "medium", "long"
mode.6.timebase.mode | "master-tdm", "master-sync", "master-async", "slave-cable", "slave-optical"
timebase.source | "internal", "cable", "optical"
mode.1.ootx | "short", "medium", "long"
mode.2.ootx | "short", "medium", "long"
mode.3.ootx | "short", "medium", "long"
mode.4.ootx | "short", "medium", "long"
mode.2.timebase.mode | "master-tdm", "master-sync", "master-async", "slave-cable", "slave-optical"
mode.4.timebase.mode | "master-tdm", "master-sync", "master-async", "slave-cable", "slave-optical"
mode.0.ootx | "short", "medium", "long"
mode.0.timebase.mode | "master-tdm", "master-sync", "master-async", "slave-cable", "slave-optical"
mode.1.timebase.mode | "master-tdm", "master-sync", "master-async", "slave-cable", "slave-optical"
mode.3.timebase.mode | "master-tdm", "master-sync", "master-async", "slave-cable", "slave-optical"
mode.5.timebase.mode | "master-tdm", "master-sync", "master-async", "slave-cable", "slave-optical"
mode.7.ootx | "short", "medium", "long"
