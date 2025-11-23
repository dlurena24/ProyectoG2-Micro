### Sistema de Puerta Automática Inteligente  
# Diseño de Hardware  


---

## 1. Selección del Microcontrolador
## Comparación entre plataformas evaluadas

Para el sistema de puerta automática se evaluaron diversas opciones de microcontroladores y SoCs utilizados comúnmente en proyectos de automatización, IoT y control mecatrónico. La selección final del **ESP32-S3 WROOM-1** se fundamentó en un equilibrio entre potencia de cómputo, periféricos integrados, facilidad de desarrollo, conectividad inalámbrica y costo.

A continuación, se presenta la comparación con alternativas relevantes:

---

### *Opción 1: STM32 (F103 / F401 / L4)*  
**Ventajas:**  
- Rendimiento superior en control en tiempo real  
- ADC/DAC avanzados  
- Ecosistema industrial

**Limitaciones:**  
- **No incluye Wi-Fi** → se necesita un ESP8266 o ESP32 adicional  
- Capa de software más compleja (HAL/LL)  
- Mayor costo final  
- Tiempo de desarrollo más elevado  
- Requiere programador ST-Link

**Conclusión:** Muy buenos para robots industriales, pero sobredimensionados y sin conectividad inalámbrica integrada, lo que aumenta costo y complejidad.

---

### *Opción 2: PIC32MX / PIC32MZ*  
**Ventajas:**  
- Alto rendimiento  
- Arquitectura MIPS eficiente

**Limitaciones:**  
- No traen Wi-Fi ni BLE  
- Documentación menos accesible  
- Programación más compleja  
- Costo significativamente mayor  
- Menor ecosistema de librerías y soporte moderno

**Conclusión:** No es coste-efectivo para un sistema pequeño e IoT.

---

### *Opción 3: ESP32 clásico (DevKit V1)*  
**Ventajas:**  
- Wi-Fi + BLE integrados  
- Buena relación costo/rendimiento  
- Amplia comunidad  

**Limitaciones vs ESP32-S3:**  
- Núcleo más antiguo (LX6)  
- Bluetooth 4.2 en lugar de BLE 5  
- Menor sensibilidad de antena  
- RMT y timers menos eficientes  
- Menor capacidad de RAM interna

**Conclusión:** Una opción viable, pero técnicamente inferior al S3, especialmente en conectividad y periféricos avanzados.

---

### Opción seleccionada: ESP32-S3 WROOM-1

**Ventajas determinantes:**

| Característica | Detalle |
|----------------|---------|
| **CPU** | Dual-core Xtensa LX7 240 MHz (30% más eficiente que LX6) |
| **Conectividad** | Wi-Fi 802.11 b/g/n + Bluetooth 5 LE integrado |
| **Periféricos** | RMT v2, timers de 1 µs, GPIO Matrix flexible |
| **Memoria** | 512 KB RAM + opción PSRAM externa |
| **Seguridad** | Secure Boot + Flash Encryption |
| **Costo** | Bajo comparado con STM32 + módulo Wi-Fi |
| **Ecosistema** | ESP-IDF con soporte oficial y librerías maduras |

### Razones clave por las que el ESP32-S3 es la mejor opción:
- **Único microcontrolador que combina Wi-Fi + BLE + periféricos avanzados + bajo costo**  
- Permite ejecutar **un servidor web completo**, cosa que los demás no pueden sin hardware extra  
- Manejo preciso del servo gracias al **RMT v2**, mejor que PWM por software en Arduino o en RP2040  
- Lectura de ultrasonido precisa usando GPTimers a 1 MHz (no presente en Arduino)  
- I2C rápido y estable para el MPU-6050  
- Excelente balance entre capacidad y simplicidad de desarrollo  
- Consumo energético menor que otras plataformas


---

## 2. Sensores y Actuadores

### 2.1 Sensor Ultrasónico HC-SR04  
Usado para detectar presencia mediante medición de distancia.

- Alimentación: 5V  
- Trigger: pulso de 10 µs  
- Echo: captura mediante interrupciones  
- Distancia útil: 2–400 cm  
- Fórmula implementada:  
  `distancia_cm = tiempo_us * 0.01715`

**Integración con el ESP32-S3:**  
- El ECHO utiliza interrupciones del GPIO Matrix + GPTimer a 1 MHz para medir microsegundos con alta precisión.

---

### 2.2 Acelerómetro/Giroscopio MPU-6050 (I2C)

Detecta impactos en la estructura de la puerta.

- Protocolo: I2C @ 100 kHz
- Registros leídos: 6 bytes consecutivos del acelerómetro  
- Se calcula magnitud usando:  
  `sqrt(ax² + ay² + az²) * 9.80665`

**Ventajas con ESP32-S3:**  
- Controlador I2C estable  
- Menos jitter en lecturas repetidas

---

### 2.3 Servomotor de 5V (SG90/MG996R equivalente)

Control del actuador principal de la puerta.

- Posición: 0° a 120° según apertura  
- PWM: 50 Hz  
- Pulso: 500–2400 µs  
- Control mediante RMT del S3, no con PWM por software.

---

## 3. Conexiones de Hardware

### 3.1 Conexiones de pines 
| Dispositivo | Pin ESP32-S3 | Notas |
|-------------|--------------|-------|
| HC-SR04 TRIG | GPIO13 | Salida digital |
| HC-SR04 ECHO | GPIO11 | Interrupción + Timer |
| MPU6050 SDA | GPIO5 | I2C |
| MPU6050 SCL | GPIO4 | I2C |
| Servo PWM | GPIO18 | Canal RMT |
| Fuente 5V | Ext. | Alimentación servo + regulador ESP32 |

### 3.2 Diagrama de conexión 

<img width="768" height="694" alt="DiagramaP2Arqui (3)" src="https://github.com/user-attachments/assets/d098e6f4-b875-4920-8487-6f87e23f0f14" />

---

## 4. Análisis de Consumo

| Componente | Consumo |
|-----------|----------|
| ESP32-S3 | 80–150 mA |
| Servo | 250–900 mA (picos) |
| HC-SR04 | 15 mA |
| MPU-6050 | 5 mA |

Fuente usada: 5V / 2.5A → suficiente para picos del servomotor.

---

## 5. Diseño de Tarjeta Perforada

<img width="1294" height="616" alt="DiagramaPlaca" src="https://github.com/user-attachments/assets/638d11c7-265a-4be2-ae01-a7efa1e48f1d" />

---

## 6. Consideraciones de Diseño Mecánico

- El servo mueve la puerta mediante un piñon y una cremallera diseñados e impresos en 3D con filamento PCA.
- La estructura se fabricó con cartofoam y perfiles impresos en PLA+.
- Se eligió un ángulo máximo de 180° para proteger engranajes.

---
