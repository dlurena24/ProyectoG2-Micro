idf_component_register(
    SRCS
        "app_web.c"
    INCLUDE_DIRS
        "."
    PRIV_REQUIRES
        driver
        esp_http_server
        nvs_flash
        esp_wifi
        esp_netif
        mbedtls

)