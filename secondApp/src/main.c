#include <nrfx_pdm.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2s.h>
#include <zephyr/logging/log.h>
#include <math.h>

LOG_MODULE_REGISTER(pdm_sample, LOG_LEVEL_INF);

#define PDM_CLK_PIN 9  // Pin P0.09 for PDM clock
#define PDM_DIN_PIN 8  // Pin P0.08 for PDM data
#define I2S_DEV DEVICE_DT_GET(DT_NODELABEL(i2s0))
#define CODEC_DEV DEVICE_DT_GET(DT_NODELABEL(cs47l63))


// Buffer for PDM audio data
static int16_t pdm_buffer[128];
static int16_t inverted_pdm_buffer[128];
static int message_count = 0; // Counter to track messages

//Send PCM data to the codec
void send_pcm_to_codec(int16_t *buffer, size_t size) {
    struct i2s_config i2s_cfg = {
        .word_size = 16,
        .channels = 2,  // Stereo output
        .format = I2S_FMT_DATA_FORMAT_I2S,
        .frame_clk_freq = 16000,  // Match PDM sample rate
    };

    i2s_configure(I2S_DEV, I2S_DIR_TX, &i2s_cfg);
    i2s_write(I2S_DEV, buffer, size * sizeof(int16_t));
}




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

        // Invert PCM signal
        for (size_t i = 0; i < ARRAY_SIZE(pdm_buffer); i++) {
            pdm_buffer[i] = -event->buffer_released[i];
        }

        // Send inverted PCM to CS47L63 via I2S
        send_pcm_to_codec(pdm_buffer, ARRAY_SIZE(pdm_buffer));
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
