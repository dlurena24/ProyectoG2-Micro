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

# Mostrar los eventos en el HTML

El programa envia los eventos en el siguiente formato:

```json
{ "event": "door_action", "message": "Opening door", "timestamp": 123456 }
```

Hay que hacer que:

1. El ESP32 que tiene la **web app** lea los eventos vía `Serial.read()`.
2. Despues guarde los eventos leídos en un **array** de eventos.
3. El HTML haga `fetch("/events)`.
4. El ESP32 responda con un **JSON** que contenga todos los eventos.

## Buffer de eventos

```cpp
String eventLog[20];
int eventIndex = 0;

void addEvent(String e) {
  eventLog[eventIndex] = e;
  eventIndex = (eventIndex + 1) % 20; // Circular buffer
}
```

## Leer serial

```cpp
void readSerialEvents() {
  while (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      addEvent(line);
    }
  }
}
```

## Crear endpoint para el monitoreo

```cpp
void handleEvents() {
  String json = "[";
  for (int i = 0; i < 20; i++) {
    if (eventLog[i].length() > 0) {
      json += eventLog[i] + ",";
    }
  }
  if (json.endsWith(",")) json.remove(json.length() - 1);
  json += "]";

  server.send(200, "application/json", json);
}
```

## Agregar endpoint

En `setup()`:

```cpp
server.on("/events", HTTP_GET, handleEvents);
```

## Mostrar eventos en el HTML

```cpp
<script>
function updateEvents() {
  fetch("/events")
    .then(r => r.json())
    .then(events => {
      const div = document.getElementById("monitor");
      div.innerHTML = events.map(e => `<p>${e}</p>`).join("");
    });
}

setInterval(updateEvents, 500); // cada medio segundo
</script>

<div id="monitor"></div>
```


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
| 7 | **Filamento PLA+** | Para impresiones 3D de los rieles del prototipo | 1 |
| 8 | **Lámina Cartofoam** | Tamaño 82 x 102 cm | 1 |

---

## Diagrama del Circuito


<img width="768" height="694" alt="DiagramaP2Arqui (3)" src="https://github.com/user-attachments/assets/b71af027-806c-4100-8dbd-e0f93328d6a3" />

