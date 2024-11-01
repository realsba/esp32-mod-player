#include "Module.hpp"

#include <nvs_flash.h>
#include <esp_log.h>
#include <driver/i2s_std.h>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <esp_spiffs.h>

constexpr auto TAG            = "mod-player";
constexpr int MIXER_FREQUENCY = 44100;
constexpr int BUFFER_SIZE     = 1024;
char buff[8192];

Module module(MIXER_FREQUENCY);
QueueHandle_t tx_queue;

bool IRAM_ATTR i2s_tx_buffer_callback(i2s_chan_handle_t handle, i2s_event_data_t* eventData, void* userData)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(tx_queue, &eventData->size, &xHigherPriorityTaskWoken);
    return (xHigherPriorityTaskWoken == pdTRUE);
}

void i2s_fill_buffer(void* audioBuffer, size_t len)
{
    auto buffer       = static_cast<int16_t*>(audioBuffer);
    size_t samples    = len / sizeof(int16_t);
    auto volumeFactor = 0.06f;

    for (size_t i = 0; i < samples; ++i) {
        buffer[i] = static_cast<int16_t>(module.getFrame() * volumeFactor);
    }
}

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Start");

    esp_vfs_spiffs_conf_t spiffsConfig = {
        .base_path = "/spiffs",
        .partition_label = nullptr,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t res = esp_vfs_spiffs_register(&spiffsConfig);
    if (res != ESP_OK) {
        if (res == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (res == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(res));
        }
        return;
    }

    module.load("/spiffs/popcor10.mod");

    tx_queue = xQueueCreate(10, sizeof(size_t));
    if (tx_queue == nullptr) {
        ESP_LOGE(TAG, "Failed to create tx_queue");
        return;
    }

    i2s_chan_handle_t channel;
    i2s_chan_config_t channelConfig = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    channelConfig.dma_frame_num = 2048;
    ESP_ERROR_CHECK(i2s_new_channel(&channelConfig, &channel, nullptr));

    i2s_std_config_t stdConfig = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(MIXER_FREQUENCY),
        .slot_cfg = {
            .data_bit_width = I2S_DATA_BIT_WIDTH_16BIT,
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_16BIT,
            .slot_mode = I2S_SLOT_MODE_MONO,
            .slot_mask = I2S_STD_SLOT_BOTH,
            .ws_width = 16,
            .ws_pol = false,
            .bit_shift = false,
#if !SOC_I2S_HW_VERSION_1
            .left_align = false,
            .big_endian = false,
            .bit_order_lsb = false,
#endif
            .msb_right = false
        },
        .gpio_cfg = {
            .mclk = GPIO_NUM_0,
            .bclk = GPIO_NUM_26,
            .ws = GPIO_NUM_25,
            .dout = GPIO_NUM_22,
            .din = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = 0,
                .bclk_inv = 0,
                .ws_inv = 0
            }
        },
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(channel, &stdConfig));

    i2s_event_callbacks_t callbacks = {
        .on_recv = nullptr,
        .on_recv_q_ovf = nullptr,
        .on_sent = i2s_tx_buffer_callback,
        .on_send_q_ovf = nullptr
    };
    ESP_ERROR_CHECK(i2s_channel_register_event_callback(channel, &callbacks, nullptr));

    ESP_ERROR_CHECK(i2s_channel_enable(channel));

    size_t dmaSize = 0;
    while (true) {
        if (xQueueReceive(tx_queue, &dmaSize, portMAX_DELAY)) {
            size_t bytesWritten = 0;
            size_t size         = std::min(dmaSize, sizeof(buff));
            i2s_fill_buffer(buff, size);
            if (i2s_channel_write(channel, buff, size, &bytesWritten, 1000) == ESP_OK) {
                printf("Write Task: i2s write %d bytes\n", bytesWritten);
            } else {
                printf("Write Task: i2s write failed\n");
            }
        }
    }
}
