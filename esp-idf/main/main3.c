#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "string.h"
#include "hal/usb_serial_jtag_ll.h"
void mytask1(void *pvParameter)
{
    uint8_t *rxbuf;
    int cnt;
    rxbuf = (uint8_t *)malloc(64);
    int rxcnt;
 
    while (1)
    {
        if (usb_serial_jtag_ll_rxfifo_data_available())
        {
            rxcnt = usb_serial_jtag_ll_read_rxfifo(rxbuf,64);
            cnt = (int)usb_serial_jtag_ll_write_txfifo((const uint8_t *)rxbuf, rxcnt);
            usb_serial_jtag_ll_txfifo_flush();
            //printf("Send %d characters to host \n", cnt);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    free(rxbuf);
    vTaskDelete(NULL);
}
 
void app_main(void)
{
    xTaskCreate(mytask1, "mytask1", 1024 * 5, NULL, 1, NULL);
}