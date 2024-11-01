idf_component_register(
    SRCS "main.cpp"
    REQUIRES nvs_flash spiffs esp_driver_i2s esp_driver_gpio esp32-mod-player
    INCLUDE_DIRS "."
)

#spiffs_create_partition_image(storage ../storage FLASH_IN_PROJECT)
