// To run this example: python shammam.py -s <this file>
// You can interact with the serial using netcat: nc localhost 9911 (by default)

void setup() {
    Serial.begin(9600);
}

void loop() {
    while (Serial.available())
        Serial.write(Serial.read());
}
