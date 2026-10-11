#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int led_toggled(const struct device *dev, uint32_t times_toggled);

#ifdef __cplusplus
}
#endif
