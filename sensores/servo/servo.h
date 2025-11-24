#ifndef SERVO_H
#define SERVO_H

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Este metodo se encarga de inicializar el servo en el pin indicado
 * 
 * @param gpio Pin al que se desea esscribir la señal
 */
void servo_init(int gpio);

/**
 * @brief Este metodo se encarga de colocar los angulos al que se va enviar el servo
 * 
 * @param degrees El angulo al que se desea que apunte el servo
 */
void servo_set_angle(float degrees);

/**
 * @brief Este metodo se encarga de mover angulo a angulo el servo hasta la posicion deseada
 * 
 * @param start_deg Angulo de inicio
 * @param end_deg Angulo final
 * @param step_deg Cantidad de angulos por paso
 * @param step_delay_ms Tiempo entre angulos
 */
void servo_move_smooth(float start_deg, float end_deg, float step_deg, int step_delay_ms);

/**
 * @brief Este metodo pone el servo en la posicion de abierto solo para la puerta
 * 
 */
void servo_open(void);

/**
 * @brief Este metodo se encarga poner el servo en poscion de cerrado, solo para la puerta
 * 
 */
void servo_close(void);

#ifdef __cplusplus
}
#endif
#endif
