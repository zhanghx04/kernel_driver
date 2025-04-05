#ifndef _VIRTUAL_IMU_H
#define _VIRTUAL_IMU_H

#include <linux/i2c.h>
#include <linux/mutex>
#include <linux/delay.h>
#include <linux/sysfs.h>

/* Device Constants */
#define VIMU_DEV_NAME "virtual_imu"
#define VIMU_I2C_ADDR       0x68

/* Register Map */
#define VIMU_REG_WHO_AM_I   0x00
#define VIMU_REG_CONFIG     0x01
#define VIMU_REG_STATUS     0x02
#define VIMU_REG_ACCEL_X    0x10
#define VIMU_REG_ACCEL_Y    0x12
#define VIMU_REG_ACCEL_Z    0x14
#define VIMU_REG_GYRO_X     0x20
#define VIMU_REG_GYRO_Y     0x22
#define VIMU_REG_GYRO_Z     0x24
#define VIMU_REG_TEMP       0x30

/* Status Register Bits */
#define VIMU_STATUS_READY   BIT(0)
#define VIMU_STATUS_SLEEP   BIT(1)

/* Configuration Register Bits */
#define VIMU_CONFIG_ENABLE  BIT(0)
#define VIMU_CONFIG_SLEEP   BIT(1)

struct vimu_data {
    struct i2c_client *client;
    struct mutex lock;
    bool enabled;
    s16 accel_data[3];
    s16 gyro_data[3];
    s16 temp_data;
    u8 config;
    u8 status;
};

/* Function Prototypes */
int vimu_read_reg(struct i2c_client *client, u8 reg, u16 *val);
int vimu_write_reg(struct i2c_client *client, u8 reg, u16 val);


#endif // _VIRTUAL_IMU_H