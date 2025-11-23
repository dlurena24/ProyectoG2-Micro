# Sistema de Puerta Automática Inteligente

Control por presencia y gestión remota vía Wi-Fi

---

## Descripción del Proyecto

Sistema de automatización de puertas que combina detección inteligente de presencia con control remoto via Wi-Fi, eliminando el contacto físico innecesario y mejorando la accesibilidad universal.

### Problema que Resuelve

- **Contacto físico innecesario**: Reduce la transmisión de enfermedades mediante apertura sin contacto
- **Barreras de accesibilidad**: Facilita el acceso a personas con movilidad reducida o transportando objetos
- **Control de accesos limitado**: Proporciona gestión eficiente y económica de entradas
- **Soluciones inaccesibles**: Alternativa económica a sistemas de automatización costosos y complejos

### Características Principales

#### Detección Automática
- **Sensor análogo acelerómetro** para detección de movimiento por infrarrojos
- **Sensor digital ultrasónico** para medición precisa de distancia (2-400 cm)
- Apertura automática sin contacto físico

#### Control y Monitoreo Remoto
- Gestión completa desde smartphone via Wi-Fi
- Apertura/cierre manual desde la aplicación móvil
- Estado actual de la puerta (abierta/cerrada)
- Registro de eventos de acceso

---

## Instrucciones de Compilación y Ejecución

---

## Dependencias Necesarias

- Framework ESP IDS

---

## Lista de Materiales

| # | Componente | Especificaciones | Cantidad |
|---|------------|------------------|----------|
| 1 | **Microcontrolador ESP32** | ESP32 DevKit V1, Dual-core 240MHz, WiFi + BLE, 38 pines | 1 |
| 2 | **Sensor Acelerómetro** | MPU-6050, 6DOF, Acelerómetro ±16 g, Giroscópio ±2000 °/s, 3.3-5V | 1 |
| 3 | **Sensor Ultrasónico** | HC-SR04, Rango 2 - 400 cm, Precisión ±3mm, 5V | 1 |
| 4 | **Servomotor 180°** | 3,7 - 7,2Vcc 20mA, 10 Kgf/cm, Velocidad <0,16 s | 1 |
| 5 | **Batería 9V** |  | 1 |
| 6 | **Placa Perforada** | Tamaño 76 x 125 mm | 1 |
| 7 | **Filamento PCA** | Para impresiones 3D de los rieles del prototipo | 1 |
| 8 | **Lámina Cartofoam** | Tamaño 82 x 102 cm | 1 |

---

## Diagrama del Circuito


<img width="768" height="694" alt="DiagramaP2Arqui (2)" src="https://github.com/user-attachments/assets/09dedde8-b4a6-4d71-9655-420ff49fe1dd" />


