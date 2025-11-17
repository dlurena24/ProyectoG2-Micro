idf_component_register(
    SRCS
        "acelerometro.c"
        "acelerometro_i2c.c"
    INCLUDE_DIRS
        "."
    REQUIRES
        soc
        driver
        esp_common
)

