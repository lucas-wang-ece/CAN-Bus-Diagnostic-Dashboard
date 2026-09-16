// V1: Simulated Automotive Dashboard Data

int speed = 0;
int rpm = 800;
float temp = 70.0;
float voltage = 12.4;

void setup() {
  Serial.begin(9600);  //communication speed between Arduino board and laptop

  Serial.println("Simulated Automotive CAN Bus Diagnostic Dashboard");
  Serial.println("V1: Vehicle telemetry simulation");
  Serial.println("*********************************8");
}

void loop() {
  speed = random(0, 121);   // Simulate changing vehicle data
  rpm = random(800, 4501);
  temp = random(700, 1060) / 10.0;
  voltage = random(115, 131) / 10.0;

  bool warning = false;    // Check warning status

  if (temp >= 100.0 || voltage < 12.0) {
    warning = true;
  }

  // Print dashboard data
  Serial.println();
  Serial.println("Vehicle Telemetry");
  Serial.println("***************");

  Serial.print("Speed: ");
  Serial.print(speed);
  Serial.println(" km/h");

  Serial.print("Engine RPM: ");
  Serial.print(rpm);
  Serial.println(" rpm");

  Serial.print("Coolant Temp: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Warning: ");
  if (warning) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }

  delay(2000);
}