#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/delay.h>

#define DEVICE_NAME "ads1115"
#define CLASS_NAME  "ads"

static int majorNumber;
static struct class* ads1115_class = NULL;
static struct device* ads1115_device = NULL;
static struct i2c_client *ads1115_client = NULL;

static int ads1115_open(struct inode *inodep, struct file *filep) {
    return 0;
}

static int ads1115_release(struct inode *inodep, struct file *filep) {
    return 0;
}

static ssize_t ads1115_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int adc_value;
    u8 config[2];
    u8 data[2];

    // Configura o ADS1115 para uma leitura simples no canal 0
    config[0] = 0x01; // Ponteiro para o registrador de configuração
    config[1] = 0xC3; // Configuração MSB (canal 0, modo single-shot, PGA ±4.096V)
    config[2] = 0x83; // Configuração LSB (data rate e modo)

    i2c_master_send(ads1115_client, config, 3);

    // Espera conversão (~8ms)
    msleep(10);

    // Lê o resultado do conversor
    config[0] = 0x00;
    i2c_master_send(ads1115_client, config, 1);
    i2c_master_recv(ads1115_client, data, 2);

    adc_value = (data[0] << 8) | data[1];

    return copy_to_user(buffer, &adc_value, sizeof(adc_value)) ? -EFAULT : sizeof(adc_value);
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = ads1115_open,
    .read = ads1115_read,
    .release = ads1115_release,
};

// Novo formato da função probe
static int ads1115_probe(struct i2c_client *client) {
    ads1115_client = client;

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        pr_err("ADS1115: falha ao registrar o major number\n");
        return majorNumber;
    }

    ads1115_class = class_create(CLASS_NAME);
    if (IS_ERR(ads1115_class)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        pr_err("ADS1115: falha ao criar classe\n");
        return PTR_ERR(ads1115_class);
    }

    ads1115_device = device_create(ads1115_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(ads1115_device)) {
        class_destroy(ads1115_class);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        pr_err("ADS1115: falha ao criar device\n");
        return PTR_ERR(ads1115_device);
    }

    pr_info("ADS1115: driver inicializado com sucesso\n");
    return 0;
}

static void ads1115_remove(struct i2c_client *client) {
    device_destroy(ads1115_class, MKDEV(majorNumber, 0));
    class_unregister(ads1115_class);
    class_destroy(ads1115_class);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    pr_info("ADS1115: driver removido\n");
}

static const struct of_device_id ads1115_dt_ids[] = {
    { .compatible = "ti,ads1115" },
    { }
};
MODULE_DEVICE_TABLE(of, ads1115_dt_ids);

static const struct i2c_device_id ads1115_id[] = {
    { "ads1115", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, ads1115_id);

static const struct of_device_id ads1115_of_match[] = {
    { .compatible = "ti,ads1115" },
    { }
};
MODULE_DEVICE_TABLE(of, ads1115_of_match);

static struct i2c_driver ads1115_driver = {
    .driver = {
        .name = "ads1115",
        .of_match_table = of_match_ptr(ads1115_of_match),  // se estiver usando device tree
    },
    .probe = ads1115_probe,
    .remove = ads1115_remove,
    .id_table = ads1115_id,  // essa é a tabela de device_id
};

module_i2c_driver(ads1115_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seu Nome");
MODULE_DESCRIPTION("Driver I2C do ADS1115 com suporte a /dev/ads1115");
