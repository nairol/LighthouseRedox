#pragma once
#include <stdint.h>

#pragma pack(1)

struct IMUSample
{
	int16_t acc_x;
	int16_t acc_y;
	int16_t acc_z;
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
	uint32_t time;
	uint8_t seq;
};

struct VID28DE_PID2012_IF0_IN32 //also HMD!
{
	uint8_t reportID;
	struct IMUSample samples[3];
};

struct OpticalPulse_Controller
{
	uint16_t sensorID;
	uint16_t pulseLength;
	uint32_t pulseTime;
};

struct VID28DE_PID2012_IF1_IN33
{
	uint8_t reportID;
	struct OpticalPulse_Controller events[7];
	uint8_t unknownA;
};

struct VID28DE_PID2012_IF2_IN1
{
	uint8_t reportID;
	uint8_t unknown_A;
	uint16_t type;
	uint32_t sequenceNumber;
	uint32_t buttons;
	uint16_t triggerValue;
	uint8_t battery; // bit7: charging? bit6..0: percentage
	uint8_t unknown_C;
	uint32_t hardwareID;
	int16_t touchpadHorizontal;
	int16_t touchpadVertical;
	uint8_t unknown_D[2];
	int16_t triggerValueHighRes;
	uint8_t unknown_E[24];
	uint16_t triggerValueRaw_Maybe;
	uint8_t unknown_F[8];
	uint8_t unknown_G;
	uint8_t unknown_H;
};

struct OpticalPulse_HMD
{
	uint8_t sensorID;
	uint16_t pulseLength;
	uint32_t pulseTime;
};

struct VID28DE_PID2000_IF1_IN37
{
	uint8_t reportID;
	struct OpticalPulse_HMD events[9];
};

struct VID0BB4_PID2C87_IN3
{
	uint8_t reportID;
	uint8_t unknown_A[7];
	uint8_t buttons;
	uint8_t unknown_B[3];
	uint8_t unknown_C; //TODO: faceProximityChange
	uint8_t unknown_D;
	uint16_t faceProximity;
	uint8_t unknown_E[2];
	uint8_t unknown_F[46];
};

struct VID28DE_PID2101_IN35
{
	uint8_t a; //debug
};

struct VID28DE_PID2101_IN36
{
	uint8_t a; //debug
};

#pragma pack()