#include <WiFi.h>
#include <WebServer.h>
#include "ESPAsyncWebServer.h"
#include <Wire.h>
#include <NTPClient.h> /* https://github.com/arduino-libraries/NTPClient */

AsyncWebServer sv(80);

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

const int sensor = 34;
const int relay1 = 14;
int _moisture;

// WiFi - Coloque aqui suas configurações de WI-FI
const char* ssid = "SEU_SSID";
const char* senha = "SUA_SENHA";
//DEFINIÇÃO DE IP FIXO PARA O NODEMCU
IPAddress staticIP();  // COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR
IPAddress gateway();     // GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR)
IPAddress subnet();    // MÁSCARA DE REDE

WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000); /* Cria um objeto "NTP" com as configurações utilizada no Brasil */
String hora;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>HORTA INTELIGENTE</h2>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
  <p>
    <span class="dht-labels">Hora: </span>
    <span id="hora">%TIME%</span>
  </p>
</body>
<script>
setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000);

setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("hora").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/hora", true);
  xhttp.send();
}, 1000);
</script>
</html>)rawliteral";

String processor(const String& var) {
  if (var == "HUMIDITY") {
    return (String)_moisture;
  }
  if (var == "TIME") {
    return ntp.getFormattedTime();
  }
  return String();
}

void setup() {
  Serial.begin(9600);
  pinMode(relay1, OUTPUT);
  delay(500);
  Serial.print("Se conectando a: ");
  Serial.println(ssid);

  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(ssid, senha);  // Se conecta ao Wi-Fi

  while (WiFi.status() != WL_CONNECTED) {  // Verifica se a conexão foi bem-sucedida
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  sv.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  sv.on("/humidity", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", (String)_moisture);
  });
  sv.on("/hora", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", ntp.getFormattedTime().c_str());
  });

  sv.begin();  // Inicia o servidor
  Serial.println("Servidor Online");
  delay(2000);
  ntp.begin();  // Inicia o protocolo
  ntp.forceUpdate();
}

void loop() {
  int sensor_analog = analogRead(sensor);
  Serial.print("Sensor = " + (String)sensor_analog);

  _moisture = (100 - ((sensor_analog / 4095.00) * 100));
  Serial.print("\nMoisture = " + (String)_moisture + "%");

  if (_moisture < 55) {
    digitalWrite(relay1, LOW);  // Por algum motivo, o relay estava funcionando ao contrário, com LOW ligando e HIGH desligando
  } else {
    digitalWrite(relay1, HIGH);
  }

  delay(1000);

  if ((millis() - lastTime) > timerDelay) {
    lastTime = millis();
    ntp.forceUpdate();  // Atualiza a hora
  }
}
