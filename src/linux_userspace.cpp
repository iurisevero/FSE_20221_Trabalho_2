// Adapted by Iuri Severo

/**\
 * Copyright (c) 2020 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

/**
 * \ingroup bme280
 * \defgroup bme280Examples Examples
 * @brief Reference Examples
 */

/*!
 * @ingroup bme280Examples
 * @defgroup bme280GroupExampleLU linux_userspace
 * @brief Linux userspace test code, simple and mose code directly from the doco.
 * compile like this: gcc linux_userspace.c ../bme280.c -I ../ -o bme280
 * tested: Raspberry Pi.
 * Use like: ./bme280 /dev/i2c-0
 * \include linux_userspace.c
 */

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/******************************************************************************/
/*!                         System header files                               */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/******************************************************************************/
/*!                         Own header files                                  */
#include "linux_userspace.hpp"
#include "bme280.hpp"
#include "globalValues.hpp"
#include "helpers.hpp"

/* Structure to get the pressure, temperature and humidity values */
struct bme280_data comp_data;

void runBME280Sensors(int _idFd)
{
    struct bme280_dev dev;

    struct identifier id;

    /* Variable to define the result */
    int8_t rslt = BME280_OK;

    id.fd = _idFd;

    /* Make sure to select BME280_I2C_ADDR_PRIM or BME280_I2C_ADDR_SEC as needed */
    id.dev_addr = BME280_I2C_ADDR_PRIM;

    if (ioctl(id.fd, I2C_SLAVE, id.dev_addr) < 0)
    {
        fprintf(stderr, "Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }

    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    /* Update interface pointer with the structure that contains both device address and file descriptor */
    dev.intf_ptr = &id;

    /* Initialize the bme280 */
    rslt = bme280_init(&dev);
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
        exit(1);
    }

    rslt = stream_sensor_data_forced_mode(&dev);
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to stream sensor data (code %+d).\n", rslt);
        exit(1);
    }
    printf("Sensors finished\n");
    return;
}

/*!
 * @brief This function reading the sensor's registers through I2C bus.
 */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    write(id.fd, &reg_addr, 1);
    read(id.fd, data, len);

    return 0;
}

/*!
 * @brief This function provides the delay for required time (Microseconds) as per the input provided in some of the
 * APIs
 */
void user_delay_us(uint32_t period, void *intf_ptr)
{
    usleep(period);
}

/*!
 * @brief This function for writing the sensor's registers through I2C bus.
 */
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t *buf;
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    buf = (uint8_t*) malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);
    if (write(id.fd, buf, len + 1) < (uint16_t)len)
    {
        return BME280_E_COMM_FAIL;
    }

    free(buf);

    return BME280_OK;
}

/*!
 * @brief This API used to print the sensor temperature, pressure and humidity data.
 */
void print_sensor_data(struct bme280_data *comp_data)
{
    float temp, press, hum;

#ifdef BME280_FLOAT_ENABLE
    temp = comp_data->temperature;
    press = 0.01 * comp_data->pressure;
    hum = comp_data->humidity;
#else
#ifdef BME280_64BIT_ENABLE
    temp = 0.01f * comp_data->temperature;
    press = 0.0001f * comp_data->pressure;
    hum = 1.0f / 1024.0f * comp_data->humidity;
#else
    temp = 0.01f * comp_data->temperature;
    press = 0.01f * comp_data->pressure;
    hum = 1.0f / 1024.0f * comp_data->humidity;
#endif
#endif
    printf("%0.2lf deg C, %0.2lf hPa, %0.2lf%%\n", temp, press, hum);
}

/*!
 * @brief This API reads the sensor temperature, pressure and humidity data in forced mode.
 */
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev)
{
    /* Variable to define the result */
    int8_t rslt = BME280_OK;

    /* Variable to define the selecting sensors */
    uint8_t settings_sel = 0;

    /* Variable to store minimum wait time between consecutive measurement in force mode */
    uint32_t req_delay;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    /* Set the sensor settings */
    rslt = bme280_set_sensor_settings(settings_sel, dev);
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to set sensor settings (code %+d).", rslt);

        return rslt;
    }

    // printf("Temperature, Pressure, Humidity\n");

    /*Calculate the minimum delay required between consecutive measurement based upon the sensor enabled
     *  and the oversampling configuration. */
    req_delay = bme280_cal_meas_delay(&dev->settings);

    /* Continuously stream sensor data */
    while (run)
    {
        /* Set the sensor to forced mode */
        rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
        if (rslt != BME280_OK)
        {
            fprintf(stderr, "Failed to set sensor mode (code %+d).", rslt);
            break;
        }

        /* Wait for the measurement to complete and print data */
        dev->delay_us(req_delay, dev->intf_ptr);
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        if (rslt != BME280_OK)
        {
            fprintf(stderr, "Failed to get sensor data (code %+d).", rslt);
            break;
        }
        roomTemp = comp_data.temperature;
        // print_sensor_data(&comp_data);
        sleepMs(900);
    }

    return rslt;
}

struct bme280_data getSensorData(){
    return comp_data;
}