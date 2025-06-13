#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/delay.h>

#define DEVICE_NAME "ads1115"
#define CLASS_NAME "ads1115_class"
#define MOISTURE_SENSOR_MINOR 1
#define WATER_SENSOR_MINOR 0
#define NUM_DEVICES 2

static int major;
static struct class* ads1115_class = NULL;
static struct cdev ads1115_cdev;
static struct device* devices[NUM_DEVICES];
static struct i2c_client* ads1115_client = NULL;

// Função que configura e lê um canal do ADS1115
static int ads1115_read_channel(int channel)
{
    u8 config[3];
    u16 raw;
    s16 value;
    struct i2c_msg msgs[2];

    if (channel < 0 || channel > 3)
        return -EINVAL;

    // CONFIG = [MSB, LSB]
    // Canal de entrada: AINp = channel, AINn = GND (modo single-ended)
    u16 config_value = 0x4000 |  // Modo single-shot
                       0x8000 |  // Inicia conversão
                       0x0003 |  // Taxa de amostragem = 128 SPS
                       0x0100;   // Modo single-ended AINx

    config_value |= (channel & 0x03) << 12; // Seleciona canal

    config[0] = 0x01; // Registrador de configuração
    config[1] = config_value >> 8;
    config[2] = config_value & 0xFF;

    msgs[0].addr = ads1115_client->addr;
    msgs[0].flags = 0;
    msgs[0].len = 3;
    msgs[0].buf = config;

    if (i2c_transfer(ads1115_client->adapter, &msgs[0], 1) < 0)
        return -EIO;

    msleep(10); // Aguarda a conversão (~8ms em 128SPS)

    config[0] = 0x00; // Registrador de leitura

    msgs[0].addr = ads1115_client->addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = config;

    msgs[1].addr = ads1115_client->addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = 2;
    msgs[1].buf = config + 1;

    if (i2c_transfer(ads1115_client->adapter, msgs, 2) < 0)
        return -EIO;

    raw = (config[1] << 8) | config[2];
    value = (s16)raw;

    return value;
}

// Função de leitura para os dois dispositivos
static ssize_t ads1115_read(struct file* file, char __user* buf, size_t count, loff_t* offset)
{
    int minor = iminor(file_inode(file));
    int value, len;
    char buffer[16];

    if (*offset > 0)
        return 0;

    switch (minor) {
        case MOISTURE_SENSOR_MINOR:
            value = ads1115_read_channel(0); // Canal 0
            break;
        case WATER_SENSOR_MINOR:
            value = ads1115_read_channel(1); // Canal 1
            break;
        default:
            return -ENODEV;
    }

    if (value < 0)
        return value;

    len = snprintf(buffer, sizeof(buffer), "%d\n", value);
    if (copy_to_user(buf, buffer, len))
        return -EFAULT;

    *offset += len;
    return len;
}

// Estrutura file_operations
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = ads1115_read,
};

// Probe: chamado quando o driver é carregado
static int ads1115_probe(struct i2c_client* client)
{
    if (!client)
        return -ENODEV;

    if (strcmp(client->name, "ads1115") != 0)
        return -ENODEV;

    dev_t dev_num;
    int err;

    ads1115_client = client;

    err = alloc_chrdev_region(&dev_num, 0, NUM_DEVICES, DEVICE_NAME);
    if (err < 0)
        return err;

    major = MAJOR(dev_num);

    cdev_init(&ads1115_cdev, &fops);
    err = cdev_add(&ads1115_cdev, dev_num, NUM_DEVICES);
    if (err)
        goto unregister;

    ads1115_class = class_create(CLASS_NAME);
    if (IS_ERR(ads1115_class)) {
        err = PTR_ERR(ads1115_class);
        goto del_cdev;
    }

    devices[MOISTURE_SENSOR_MINOR] = device_create(ads1115_class, NULL, MKDEV(major, MOISTURE_SENSOR_MINOR), NULL, "moisture_sensor");
    devices[WATER_SENSOR_MINOR] = device_create(ads1115_class, NULL, MKDEV(major, WATER_SENSOR_MINOR), NULL, "water_level_sensor");

    if (IS_ERR(devices[MOISTURE_SENSOR_MINOR]) || IS_ERR(devices[WATER_SENSOR_MINOR])) {
        err = -EINVAL;
        goto destroy_class;
    }

    dev_info(&client->dev, "ads1115: Driver initialized, devices created\n");
    return 0;

destroy_class:
    device_destroy(ads1115_class, MKDEV(major, MOISTURE_SENSOR_MINOR));
    device_destroy(ads1115_class, MKDEV(major, WATER_SENSOR_MINOR));
    class_destroy(ads1115_class);
del_cdev:
    cdev_del(&ads1115_cdev);
unregister:
    unregister_chrdev_region(MKDEV(major, 0), NUM_DEVICES);
    return err;
}

// Remove: chamado ao remover o módulo
static void ads1115_remove(struct i2c_client* client)
{
    device_destroy(ads1115_class, MKDEV(major, MOISTURE_SENSOR_MINOR));
    device_destroy(ads1115_class, MKDEV(major, WATER_SENSOR_MINOR));
    class_destroy(ads1115_class);
    cdev_del(&ads1115_cdev);
    unregister_chrdev_region(MKDEV(major, 0), NUM_DEVICES);
    dev_info(&client->dev, "ads1115: Driver removed\n");
}

static const struct i2c_device_id ads1115_id[] = {
    { "ads1115", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, ads1115_id);

static struct i2c_driver ads1115_driver = {
    .driver = {
        .name = "ads1115_driver",
    },
    .probe = ads1115_probe,  // <- Use .probe_new ao invés de .probe
    .remove = ads1115_remove,
    .id_table = ads1115_id,
};

module_i2c_driver(ads1115_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("artur.pires");
MODULE_DESCRIPTION("Driver para ler dispositivos no ADS1115");
