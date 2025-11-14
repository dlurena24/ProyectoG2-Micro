bool doorOpen = false;

const int distanceToOpen = 30;                  // 30cm
const unsigned long timeToWaitForClose = 3000;  // 3s

unsigned long lastPresenceTime = 0;

int readDistance() {
  // TODO: Replace with real ultrasonic reading logic
  return 25;
}

void openDoor() {
  if (!doorOpen) {
    Serial.println("Opening door...");
    // Code to move servo motor
    doorOpen = true;
  }
}

void closeDoor() {
  if (doorOpen) {
    Serial.println("Closing door...");
    // Code to move servo motor
    doorOpen = false;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Automatic door system started.");
}

void loop() {
  int distance = readDistance();

  // If presence is detected, open door
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