const int buzzer = 8;

String cmd = "";

void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      process(cmd);
      cmd = "";
    } else if (c != '\r') {
      cmd += c;
    }
  }
}

void process(String s) {

  if (s.length() == 0) return;

  if (s[0] == 'd') {
    int ms = s.substring(1).toInt();
    delay(ms);
    return;
  }

  int freq = s.toInt();

  if (freq <= 0)
    noTone(buzzer);
  else
    tone(buzzer, freq);
}