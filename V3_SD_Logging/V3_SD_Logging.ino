// V3: Vehicle Telemetry microSD CSV Logging
// This program generates simulated vehicle telemetry data,
// prints it to the Serial Monitor, and logs it to a microSD card.

#include <SPI.h>  //use SPI to communicate between Arduino board and micro SD card
#include <SD.h>

const int SD_CS_PIN = 10;
const char LOG_FILE_NAME[] = "vehicle.csv";

int speed = 0;
int rpm = 800;
float temp = 70.0;
float voltage = 12.4;

void writeCsvHeaderIfNeeded() {
  if (!SD.exists(LOG_FILE_NAME)) {
    File logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);

    if (logFile) {
      logFile.println("time_ms,speed,rpm,temp,voltage,warning");
      logFile.close(); //mandatory in order to store the data successfully into the SD card
      Serial.println("CSV header written.");
    } else {
      Serial.println("Failed to write CSV header.");
    }
  }
}

void logVehicleData(unsigned long timeMs, int speed, int rpm, float temp, float voltage, bool warning) {
  File logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);

  if (logFile) {
    logFile.print(timeMs);
    logFile.print(",");

    logFile.print(speed);
    logFile.print(",");

    logFile.print(rpm);
    logFile.print(",");

    logFile.print(temp);
    logFile.print(",");

    logFile.print(voltage);
    logFile.print(",");

    if (warning) {
      logFile.println("ON");
    } else {
      logFile.println("OFF");
    }

    logFile.close();
    Serial.println("Data logged to SD card.");
  } else {
    Serial.println("Failed to open log file.");
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("V3: Vehicle Telemetry microSD CSV Logging");
  Serial.println("-----------------------------------------");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed.");
    while (true) {
      // Stop program here if SD card fails
    }
  }

  Serial.println("SD card initialized successfully.");

  writeCsvHeaderIfNeeded();

  Serial.println("time_ms,speed,rpm,temp,voltage,warning");
}

void loop() {
  speed = random(0, 121);
  rpm = random(800, 4501);
  temp = random(700, 1060) / 10.0;
  voltage = random(115, 131) / 10.0;

  bool warning = false;

  if (temp >= 100.0 || voltage < 12.0) {
    warning = true;
  }

  unsigned long timeMs = millis();

  Serial.print(timeMs);
  Serial.print(",");

  Serial.print(speed);
  Serial.print(",");

  Serial.print(rpm);
  Serial.print(",");

  Serial.print(temp);
  Serial.print(",");

  Serial.print(voltage);
  Serial.print(",");

  if (warning) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }

  logVehicleData(timeMs, speed, rpm, temp, voltage, warning);

  delay(2000);
}