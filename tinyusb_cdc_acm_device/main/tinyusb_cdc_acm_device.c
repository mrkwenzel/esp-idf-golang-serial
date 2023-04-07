/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdio.h>
#include <stdint.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include "tusb_console.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "bootloader_random.h"
#include "esp_random.h"


#define STATUS_GPIO GPIO_NUM_15

#define LED_STATE_ON 1
#define LED_STATE_OFF 0

#define BITS_256 256
#define BITS_128 128

static const char *TAG = "esprandnum";
static const char *prefix = "SAS";
static const char *suffix = "KIA";
static const char *nl = "\n";
static uint8_t buf[CONFIG_TINYUSB_CDC_RX_BUFSIZE + 1];

char RESP_CMD[]                 = "GENJOKE";
char RESP_CMD2[]                = "GENRAND";

static void configure_led(void)
{
    gpio_reset_pin(STATUS_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(STATUS_GPIO, GPIO_MODE_OUTPUT);
}

static void set_led(uint8_t led_state)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(STATUS_GPIO, led_state);
}

void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event)
{
    /* initialization */
    size_t rx_size = 0;

    /* read */
    esp_err_t ret = tinyusb_cdcacm_read(itf, buf, CONFIG_TINYUSB_CDC_RX_BUFSIZE, &rx_size);

    if (ret != ESP_OK){
        // error handling here...
    }

    if (strcmp((const char *)buf, RESP_CMD) == 0)
    {
        ESP_LOGI(TAG, "log -> USB");
        fprintf(stdout, "example: print -> stdout\n");
        unsigned char conbuf[32] = {'{','"','j','o','k','e','"',':','"','t','e','s','t','"', '}', '\n'};

        /* write back */
        tinyusb_cdcacm_write_queue(itf, conbuf, 16);
        tinyusb_cdcacm_write_flush(itf, 0);
    } else if (strcmp((const char *)buf, RESP_CMD2) == 0) {
        set_led(LED_STATE_ON);
        uint8_t randval[BITS_256/8];
        esp_fill_random(randval, sizeof(randval));

        tinyusb_cdcacm_write_queue(itf, (const unsigned char*)prefix, 3);
        tinyusb_cdcacm_write_queue(itf, randval, BITS_256/8);
        tinyusb_cdcacm_write_queue(itf, (const unsigned char*)suffix, 3);
        tinyusb_cdcacm_write_queue(itf, (const unsigned char*)nl, 1);
        tinyusb_cdcacm_write_flush(itf, 0);
    }
}

void app_main(void)
{
    bootloader_random_enable();

    configure_led();

    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = NULL,
        .external_phy = false,
        .configuration_descriptor = NULL,
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    tinyusb_config_cdcacm_t acm_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = 64,
        .callback_rx = &tinyusb_cdc_rx_callback, // the first way to register a callback
        .callback_rx_wanted_char = NULL,
        .callback_line_state_changed = NULL,
        .callback_line_coding_changed = NULL
    };

    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));
    //esp_tusb_init_console(TINYUSB_CDC_ACM_0); // log to usb
}
