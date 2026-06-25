#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

HardwareSerial dfSerial(2);
DFRobotDFPlayerMini mp3;

// Flex pins
#define F1 34
#define F2 35
#define F3 32
#define F4 33

// Thresholds (adjust after testing)
int TH1 = 2300;
int TH2 = 2300;
int TH3 = 2300;
int TH4 = 2300;

unsigned long lastTriggerTime = 0;
int cooldown = 1500; // 1.5 seconds

void setup() {
  Serial.begin(115200);
  dfSerial.begin(9600, SERIAL_8N1, 26, 27);

  if (!mp3.begin(dfSerial)) {
    Serial.println("DFPlayer error");
    while (true);
  }

  mp3.volume(25);
}

void loop() {
  int v1 = analogRead(F1);
  int v2 = analogRead(F2);
  int v3 = analogRead(F3);
  int v4 = analogRead(F4);

  // Print values (for tuning)
  Serial.printf("%d | %d | %d | %d\n", v1, v2, v3, v4);

  // Cooldown check
  if (millis() - lastTriggerTime < cooldown) return;

  // Find MAX sensor
  int maxVal = v1;
  int sensor = 1;

  if (v2 > maxVal) { maxVal = v2; sensor = 2; }
  if (v3 > maxVal) { maxVal = v3; sensor = 3; }
  if (v4 > maxVal) { maxVal = v4; sensor = 4; }

  // Trigger only if above threshold
  if ((sensor == 1 && v1 > TH1) ||
      (sensor == 2 && v2 > TH2) ||
      (sensor == 3 && v3 > TH3) ||
      (sensor == 4 && v4 > TH4)) {

    mp3.playMp3Folder(sensor); // plays 000X.mp3
    Serial.print("Playing: "); Serial.println(sensor);

    lastTriggerTime = millis(); // start cooldown
  }

  delay(50);
}