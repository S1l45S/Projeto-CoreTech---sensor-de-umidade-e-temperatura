#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <time.h>
#include "web.h"

#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -3 * 3600;
const int   daylightOffset_sec = 0;

unsigned long lastRead = 0;
const unsigned long interval = 2000;

float temperatura = 0;
float umidade = 0;

String csvData = "Hora;Temperatura;Umidade\n";


String getDateTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "0000-00-00 00:00:00";
  }

  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

 
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Wi-Fi conectado com sucesso!");
  Serial.print("IP do servidor: ");
  Serial.println(WiFi.localIP());
  Serial.println("Acesse no navegador: http://localhost:8280");

  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/variaveis", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"t\":" + String(temperatura, 2) + ",";
    json += "\"u\":" + String(umidade, 2);
    json += "}";
    request->send(200, "application/json", json);
  });

  server.on("/dados", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/csv", csvData);
  });

  server.begin();
  Serial.println("Servidor Async HTTP iniciado na porta 80");
}

void loop() {
  unsigned long now = millis();

  if (now - lastRead >= interval) {
    lastRead = now;

    float t = dht.readTemperature();
    float u = dht.readHumidity();

    if (!isnan(t) && !isnan(u)) {
      temperatura = t;
      umidade = u;

      // Data e hora
      String dt = getDateTime(); // YYYY-MM-DD,HH:MM:SS

      // Salva no CSV
      csvData += dt + ";" +
                 String(temperatura, 2) + ";" +
                 String(umidade, 2) + "\n";
    }
  }
}
