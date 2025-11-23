### Sistema de Puerta Automática Inteligente 
# Arquitectura de Software Embebido  

---

## 1. Visión General

El software está desarrollado en **ESP-IDF** y ejecutado sobre **FreeRTOS**, aprovechando periféricos nativos del ESP32-S3:

- RMT v2 → PWM para el servo  
- GPTimer → medición precisa de ultrasonido  
- I2C Controller → MPU6050  
- HTTP Server → aplicación web integrada  
- NVS Flash → configuración persistente  

---

## 2. Módulos del Sistema

### 2.1 Módulo de acelerómetro (I2C)
Responsable de:

- Inicializar I2C  
- Configurar el MPU6050  
- Leer datos crudos y convertirlos a G  
- Detectar impactos mediante variación respecto al baseline  

### 2.2 Módulo de ultrasonido (Interrupciones + Timer)
Incluye:

- ISR para flancos de ECHO  
- GPTimer a 1 MHz  
- Cálculo del pulso ECHO → distancia en cm  
- Timeout para evitar bloqueos

### 2.3 Módulo del servomotor (RMT PWM)
- RMT genera una señal estable 50 Hz  
- Conversión de grados → µs  
- Movimientos suaves con incrementos de 2°  

### 2.4 Controlador de estados de la puerta
Estados:

- STOPPED
- OPENING
- CLOSING

Eventos:

- `distance <= threshold` → abrir  
- `timeout` → cerrar  
- `impact` → detener o reabrir por seguridad  

---

## 3. Diagrama de Flujo del Programa Principal

---

## 4. Estrategia de Gestión de E/S

| Dispositivo | Estrategia | Justificación |
|-------------|------------|---------------|
| HC-SR04 | **Interrupción GPIO + Timer** | Requiere precisión de µs |
| MPU6050 | **I2C sondeado** | Cambios lentos, menor ruido en interrupciones |
| Servo | **RMT PWM** | PWM estable + mínimo uso de CPU |
| Web | **Eventos vía HTTP** | Fácil integración y depuración |

---

## 5. Organización de Memoria

### ✔ RAM
- Tareas FreeRTOS → stacks de 4 KB  
- Buffers de I2C, lecturas y logs → <1 KB  
- Servidor HTTP → ~30-40 KB  

### ✔ Flash
- Firmware C  
- Páginas HTML embebidas  
- NVS para configuraciones

---

## 6. Eventos del Sistema (Logging)
Cada lectura genera:

```json
{ 
  "event": "door_action / acceleration / distance / impact",
  "message": "...",
  "timestamp": <ms>
}
