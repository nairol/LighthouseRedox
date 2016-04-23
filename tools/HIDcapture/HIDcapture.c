/*
** This is a program to help me figure out the HID reports the USB devices that are part of the HTC Vive are sending to the PC.
** 
** In it's current state it is not usable by non-programmers and it's quite user unfriendly. :P
** All of the "feature reports" are still missing. Also most of the wireless dongle protocol is currently unknown.
**
** To use this you have to start SteamVR once and then exit it again.
** This limitation is there because I don't know the proper way to initialize the hardware yet.
** 
** https://github.com/nairol/LighthouseRedox
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hidapi-windows-0.8.0-rc1/hid.c"
// Yes I know this is very hackish... Should be easy to change for Linux/OSX support.

#include "hid_reports.h"

uint32_t endian(uint8_t buf[])
{
	return buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3];
}

uint16_t endian16(uint8_t buf[])
{
	return buf[0] << 8 | buf[1];
}

void printBinString(char format[], uint32_t num)
{
	char binbuf[33];
	itoa(num, binbuf, 2);
	printf(format, binbuf);
}

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[128];
	hid_device *handle;
	char* selectedPath = NULL;

	struct hid_device_info *devs, *cur_dev;

	printf("| VID  | PID  |  Manufacturer  |      Product       | Interface | Serial Number |\n");
	printf("|------|------|----------------|--------------------|-----------|---------------|\n");

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;
	while (cur_dev) {
		if (cur_dev->vendor_id == 0x28de || cur_dev->vendor_id == 0x0bb4)
		{
			// 
			// SELECT WHAT YOU WANT TO SEE BY UNCOMMENTING ONE OF THE IF STATEMENTS BELOW
			//
			printf("| %04hx | %04hx | %-14ls | %-18ls | %9d | %-13ls |", cur_dev->vendor_id, cur_dev->product_id, cur_dev->manufacturer_string, cur_dev->product_string, cur_dev->interface_number, cur_dev->serial_number);
			//if (cur_dev->vendor_id == 0x28de && cur_dev->product_id == 0x2012 && cur_dev->interface_number == 2) // Wired controller, buttons + touch pad
			//if (cur_dev->vendor_id == 0x28de && cur_dev->product_id == 0x2012 && cur_dev->interface_number == 0) // Wired controller, IMU
			//if (cur_dev->vendor_id == 0x28de && cur_dev->product_id == 0x2012 && cur_dev->interface_number == 1) // Wired controller, optical
			//if (cur_dev->vendor_id == 0x28de && cur_dev->product_id == 0x2000 && cur_dev->interface_number == 1) // Vive, optical
			if (cur_dev->vendor_id == 0x28de && cur_dev->product_id == 0x2000 && cur_dev->interface_number == 0) // Vive, IMU
			//if (cur_dev->vendor_id == 0x0bb4 && cur_dev->product_id == 0x2c87) // Vive, button + face proximity
			//if (cur_dev->vendor_id == 0x28de && cur_dev->product_id == 0x2101) // Wireless controller dongle
			{
				if (!selectedPath)
				{
					selectedPath = cur_dev->path;
					printf(" <- selected");
				}
			}
			printf("\n");
		}
		cur_dev = cur_dev->next;
	}

	if (selectedPath == NULL)
	{
		printf("Could not find suitable HID device!\n");
		exit(0);
	}

	handle = hid_open_path(selectedPath);
	hid_free_enumeration(devs);
	if (!handle)
	{
		printf("Couldn't open HID device! Close SteamVR if it is running.\n");
		exit(0);
	}

	while (TRUE)
	{
		// Read requested state
		res = hid_read(handle, buf, 65);
		if (res < 0)
		{
			printf("hid_read failed!\n");
			break;
		}

		//if (buf[0] != 36) continue;
		//printf("%3d: ", buf[0]);

		if (buf[0] == 1)
		{
			struct VID28DE_PID2012_IF2_IN1* r = (struct VID28DE_PID2012_IF2_IN1*)buf;
			printf("type=%4x seq=%7d buttons=%08x trigger/batt.voltage=%5u battery=%3u ID=%8x touch(H/V)=%6d,%6d trigValHiRes=%6d trigValRaw=%5d unkD=%d\n",
				r->type, r->sequenceNumber, r->buttons, r->triggerValue, r->battery, r->hardwareID,
				r->touchpadHorizontal, r->touchpadVertical, r->triggerValueHighRes, r->triggerValueRaw_Maybe, r->unknown_G);
		}
		else if (buf[0] == 3)
		{
			struct VID0BB4_PID2C87_IN3* r = (struct VID0BB4_PID2C87_IN3*)buf;
			printf("Btn=%1d FaceProximity=%5u   unkA=", r->buttons, r->faceProximity);
			for (int i = 0; i < 7; ++i) { printf("%02x", r->unknown_A[i]); }
			printf(" unkB=");
			for (int i = 0; i < 3; ++i) { printf("%02x", r->unknown_B[i]); }
			printf(" unkC=%02x unkD=%02x unkE=", r->unknown_C, r->unknown_D);
			for (int i = 0; i < 2; ++i) { printf("%02x", r->unknown_E[i]); }
			printf(" unkF=");
			for (int i = 0; i < 46; ++i) { printf("%02x", r->unknown_F[i]); }
			printf("\n");
		}
		else if (buf[0] == 32)
		{
			//
			// BUGGY! Samples can be returned out of order!
			// Needs a more complex seq check and sorting...
			//
			struct VID28DE_PID2012_IF0_IN32* r = (struct VID28DE_PID2012_IF0_IN32*)buf;
			static int lastSeq[3] = {-1, -1, -1};

			if (r->samples[0].seq != lastSeq[0])
			{
				printf("Ax=%6d  Ay=%6d  Az=%6d  Gx=%6d  Gy=%6d  Gz=%6d  t=%10u  s=%3u .  \n", r->samples[0].acc_x, r->samples[0].acc_y, r->samples[0].acc_z, r->samples[0].gyro_x, r->samples[0].gyro_y, r->samples[0].gyro_z, r->samples[0].time, r->samples[0].seq);
				lastSeq[0] = r->samples[0].seq;
			}
			if (r->samples[1].seq != lastSeq[1])
			{
				printf("Ax=%6d  Ay=%6d  Az=%6d  Gx=%6d  Gy=%6d  Gz=%6d  t=%10u  s=%3u  . \n", r->samples[1].acc_x, r->samples[1].acc_y, r->samples[1].acc_z, r->samples[1].gyro_x, r->samples[1].gyro_y, r->samples[1].gyro_z, r->samples[1].time, r->samples[1].seq);
				lastSeq[1] = r->samples[1].seq;
			}
			if (r->samples[2].seq != lastSeq[2])
			{
				printf("Ax=%6d  Ay=%6d  Az=%6d  Gx=%6d  Gy=%6d  Gz=%6d  t=%10u  s=%3u   .\n", r->samples[2].acc_x, r->samples[2].acc_y, r->samples[2].acc_z, r->samples[2].gyro_x, r->samples[2].gyro_y, r->samples[2].gyro_z, r->samples[2].time, r->samples[2].seq);
				lastSeq[2] = r->samples[2].seq;
			}
		}
		else if (buf[0] == 33)
		{
			struct VID28DE_PID2012_IF1_IN33* r = (struct VID28DE_PID2012_IF1_IN33*)buf;
			for (int i = 0; i < 7; ++i)
			{
				if (r->events[i].sensorID != 0xFFFF)
				{
					printf("S%2d: %10u %5u  ", r->events[i].sensorID, r->events[i].pulseTime, r->events[i].pulseLength);
				}
			}
			printf("\n");
		}
		else if (buf[0] == 35)
		{
			res = 30;

			if ((*(int16_t*)&buf[12]) < 50 && (*(int16_t*)&buf[12]) > -50)
				continue;

			/*
			for (int i = 0; i < res; ++i)
			{
				printf("%03u ", buf[i]);
			}
			printf("\n");
			continue;
			*/

			/////// TODO: COMPARE WITH LAST REPORT AND DON'T SHOW UNCHANGED BYTES FROM THE RIGHT

			uint32_t time = endian(&buf[1]);
			double timed = time / 48000000.0;
			printf("%10u ", time);

			//printf("%02X ", buf[5]);
			//printBinString("%08s ", buf[5]);
			
			/*
			// Decimal Output
			printf("%6d ", *(int16_t*)&buf[6]);
			printf("%6d ", *(int16_t*)&buf[8]);
			printf("%6d ", *(int16_t*)&buf[10]);
			printf("%6d ", *(int16_t*)&buf[12]);
			printf("%6d ", *(int16_t*)&buf[14]);
			printf("%6d ", *(int16_t*)&buf[16]);
			*/

			for (int i = 5; i < res; ++i)
			{
				printf("%02x ", buf[i]);
				//printBinString("%08s ", buf[i]);
			}
			printf("\n");
		}
		else if (buf[0] == 36)
		{ 
			res = 59;
			printf("\n");
			// UNKNOWN REPORT
		}
		else if (buf[0] == 37)
		{
			struct VID28DE_PID2000_IF1_IN37* r = (struct VID28DE_PID2000_IF1_IN37*)buf;
			for (int i = 0; i < 9; ++i)
			{
				if (r->events[i].sensorID != 0xFF)
				{
					printf("S%2d: %10u %5u  ", r->events[i].sensorID, r->events[i].pulseTime, r->events[i].pulseLength);
				}
			}
			printf("\n");
		}
		else
		{
			for (int i = 0; i < res; ++i)
			{
				printf("%02x ", buf[i]);
			}
			printf("\n");
		}


	}
	hid_close(handle);
	hid_exit();

	return 0;
}