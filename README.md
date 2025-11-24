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

### Instalar ESP-IDF en VsCode

<img width="1308" height="734" alt="33" src="https://github.com/user-attachments/assets/9c2e1b33-835f-4be5-90ee-59841e84ac68" />

### Abrir el setup de ESP-IDF
<img width="1009" height="702" alt="44" src="https://github.com/user-attachments/assets/56e146b9-7cf7-4fb3-b6da-52a2cc87e608" />

### Configurarlo de acuerdo a lo observado en la imagen y apretar instalar
<img width="1148" height="711" alt="11" src="https://github.com/user-attachments/assets/f79b0aa7-3b95-4f43-a45a-9988973f7a05" />

## Una vez configurado e instalado, se abrira una pestaña como la siguiente:
#<img width="1360" height="768" alt="22" src="https://github.com/user-attachments/assets/53a5793c-424a-4452-9afd-e53fa1144b1c" />

## Desde la barra de VsCode, abrir el folder del proyecto
<img width="1360" height="768" alt="55" src="https://github.com/user-attachments/assets/1c910780-cdbc-4570-9e6d-5e53498b7630" />

## Seleccionar la carpeta de "sensores"
<img width="1360" height="768" alt="66" src="https://github.com/user-attachments/assets/004895bd-7cd5-462d-ad8d-1749979babbf" />

## Abrir el menú lateral de EXPRESSIF y seleccionar "Full clean" para poder seleccionar "Build project" 
<img width="1360" height="768" alt="77" src="https://github.com/user-attachments/assets/7c9f0202-e35e-41e3-8285-75b02fc0cfec" />

- *Nota:* Recuerde seleccionar el puerto donde se encuentra la ESP32, y si no utiliza una ESP32, seleccionar "Set EXPRESSIF device target" para poder definir cual es la placa a la que se le va a hacer el build. Seguidamente puede seleccionar "Flash device" para subir su imagen al dispositivo.
---

## Dependencias Necesarias

- Framework ESP-IDF

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
| 7 | **Filamento PLA+** | Para impresiones 3D de los rieles del prototipo | 1 |
| 8 | **Lámina Cartofoam** | Tamaño 82 x 102 cm | 1 |

---

## Diagrama del Circuito


<img width="768" height="694" alt="DiagramaP2Arqui (3)" src="https://github.com/user-attachments/assets/b71af027-806c-4100-8dbd-e0f93328d6a3" />

