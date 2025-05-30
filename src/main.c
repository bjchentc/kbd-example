
#include <stdio.h>
#include <soc.h>
#include <zephyr/kernel.h>
#include <zephyr/input/input.h>
#include <zephyr/input/input_kbd_matrix.h>
#include "main.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#ifdef CONFIG_ITE_KBD_HOOK_EVENT_THREAD
K_EVENT_DEFINE(ec_event);
#define USER_STACKSIZE_KBD    1024
K_THREAD_STACK_DEFINE(kbd_stack, USER_STACKSIZE_KBD);

struct k_thread kbd_thread;
void kbd_service_thread(void *p1, void *p2, void *p3)
{
    while (1) {
        k_event_wait(&ec_event, BIT(0), false, K_FOREVER);

        #ifdef CONFIG_ITE_EC_KBD_DBG
        LOG_INF("Keyboard Service Triggered by kbd input callback event.");
        #endif /* #ifdef CONFIG_ITE_EC_KBD_DBG */

        k_event_clear(&ec_event, BIT(0));

        /* Emulate process time */
        k_sleep(K_MSEC(5));
    }
}
#endif /* #ifdef CONFIG_ITE_KBD_HOOK_EVENT_THREAD */


#define EC_KEYBOARD_NODE DT_CHOSEN(ec_keyboard)
static const struct device *const kbd_dev = DEVICE_DT_GET(EC_KEYBOARD_NODE);

static void keyboard_input_cb(struct input_event *evt, void *user_data)
{
    static int row;
    static int col;
    static bool pressed;

    switch (evt->code) {
    case INPUT_ABS_X:
        col = evt->value;
        break;
    case INPUT_ABS_Y:
        row = evt->value;
        break;
    case INPUT_BTN_TOUCH:
        pressed = evt->value;
        break;
    }

    if (evt->sync) {
        #ifdef CONFIG_ITE_EC_KBD_DBG
        LOG_INF("Keyboard Event Triggered: [x, y, pressed] = [%d, %d, %d]\r\n", col, row, pressed);
        #endif /* #ifdef CONFIG_ITE_EC_KBD_DBG */

        #ifdef CONFIG_ITE_KBD_HOOK_EVENT_THREAD
        k_event_post(&ec_event, BIT(0));
        #endif /* #ifdef CONFIG_ITE_KBD_HOOK_EVENT_THREAD */
    }
}
INPUT_CALLBACK_DEFINE(kbd_dev, keyboard_input_cb,NULL);

int kbd_init(void)
{
    if (!device_is_ready(kbd_dev)) {

        #ifdef CONFIG_ITE_EC_KBD_DBG
		LOG_ERR("kbd device %s not ready", kbd_dev->name);
        #endif /* #ifdef CONFIG_ITE_EC_KBD_DBG */

        return -1;
    }

    input_kbd_matrix_pm_action(kbd_dev, 1);

    LOG_INF("Keyboard Initialization Complete\r\n");

    return 0;
}


int main(void)
{
    #ifdef CONFIG_ITE_KBD_HOOK_EVENT_THREAD
	k_thread_create(
        &kbd_thread,
        kbd_stack,
        K_THREAD_STACK_SIZEOF(kbd_stack),
	    kbd_service_thread,
        NULL,
        NULL,
        NULL,
        0,
        K_USER,
        K_NO_WAIT
    );
    #endif /* CONFIG_ITE_KBD_HOOK_EVENT_THREAD */

    LOG_INF("ITE Keyboard Example Enabled.\r\n");
    kbd_init();

	return 0;
}
