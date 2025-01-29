#include <nrfx_pdm.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <math.h>

LOG_MODULE_REGISTER(pdm_sample, LOG_LEVEL_INF);

#define PDM_CLK_PIN 9  // Pin P0.09 for PDM clock
#define PDM_DIN_PIN 8  // Pin P0.08 for PDM data

// Buffer for PDM audio data
static int16_t pdm_buffer[128];
static int message_count = 0; // Counter to track messages

// PDM instance
static nrfx_pdm_t pdm_instance = NRFX_PDM_INSTANCE(0);

static void pdm_event_handler(nrfx_pdm_evt_t const *event) {
    if (event->error) {
        LOG_ERR("PDM error: %d", event->error);
        return;
    }

    if (event->buffer_requested) {
        nrfx_pdm_buffer_set(&pdm_instance, pdm_buffer, ARRAY_SIZE(pdm_buffer));
    }

    if (event->buffer_released) {
        if (event->buffer_released == NULL) {
            LOG_ERR("PDM buffer is NULL!");
            return;
        }

        message_count++;
        if (message_count % 10 == 0) { // Log every 10th message
            message_count=1;
            printk("PCM Data: ");
            for (size_t i = 0; i < ARRAY_SIZE(pdm_buffer); i += 8) { // Print every 8th sample
                printk("%d,", event->buffer_released[i]);
            }
            printk("\n");
        }
    }
}

int main(void) {
    LOG_INF("Initializing PDM...");

    nrfx_pdm_config_t pdm_config = NRFX_PDM_DEFAULT_CONFIG(PDM_CLK_PIN, PDM_DIN_PIN);
    pdm_config.clock_freq = NRF_PDM_FREQ_1333K;  // Set an appropriate clock frequency
    pdm_config.gain_l = NRF_PDM_GAIN_DEFAULT;
    pdm_config.gain_r = NRF_PDM_GAIN_DEFAULT;

    int ret = nrfx_pdm_init(&pdm_instance, &pdm_config, pdm_event_handler);
    if (ret != NRFX_SUCCESS) {
        LOG_ERR("Failed to initialize PDM: %d", ret);
        return -1;
    }

    ret = nrfx_pdm_start(&pdm_instance);
    if (ret != NRFX_SUCCESS) {
        LOG_ERR("Failed to start PDM: %d", ret);
        return -1;
    }

    LOG_INF("PDM started successfully");

    while (1) {
        k_sleep(K_MSEC(1000));  // Add a delay to prevent CPU overuse
    }
    return 0;
}