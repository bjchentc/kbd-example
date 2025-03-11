
#include <stdio.h>
#include <soc.h>
#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#ifdef CONFIG_KSCAN
#include <zephyr/drivers/kscan.h>

#ifdef CONFIG_ITE_KBD_HOOK_EVENT_THREAD
K_EVENT_DEFINE(ec_event);
#define USER_STACKSIZE_KSCAN    1024
K_THREAD_STACK_DEFINE(kbmatrix_stack, USER_STACKSIZE_KSCAN);

struct k_thread kbmatrix_thread;
void keyboard_matrix_thread(void *p1, void *p2, void *p3)
{
    while (1) {
        k_event_wait(&ec_event, BIT(0), false, K_FOREVER);

        #ifdef CONFIG_ITE_EC_KBD_DBG
        LOG_INF("Key Scan Service Triggered by kbd callback event.");
        #endif /* #ifdef CONFIG_ITE_EC_KBD_DBG */

        k_event_clear(&ec_event, BIT(0));

        /* Emulate process time */
        k_sleep(K_MSEC(5));
    }
}
#endif /* #ifdef CONFIG_ITE_KBD_HOOK_EVENT_THREAD */

const struct device *const kscan_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_keyboard_scan));

static void kb_callback(const struct device *dev, uint32_t row, uint32_t col,
			bool pressed)
{
	ARG_UNUSED(dev);
    #ifdef CONFIG_ITE_EC_KBD_DBG
	LOG_INF("Key [%u,%u] Pressed = %u\n", row,col, pressed);
    #endif /* #ifdef CONFIG_ITE_EC_KBD_DBG */

    #ifdef CONFIG_ITE_KBD_HOOK_EVENT_THREAD
    k_event_post(&ec_event, BIT(0));
    #endif /* #ifdef CONFIG_ITE_KBD_HOOK_EVENT_THREAD */
}

int kbd_init(const struct device *dev)
{
	if (!device_is_ready(kscan_dev)) {
        #ifdef CONFIG_ITE_EC_KBD_DBG
		LOG_ERR("kscan device %s not ready", kscan_dev->name);
        #endif /* #ifdef CONFIG_ITE_EC_KBD_DBG */
		return -1;
	}

	kscan_config(kscan_dev, kb_callback);

    return 0;
}
#endif /* CONFIG_KSCAN */

int main(void)
{
    #ifdef CONFIG_ITE_KBD_HOOK_EVENT_THREAD
	k_thread_create(
        &kbmatrix_thread,
        kbmatrix_stack,
        K_THREAD_STACK_SIZEOF(kbmatrix_stack),
	    keyboard_matrix_thread,
        NULL,
        NULL,
        NULL,
        0,
        K_USER,
        K_NO_WAIT
    );
    #endif /* CONFIG_ITE_KBD_HOOK_EVENT_THREAD */

#ifdef CONFIG_KSCAN
    LOG_INF("ITE Keyboard Example Enabled.\r\n");
    kbd_init(kscan_dev);
#endif /* #ifdef CONFIG_KSCAN */

	return 0;
}
