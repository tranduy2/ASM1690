  #define BLYNK_TEMPLATE_ID "TMPL6KMup0Jei"
  #define BLYNK_TEMPLATE_NAME "ASM2"

  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>
  #include "DHT.h" 

  #define DHTTYPE DHT11
  #define dht_dpin D2  // Correct pin for DHT11 sensor data
  #define buzzerPin D8 

  const char* ssid = "Faiz";
  const char* pass = "hhhhhhhh";

  // Your Blynk Auth Token
  char auth[] = "kwe550MvbyNdUaZ1_rTtIl3KYvemZhb2";

  // Define the LED pin
  const int ledPin = 13;

  // Create DHT object with the correct pin
  DHT dht(dht_dpin, DHTTYPE);

  SimpleTimer timer;

  int melody[] = {
    262, 262, 294, 262, 349, 330, // Happy Birthday to You
    262, 262, 294, 262, 392, 349, // Happy Birthday to You
    262, 262, 523, 440, 349, 330, 294, // Happy Birthday dear 
    466, 466, 440, 349, 392, 349  // Happy Birthday to You
  };
  int noteDurations[] = {
    4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 2,
    4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 2
  };
  void playHappyBirthday() {
  for (int thisNote = 0; thisNote < 25; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.3;
    delay(pauseBetweenNotes);
    noTone(buzzerPin); // Dừng phát nhạc giữa các nốt
  }
}

    BLYNK_WRITE(V0) {
      int v0_value = param.asInt();
      Serial.print("V0: ");
     Serial.println(v0_value);

     if (v0_value == 1) {
       digitalWrite(ledPin, HIGH);  // Turn LED on
       Serial.println("LED ON");
      } else {
      digitalWrite(ledPin, LOW);  // Turn LED off
      Serial.println("LED OFF");
      }
   }

  void sendUptime() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.print(F("Current humidity = "));
    Serial.print(h);
    Serial.print("%  ");
    Serial.print(F("temperature = "));
    Serial.println(t);
    
    if (t >= 40) {
      playHappyBirthday();  // Nếu nhiệt độ đạt 40°C hoặc cao hơn, phát nhạc
    }
    
    // Send to Blynk virtual pins
    Blynk.virtualWrite(V5, h);
    Blynk.virtualWrite(V6, t);
  }

  void setup() {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);  // loa tắt ban đầu

    Serial.begin(9600);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println(WiFi.localIP());

    Blynk.begin(auth, ssid, pass);
    dht.begin();

    delay(3000);  // Tăng thời gian delay khởi động cảm biến

    timer.setInterval(3000, sendUptime);
  }

  void loop() {
    Blynk.run();
    timer.run();
  }
