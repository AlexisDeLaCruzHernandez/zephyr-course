#include "zephyr/shell/shell.h"
#include "zephyr/drivers/sensor.h"
#include "custom_sensor_led.h"

#define LED_SENSOR_NODE DT_ALIAS(sensor_led)

static const struct device *custom_sensor_led = DEVICE_DT_GET(DT_ALIAS(sensor_led));

static int cmd_fetch(const struct shell *sh, size_t argc, char **argv)
{
    if(!device_is_ready(custom_sensor_led)) {
        shell_error(sh, "Device is not ready");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(custom_sensor_led);
    if(ret != 0) {
        shell_error(sh, "sensor_sample_fetch() error");
        return ret;
    }

    return 0;
}

static int cmd_read(const struct shell *sh, size_t argc, char **argv)
{
    struct sensor_value val;

    if(!device_is_ready(custom_sensor_led)) {
        shell_error(sh, "Device is not ready");
        return -ENODEV;
    }

    int ret = sensor_channel_get(custom_sensor_led, SENSOR_CHAN_ALL, &val);
    if(ret != 0) {
        shell_error(sh, "sensor_channel_get() error");
        return ret;
    }

    shell_info(sh, "Value: %d", val.val1);
    return 0;
}

static int cmd_info(const struct shell *sh, size_t argc, char **argv)
{
    if(!device_is_ready(custom_sensor_led)) {
        shell_error(sh, "Device is not ready");
        return -ENODEV;
    }

    shell_info(sh, "Device name: %s", custom_sensor_led->name);
    shell_info(sh, "Device ready: yes");
    return 0;
}

static int cmd_set(const struct shell *sh, size_t argc, char **argv)
{
    if(!device_is_ready(custom_sensor_led)) {
        shell_error(sh, "Device is not ready");
        return -ENODEV;
    }

    char *endptr;
    uint32_t value = strtoul(argv[1], &endptr, 0);

    if (*endptr != '\0') {
        shell_error(sh, "Invalid format. Argument must be a number");
        return -EINVAL;
    }

    value = led_toggled(custom_sensor_led, value);

    shell_info(sh, "Times toggled: %d", value);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_commands_driver,
    SHELL_CMD_ARG(fetch, NULL, "Calls sensor_sample_fetch()", cmd_fetch, 1, 0),
    SHELL_CMD_ARG(read, NULL, "Calls sensor_channel_get() and prints the result", cmd_read, 1, 0),
    SHELL_CMD_ARG(info, NULL, "Prints the device name and ready state", cmd_info, 1, 0),
    SHELL_CMD_ARG(set, NULL, "Sets the number of times toggled", cmd_set, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_commands_driver, "Sensor commands", NULL);