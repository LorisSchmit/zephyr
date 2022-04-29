/*
 * Copyright (c) 2018 Analog Devices Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <stdio.h>
#include <sys/__assert.h>

#define DELAY_WITHOUT_TRIGGER K_SECONDS(1)

#define ATMEL_AT30TS75A 1

#if ATMEL_AT30TS75A
#define ATMEL_AT30TS75A_CONFIG_SHUTDOWN_MODE_BIT 0
#define ATMEL_AT30TS75A_CONFIG_ALARM_THERMO_MODE_BIT 1
#define ATMEL_AT30TS75A_CONFIG_ALERT_PIN_POL_BIT 2
#define ATMEL_AT30TS75A_CONFIG_FAULT_TOL_QUEUE_BITS 3
#define ATMEL_AT30TS75A_CONFIG_RESOLUTION_BITS 5
#define ATMEL_AT30TS75A_CONFIG_ONE_SHOT_MODE_BIT 7
#endif

static const char *now_str(void)
{
	static char buf[16]; /* ...HH:MM:SS.MMM */
	uint32_t now = k_uptime_get_32();
	unsigned int ms = now % MSEC_PER_SEC;
	unsigned int s;
	unsigned int min;
	unsigned int h;

	now /= MSEC_PER_SEC;
	s = now % 60U;
	now /= 60U;
	min = now % 60U;
	now /= 60U;
	h = now;

	snprintf(buf, sizeof(buf), "%u:%02u:%02u.%03u",
		 h, min, s, ms);
	return buf;
}

#if ATMEL_AT30TS75A
uint8_t set_bits(uint8_t *byte, uint8_t bit_num,
                 uint8_t bit_mask_val, uint8_t bit_val)
{
	uint8_t bit_mask = (bit_mask_val << bit_num);

	*byte = (*byte & (~bit_mask)) | (bit_val << 5);
}

static int sensor_set_configuration(const struct device *dev,
				enum sensor_channel chan,
				enum sensor_attribute attr, int value)
{
	int ret;
	struct sensor_value config_val = {
		.val1 = 0,
		.val2 = 0
	};

	uint8_t res_bit_set;
	switch (value) {
        case (9):
            res_bit_set = 0;
            break;
        case (10):
            res_bit_set = 1;
            break;
        case (11):
            res_bit_set = 2;
            break;
        case (12):
            res_bit_set = 3;
            break;
	};

	set_bits(&config_val.val1, ATMEL_AT30TS75A_CONFIG_RESOLUTION_BITS, 
             res_bit_set, 3);
	config_val.val2 = 0x0;  /* LSB of ATMEL AT30TS75A config register are reserved for future use */

    /* Additional configurations can be added here */

	ret = sensor_attr_set(dev, chan, attr, &config_val);
	if (ret) {
		printf("sensor_attr_set failed ret %d\n", ret);
	}

	return ret;
}
#endif

static void process(const struct device *dev)
{
	struct sensor_value temp_val;
	int ret;


	/* Set resolution to 12 bit */
	#if ATMEL_AT30TS75A
	sensor_set_configuration(dev, SENSOR_CHAN_AMBIENT_TEMP, 
                             SENSOR_ATTR_CONFIGURATION, 12);
	#endif

	while (1) {
		ret = sensor_sample_fetch(dev);
		if (ret) {
			printf("sensor_sample_fetch failed ret %d\n", ret);
			return;
		}

		ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP,
					 &temp_val);
		if (ret) {
			printf("sensor_channel_get failed ret %d\n", ret);
			return;
		}

		printf("[%s]: temperature %.6f Cel\n", now_str(),
		       sensor_value_to_double(&temp_val));

		k_sleep(DELAY_WITHOUT_TRIGGER);
	}
}

void main(void)
{
	const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, lm75)));

	if (dev == NULL) {
		printf("Failed to get device binding\n");
		return;
	}

	printf("device is %p, name is %s\n", dev, dev->name);

	process(dev);
}
