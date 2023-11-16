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

static void simulate_data(struct Measurement *measurement)
{
    *measurement = readADCValue();
}

void send_data_thread(void)
{
    while (1)
    {
        struct Measurement m = readADCValue();
        my_lbs_send_sensor_notify(&m);
        printk("x = %d,  y = %d,  z = %d\n", m.x, m.y, m.z);
        k_sleep(K_SECONDS(3)); 
    }
}

void main(void)
{
    int blink_status = 0;
    int err;

    LOG_INF("Starting Main\n");

    err = bt_enable(NULL);
    if (err)
    {
        LOG_ERR("Bluetooth init failed (err %d)\n", err);
        return;
    }

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

