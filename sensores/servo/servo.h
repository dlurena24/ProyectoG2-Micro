#ifndef SERVO_H
#define SERVO_H

#ifdef __cplusplus
extern "C" {
#endif

void servo_init(int gpio);
void servo_set_angle(float degrees);
void servo_open(void);
void servo_close(void);

#ifdef __cplusplus
}
#endif
#endif
