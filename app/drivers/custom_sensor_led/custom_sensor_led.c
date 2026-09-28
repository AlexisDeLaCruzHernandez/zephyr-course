#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT   custom_sensor_led

// Config struct
struct custom_sensor_led_config {
    struct gpio_dt_spec led;
};

// Turns on the LED
static int sensor_led_sample_fetch(const struct device *dev, enum sensor_channel chan) 
{
    const struct custom_sensor_led_config *cfg = dev->config;

    return gpio_pin_set_dt(&cfg->led, 1);
}

// Turns off the LED
static int sensor_led_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    const struct custom_sensor_led_config *cfg = dev->config;

    return gpio_pin_set_dt(&cfg->led, 0);
}

// Initialize the sensor
static int sensor_led_init(const struct device *dev) 
{
    const struct custom_sensor_led_config *cfg = dev->config;

    if(!gpio_is_ready_dt(&cfg->led)) {
        return -ENODEV;
    }

    return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

static DEVICE_API(sensor, custom_sensor_led_api) = {
    .sample_fetch = sensor_led_sample_fetch,
    .channel_get = sensor_led_channel_get,
};

#define CUSTOM_SENSOR_LED_DEFINE(inst)                          \
    static const struct custom_sensor_led_config cfg_##inst = { \
        .led = GPIO_DT_SPEC_INST_GET(inst, gpios),              \
    };                                                          \
    DEVICE_DT_INST_DEFINE(                                      \
        inst,                                                   \
        sensor_led_init,                                        \
        NULL,                                                   \
        NULL,                                                   \
        &cfg_##inst,                                            \
        POST_KERNEL,                                            \
        CONFIG_SENSOR_INIT_PRIORITY,                            \
        &custom_sensor_led_api                                  \
    )                                                           \

DT_INST_FOREACH_STATUS_OKAY(CUSTOM_SENSOR_LED_DEFINE)
