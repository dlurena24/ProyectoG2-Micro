#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "K2300PT";
const char* password = "Chocolate-2307";

WebServer server(80);

const int doorPin = 1; // PuertaControlada por este pin
bool doorOpen = false; // Estado de la puerta
unsigned long lastAction = 0;
const unsigned long debounceMs = 200; // Rebote

// Página principal simple con dos botones y estado
const char* htmlForm = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>Puerta ESP32</title>
  <style>
    body { font-family: Arial, sans-serif; display:flex; flex-direction:column; align-items:center; justify-content:center; min-height:100vh; margin:0; background:#f5f5f5; }
    .panel { background:#fff; padding:20px; border-radius:12px; box-shadow:0 2px 6px rgba(0,0,0,.15); text-align:center; }
    button { font-size:1.1em; padding:10px 20px; margin:8px; border-radius:8px; border:1px solid #0b7; background:#e6f7e6; }
  </style>
</head>
<body>
  <div class="panel">
    <h2>Estado de la puerta</h2>
    <div id="estado">Desconocido</div>
    <button onclick="activarPuerta('open')">Abrir Puerta</button>
    <button onclick="activarPuerta('close')">Cerrar Puerta</button>
  </div>
<script>
function updateEstado() {
  fetch('/door/state').then(res => res.text()).then(t => {
    document.getElementById('estado').textContent = t;
  });
}
// Nuevo: actualiza estado después de cada acción
function activarPuerta(accion) {
  fetch('/door/' + accion)
    .then(() => updateEstado()); // Corrección: actualiza tras comando
}
window.onload = updateEstado;
</script>
</body>
</html>
)rawliteral";

void handleRoot() { server.send(200, "text/html", htmlForm); }

// Endpoints de puerta
void handleDoorState() {
server.send(200, "text/plain", doorOpen ? "OPEN" : "CLOSED");
}

void handleDoorOpen() {
unsigned long now = millis();
if (now - lastAction > debounceMs) {
digitalWrite(doorPin, HIGH); // Ajusta si necesitas LOW para abrir
doorOpen = true;
lastAction = now;
}
server.send(200, "text/html", "<html><body><h2>Puerta abierta</h2><a href='/'>Volver</a></body></html>");
}

void handleDoorClose() {
unsigned long now = millis();
if (now - lastAction > debounceMs) {
digitalWrite(doorPin, LOW); // Ajusta si necesitas HIGH para cerrar
doorOpen = false;
lastAction = now;
}
server.send(200, "text/html", "<html><body><h2>Puerta cerrada</h2><a href='/'>Volver</a></body></html>");
}

void setup() {
pinMode(doorPin, OUTPUT);
digitalWrite(doorPin, LOW); // Por defecto cerrado
doorOpen = false;
lastAction = 0;

Serial.begin(115200);

WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) { delay(500); }

server.on("/", handleRoot);
server.on("/door/state", HTTP_GET, handleDoorState);
server.on("/door/open", HTTP_GET, handleDoorOpen);
server.on("/door/close", HTTP_GET, handleDoorClose);

server.begin();
}

void loop() {
server.handleClient();
}