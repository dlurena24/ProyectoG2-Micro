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
