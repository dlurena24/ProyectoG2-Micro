
// =======================
// DOOR PARAMS
// =======================

const int distanceToOpen = 30;                  // 30cm to detect human presence
const unsigned long timeToWaitForClose = 3000;  // 3s to wait for closing the door
const float impactThreshold = 2.5;              // 2.5m/s^2 of limit to detect hit

// =======================
// DOOR STATE
// =======================

enum DoorState { STOPPED, OPENING, CLOSING };
DoorState doorState = STOPPED;

// Time when the last person use the door
unsigned long lastPresenceTime = 0;

// =======================
// EVENT SYSTEM
// =======================

void emitEvent(const char* eventType, const char* message) {
  // Esta función prepara los datos para enviarlos a la app web
  // Se recomienda usar JSON porque el HTML lo puede consumir fácil
  Serial.print("{ \"event\": \"");
  Serial.print(eventType);
  Serial.print("\", \"message\": \"");
  Serial.print(message);
  Serial.print("\", \"timestamp\": ");
  Serial.print(millis());
  Serial.println(" }");
}

// =======================
// SENSOR READING
// =======================

int readDistance() {
  // TODO: Replace with real ultrasonic reading logic
  int dist = 25;

  // Emit event for distance
  char buffer[50];
  sprintf(buffer, "Distance read: %d cm", dist);
  emitEvent("distance", buffer);

  return dist;
}

float readAccelerometer() {
  // TODO: Replace with real accelerometer reading
  float acceleration = 0.5;  // value "normal"

  // Emit event for acceleration
  char buffer[60];
  sprintf(buffer, "Acceleration: %.2f m/s^2", acceleration);
  emitEvent("acceleration", buffer);

  return acceleration;
}

// =======================
// DOOR ACTIONS
// =======================

void openDoor() {
  if (doorState == OPENING) return;

  // Emit event for opening the door
  emitEvent("door_action", "Opening door");

  // TODO: Replace with code to move servo motor

  doorState = OPENING;
}

void closeDoor() {
  if (doorState == CLOSING) return;

  // Emit event for closing the door
  emitEvent("door_action", "Closing door");

  // TODO: Replace with code to move servo motor

  doorState = CLOSING;
}

void stopDoor() {
  emitEvent("door_action", "Door stopped");

  // TODO: Replace with code to stop the servo motor

  doorState = STOPPED;
}

void forceOpenDoor() {
  emitEvent("door_action", "Force opening door");

  // TODO: Replace with code to move servo motor

  doorState = OPENING;
}

void handleImpact() {
  if (doorState == CLOSING) {
    emitEvent("safety_stop", "Impact detected while closing. Reopening door.");

    // Stop motor and immediately reverse direction
    forceOpenDoor();
  } else {
    emitEvent("safety_stop", "Impact detected, stopping door movement.");
    stopDoor();
  }
}

// =======================
// SETUP
// =======================

void setup() {
  Serial.begin(115200);

  // Emit event for starting the door system
  emitEvent("system", "Automatic door system started");
}

// =======================
// MAIN LOOP
// =======================

void loop() {
  // Read Accelerometer
  float acceleration = readAccelerometer();
  if (acceleration >= impactThreshold) {
    emitEvent("impact", "Impact detected");
    handleImpact();
  }

  // Read Ultrasonic Sensor
  int distance = readDistance();
  if (distance <= distanceToOpen) {
    openDoor();
    lastPresenceTime = millis();
  }

  // If no presence for a while, close door
  if (doorOpen && (millis() - lastPresenceTime > timeToWaitForClose)) {
    closeDoor();
  }

  delay(100);
}