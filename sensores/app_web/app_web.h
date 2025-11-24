#pragma once

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Este metodo termina la configuracion y pasa los valores adecuados a los metodos anteriormente mencionados
 *          permite iniciar la pagina web-desde el main del proyecto
 */
void app_web_init(void);

/**
 * @brief Este metodo se encarga de actualizar la puerta a abierto en caso de que esta llamada ocurra fuera de la logica de app_web.c
 */
void actualizar_puerta_a_abierta(void);

/**
 * @brief Este metodo se encarga de actualizar la puerta a cerrado en caso de que esta llamada ocurra fuera de la logica de app_web.c
 */
void actualizar_puerta_a_cerrada(void);

#ifdef __cplusplus
}
#endif
