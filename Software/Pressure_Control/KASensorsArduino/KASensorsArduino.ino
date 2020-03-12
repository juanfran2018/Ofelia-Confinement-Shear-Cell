/*
  KA Sensors analogic reading

  An analogical signal reading software for the KA Sensor ASH-A1000-4A1-AD-2V-050-000-000.
  A 0 - 4 V signal is sent to Arduino and then processed with the manufacture calibration values.

*/


int sensorPin = A0;    // input pin
float sensorValue = 0;  // variable to store the value coming from the sensor
float offset = -2.25439; // pressure offset of the sensor
float PressureTotal = 10.0; // Pressure range
float pvav = 0; // Pressure average value of 10 measurements 
int count = 0;
float pv = 0; // Instant pressure value
float sd = 0; // Standard deviation of 10 readings

void setup() {
  Serial.begin(115200);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin) * 1.0 / 1023.0 * 5.0;
  sensorValue = offset + PressureTotal / (4.501 - 0.498) * sensorValue;
  sk = sk + sensorValue / 10.0;
  pv = pv + (sensorValue) * (sensorValue) / 10.0;
  count = count + 1;
  if (count == 10) {
    sd = sqrt(abs(pv - pvav * pvav));
    Serial.print(pvav, 4);
    Serial.print(",");
    Serial.println(sd, 4);
    count = 0;
    sk = 0;
    sd = 0;
    pv = 0;
  }
  delay(100);
}
