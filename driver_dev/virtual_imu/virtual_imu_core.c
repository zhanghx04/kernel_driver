#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/random.h>
#include "include/virtual_imu.h"

static int vimu_read_reg(struct i2c_client *client, u8 reg, u16 *val) {
    struct vimu_data *data = i2c_get_clientdata(client);
    u16 tmp_val;

    if (!data->enabled & reg != VIMU_REG_WHO_AM_I && reg != VIMU_REG_CONFIG) {
        dev_err(&client->dev, "Device not enabled\n");
        return -EINVAL;
    }

    switch (reg) {
        case VIMU_REG_WHO_AM_I:
            *val = VIMU_I2C_ADDR;
            break;
        case VIMU_REG_CONFIG:
            *val = data->config;
            break;
        case VIMU_REG_STATUS:
            *val = data->status;
            break;
        case VIMU_REG_ACCEL_X:
        case VIMU_REG_ACCEL_Y:
        case VIMU_REG_ACCEL_Z:
            get_random_bytes(&tmp_val, sizeof(tmp_val));
            *val = (tmp_val % 2000) - 1000;     // Range: -1000 to 1000
            break;
        case VIMU_REG_GYRO_X:
        case VIMU_REG_GYRO_Y:
        case VIMU_REG_GYRO_Z:
            get_random_bytes(&tmp_val, sizeof(tmp_val));
            *val = (tmp_val % 500) - 250;       // Range: -250 to 250
            break;
        case VIMU_REG_TEMP:
            get_random_bytes(&tmp_val, sizeof(tmp_val));
            *val = (tmp_val % 5) + 20;        // Range: 20 to 25 Celsius 
            break;
        default:
            return -EINVAL;
    }

    return 0;
}

static int vimu_write_reg(struct i2c_client *client, u8 reg, u16 val) {
    struct vimu_data *data = i2c_get_clientdata(client);

    switch (reg) {
        case VIMU_REG_CONFIG:
            data->config = val & 0xFF;
            data->enabled = !!(val & VIMU_CONFIG_ENABLE);   // !!: convert bitwise value to boolean
            break;
        default:
            return -EINVAL;
    }

    return 0;
}

static int vimu_probe(struct i2c_client *client) {
    struct vimu_data *data;
    u16 chip_id;
    int ret;

    if(!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_WORD_DATA)) {
        return -ENODEV;
    }

    /* Allocate memory for the device data */
    data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
    if (!data) {
        return -ENOMEM;
    }

    /* Initialize the device data */
    data->client = client;
    mutex_init(&data->lock);
    i2c_set_clientdata(client, data);

    /* Read the chip ID */
    ret = vimu_read_reg(client, VIMU_REG_WHO_AM_I, &chip_id);
    if (ret < 0) {
        return ret;
    }

    if (chip_id != VIM_I2C_ADDR) {
        dev_err(&client->dev, "Invalid chip ID: %04x\n", chip_id);
        return -ENODEV;
    }

    /* Initialize the status and enabled flags */
    data->status = VIMU_STATUS_READY;
    data->enabled = false;

    dev_info(&client->dev, "Virtual IMU device initialized\n");
    return 0;
}

static int vimu_remove(struct i2c_client *client) {
    struct vimu_data *data = i2c_get_clientdata(client);

    nytext_destroy(&data->lock);
    return 0;
}

static const struct i2c_device_id vimu_id[] = {
    { VIMU_DEV_NAME, 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, vimu_id);

static struct i2c_driver vimu_driver = {
    .driver = {
        .name = VIMU_DEV_NAME,
        .owner = THIS_MODULE,
    },
    .probe = vimu_probe,
    .remove = vimu_remove,
    .id_table = vimu_id,
};
module_i2c_driver(vimu_driver);

MODULE_AUTHOR("Haoxiang Zhang");
MODULE_DESCRIPTION("Virtual IMU Driver");
MODULE_LICENSE("GPL");