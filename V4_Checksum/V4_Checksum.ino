// V4: Vehicle Telemetry microSD CSV Logging with Checksum

#include <SPI.h>
#include <SD.h>

const int SD_CS_PIN = 10;
const char LOG_FILE_NAME[] = "vehicle.csv";

int speed = 0;
int rpm = 800;
float temp = 70.0;
float voltage = 12.4;

byte calculateChecksum(String dataLine) {
  byte checksum = 0;

  for (int i = 0; i < dataLine.length(); i++) {
    checksum += dataLine[i];
  }

  return checksum;
}

void writeCsvHeaderIfNeeded() {
  if (!SD.exists(LOG_FILE_NAME)) {
    File logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);

    if (logFile) {
      logFile.println("time_ms,speed,rpm,temp,voltage,warning,checksum");
      logFile.close();
      Serial.println("CSV header written.");
    } else {
      Serial.println("Failed to write CSV header.");
    }
  }
}

void logVehicleData(unsigned long timeMs, int speed, int rpm, float temp, float voltage, bool warning) {
  String warningText;

  if (warning) {
    warningText = "ON";
  } else {
    warningText = "OFF";
  }

  String dataLine = "";
  dataLine += String(timeMs);
  dataLine += ",";
  dataLine += String(speed);
  dataLine += ",";
  dataLine += String(rpm);
  dataLine += ",";
  dataLine += String(temp);
  dataLine += ",";
  dataLine += String(voltage);
  dataLine += ",";
  dataLine += warningText;

  byte checksum = calculateChecksum(dataLine);

  File logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);

  if (logFile) {
    logFile.println(dataLine + "," + String(checksum));
    logFile.close();

    Serial.println(dataLine + "," + String(checksum));
    //Serial.println("Data logged to SD card with checksum.");
  } else {
    Serial.println("Failed to open log file.");
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("V4: Vehicle Telemetry microSD CSV Logging with Checksum");
  Serial.println("------------------------------------------------------");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed.");
    while (true) {
      // Stop program here if SD card fails
    }
  }

  Serial.println("SD card initialized successfully.");

  writeCsvHeaderIfNeeded();

  Serial.println("time_ms,speed,rpm,temp,voltage,warning,checksum");
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

  logVehicleData(timeMs, speed, rpm, temp, voltage, warning);

  delay(2000);
}