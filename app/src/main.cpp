// #include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* The devicetree node identifier for the "app-led" alias. */
// #define LED_NODE DT_ALIAS(app_led)
#define LED_SENSOR_NODE DT_ALIAS(sensor_led)

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct device *custom_sensor_led = DEVICE_DT_GET(LED_SENSOR_NODE);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = false;

    // if(!gpio_is_ready_dt(&led)) return 0;
    if(!device_is_ready(custom_sensor_led)) return 0;

    // if(gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while(1) {
        // if(gpio_pin_toggle_dt(&led) < 0) return 0;
        if(led_state == false) {
            sensor_sample_fetch(custom_sensor_led);
            led_state = true;
        }
        else {
            sensor_channel_get(custom_sensor_led, SENSOR_CHAN_ALL, NULL);
            led_state = false;
        }

        // led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
