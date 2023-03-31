#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "linenoise/linenoise.h"
#include "esp_console.h"
#include "esp_vfs_cdcacm.h"

static void initialize_console(void)
{
    /* Disable buffering on stdin */
    setvbuf(stdin, NULL, _IONBF, 0);

    /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
    esp_vfs_dev_cdcacm_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_cdcacm_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);

    /* Enable non-blocking mode on stdin and stdout */
    fcntl(fileno(stdout), F_SETFL, 0);
    fcntl(fileno(stdin), F_SETFL, 0);

    /* Initialize the console */
    esp_console_config_t console_config = {
            .max_cmdline_args = 8,
            .max_cmdline_length = 256,
    };
    ESP_ERROR_CHECK( esp_console_init(&console_config) );

    /* Configure linenoise line completion library */
    /* Enable multiline editing. If not set, long commands will scroll within
     * single line.
     */
    linenoiseSetMultiLine(1);

    /* Tell linenoise where to get command completions and hints */
    linenoiseSetCompletionCallback(&esp_console_get_completion);
    linenoiseSetHintsCallback((linenoiseHintsCallback*) &esp_console_get_hint);

    /* Set command history size */
    linenoiseHistorySetMaxLen(10);
}

void app_main(void) {
    initialize_console();

    while(true){
        /* Get a line using linenoise.
         * The line is returned when ENTER is pressed.
         */
        char* line = linenoise("");
        if (line == NULL) { /* Ignore empty lines */
            continue;
        }
        
        if (strcmp(line, "generatefunnyjoke") == 0) {
            printf("{\"joke\":\"Why did the math book look so sad? - Because it had too many problems!\"}\n");
        } else if (strcmp(line, "random_number") == 0) {
            printf("{\"number\":\"4\"}\n"); // https://xkcd.com/221/
        } else {
            printf("Unrecognized command. Please try again...\n");
        }

        fflush(stdout);

        /* linenoise allocates line buffer on the heap, so need to free it */
        linenoiseFree(line);
    }

    // Never reached code, but just in case...
    esp_restart();
}
