/*
 * Copyright (c) 2020 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include "adc.h"
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/conn.h>
#include "my_lbs.h"

#define USER_BUTTON DK_BTN1_MSK
#define CON_STATUS_LED DK_LED2
#define USER_LED DK_LED3
static bool app_button_state;
static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(
    (BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY),
    800,
    801,
    NULL);

LOG_MODULE_REGISTER(MAIN, LOG_LEVEL_INF);

#define STACKSIZE 1024
#define PRIORITY 7

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};


static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_LBS_VAL),
};

static void button_changed(uint32_t button_state, uint32_t has_changed)
{
	if (has_changed & USER_BUTTON) {
		uint32_t user_button_state = button_state & USER_BUTTON;
		/* STEP 6 - Send indication on a button press */
		my_lbs_send_button_state_indicate(user_button_state);
		app_button_state = user_button_state ? true : false;
	}
}

static int init_button(void)
{
	int err;

	err = dk_buttons_init(button_changed);
	if (err) {
		printk("Cannot init buttons (err: %d)\n", err);
	}

	return err;
}

static void on_connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err %u)\n", err);
		return;
	}

	printk("Connected\n");

	dk_set_led_on(CON_STATUS_LED);
}

static void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);

	dk_set_led_off(CON_STATUS_LED);
}

struct bt_conn_cb connection_callbacks = {
	.connected = on_connected,
	.disconnected = on_disconnected,
};

static void simulate_data(struct Measurement *measurement)
{
    *measurement = readADCValue();
}
static void app_led_cb(bool led_state)
{
	dk_set_led(USER_LED, led_state);
}

static bool app_button_cb(void)
{
	return app_button_state;
}

void send_data_thread(void)
{
    while (1)
    {
        struct Measurement m = readADCValue();
        uint32_t my = m.x + m.y + m.z;
        my_lbs_send_sensor_notify(m.x, m.y,m.z);
        
        printk("x = %d,  y = %d,  z = %d\n", m.x, m.y, m.z);
        k_sleep(K_SECONDS(3)); 
    }
}

static struct my_lbs_cb app_callbacks = {
	.led_cb = app_led_cb,
	.button_cb = app_button_cb,
};


void main(void)
{
    int blink_status = 0;
    int err;

    LOG_INF("Starting Main\n");

	if(initializeADC() != 0)
	{
	printk("ADC initialization failed!");
	return;
	}

    err = init_button();
	if (err) {
		printk("Button init failed (err %d)\n", err);
		return;
	}
    
    err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
		return;
	}
	bt_conn_cb_register(&connection_callbacks);

	err = my_lbs_init(&app_callbacks);
	if (err) {
		printk("Failed to init LBS (err:%d)\n", err);
		return;
	}
	LOG_INF("Bluetooth initialized\n");
    err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err)
    {
        LOG_ERR("Advertising failed to start (err %d)\n", err);
        return;
    }

    LOG_INF("Advertising successfully started\n");
	struct Measurement m = readADCValue();
	printk("x = %d,  y = %d,  z = %d\n", m.x, m.y, m.z);

}
K_THREAD_DEFINE(send_data_thread_id, STACKSIZE, send_data_thread, NULL, NULL, NULL, PRIORITY, 0, 0);

