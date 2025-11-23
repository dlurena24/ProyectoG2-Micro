#ifndef SERVO_H
#define SERVO_H

#ifdef __cplusplus
extern "C" {
#endif

void servo_init(int gpio);
void servo_set_angle(float degrees);
void servo_open(void);
void servo_close(void);
void servo_move_smooth(float start_deg, float end_deg, float step_deg, int step_delay_ms);

#ifdef __cplusplus
}
#endif
#endif
