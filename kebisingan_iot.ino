#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// konek ke wifi
const char* ssid = "Kost_Kusuma-4G";
const char* password = "kusuma13"; 

// Konfigurasi Server
const char* serverName = "http://192.168.1.100/monitoring/insert.php";

// konfigurasi semua pin
const int soundSensorPin = A0;  
const int ledMerah = D1;       
const int ledKuning = D2;       
const int ledHijau = D3;       


const int thresholdRendah = 300;  // < 300 level suara rendah
const int thresholdSedang = 600;  // > 300 sedang, > 600 tinggi 

void setup() {
  Serial.begin(115200);
  
  // led output
  pinMode(ledMerah, OUTPUT);
  pinMode(ledKuning, OUTPUT);
  pinMode(ledHijau, OUTPUT);
  
  // matikan led di awal
  digitalWrite(ledMerah, LOW);
  digitalWrite(ledKuning, LOW);
  digitalWrite(ledHijau, LOW);
  
  // konek ke wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // baca nilai sensor
  int soundLevel = analogRead(soundSensorPin);
  
  String soundStatus;
  int lampId;
  
  // menentukan status level suara
  if (soundLevel < thresholdRendah) {
    // Level RENDAH - Hijau 
    soundStatus = "RENDAH";
    lampId = 3;
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledKuning, LOW);
    digitalWrite(ledHijau, HIGH);
  } 
  else if (soundLevel >= thresholdRendah && soundLevel < thresholdSedang) {
    // Level SEDANG - Kuning
    soundStatus = "SEDANG";
    lampId = 2;
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledKuning, HIGH);
    digitalWrite(ledHijau, LOW);
  } 
  else {
    // Level TINGGI - Merah
    soundStatus = "TINGGI";
    lampId = 1;
    digitalWrite(ledMerah, HIGH);
    digitalWrite(ledKuning, LOW);
    digitalWrite(ledHijau, LOW);
  }
  
  // tampilan output di serial monitor
  Serial.print("Sound Level: ");
  Serial.print(soundLevel);
  Serial.print(" | Status: ");
  Serial.print(soundStatus);
  Serial.print(" | Lamp ID: ");
  Serial.println(lampId);
  
  // kirim data
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    
    // url dengan parameter
    String url = String(serverName) + "?level=" + String(soundLevel) + 
                 "&status=" + soundStatus + "&lamp=" + String(lampId);
    
    Serial.print("Sending to: ");
    Serial.println(url);
    
    http.begin(client, url);
    http.setTimeout(5000);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Response: ");
      Serial.println(response);
      Serial.println("Data sent successfully!");
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      Serial.println("Cek: 1) XAMPP nyala? 2) IP address bener? 3) Firewall?");
    }
    
    http.end();
  } else {
    Serial.println("WiFi Disconnected!");
  }
  
  delay(2000);  // delay per 2 detik
}