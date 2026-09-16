// V2: CSV-style Vehicle Telemetry Output

int speed = 0;
int rpm = 800;
float temp = 70.0;
float voltage = 12.4;

void setup() {
  Serial.begin(9600);
  delay(1000);    // This delay is make sure we can see the CSV header
  Serial.println("time_ms,speed,rpm,temp,voltage,warning");   // Print CSV header
}

void loop() {
  speed = random(0, 121);     // Simulate changing vehicle data
  rpm = random(800, 4501);
  temp = random(700, 1060) / 10.0;
  voltage = random(115, 131) / 10.0;

  bool warning = false;   // Check warning status

  if (temp >= 100.0 || voltage < 12.0) {
    warning = true;
  }

  // Print one CSV row
  Serial.print(millis());  //record the time since Arduino board is turned on in ms
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

  delay(2000);
}
