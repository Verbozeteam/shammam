// To run this example: python shammam.py -s <this file>

void setup() {
    Serial.begin(9600);
}

void loop() {
    while (Serial.available())
        Serial.write(Serial.read());
}
