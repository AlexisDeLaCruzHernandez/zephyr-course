#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(board_init, LOG_LEVEL_INF);

static int scratch_f411ce_init(void) {
    LOG_INF("Board Initialized");
    return 0;
}

SYS_INIT(scratch_f411ce_init, APPLICATION, 0);
