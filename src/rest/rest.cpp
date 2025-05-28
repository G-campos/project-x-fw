#include "rest.h"

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include "wifi/wifi.h"
#include "storage/storage.h"


AsyncWebServer server(80);

// // Página HTML do WiFi Manager
// const char page_html[] PROGMEM = R"rawliteral(
//     <!DOCTYPE html>
//     <html lang="pt">

//     <head>
//         <meta charset="UTF-8">
//         <meta name="viewport" content="width=device-width, initial-scale=1.0">
//         <title>WiFi Manager</title>
//         <style>
//             body {
//                 color: #434343;
//                 font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
//                 font-size: 14px;
//                 background-color: #eeeeee;
//                 margin-top: 100px;
//             }

//             .container {
//                 margin: 0 auto;
//                 max-width: 400px;
//                 padding: 30px;
//                 box-shadow: 0 10px 20px rgba(0, 0, 0, 0.19), 0 6px 6px rgba(0, 0, 0, 0.23);
//                 background-color: #ffffff;
//                 border-radius: 10px;
//             }

//             h2 {
//                 text-align: center;
//                 margin-bottom: 20px;
//                 margin-top: 0px;
//                 color: #0ee6b1;
//                 font-size: 35px;
//             }

//             #titleGreen {
//                 color: #00E1AA;
//             }

//             #titleBlack {
//                 color: #000000;
//             }

//             h3 {
//                 text-align: center;
//                 margin-bottom: 40px;
//                 margin-top: 0px;
//                 color: #336859;
//                 font-size: 35px;
//             }

//             form .field-group {
//                 box-sizing: border-box;
//                 clear: both;
//                 padding: 4px 0;
//                 position: relative;
//                 margin: 1px 0;
//                 width: 100%;
//             }

//             .text-field {
//                 font-size: 15px;
//                 margin-bottom: 4%;
//                 -webkit-appearance: none;
//                 display: block;
//                 background: #fafafa;
//                 color: #636363;
//                 width: 100%;
//                 padding: 15px 0px 15px 0px;
//                 text-indent: 10px;
//                 border-radius: 5px;
//                 border: 1px solid #e6e6e6;
//                 background-color: transparent;
//             }

//             .text-field:focus {
//                 border-color: #00bcd4;
//                 outline: 0;
//             }

//             .button-container {
//                 box-sizing: border-box;
//                 clear: both;
//                 margin: 1px 0 0;
//                 padding: 4px 0;
//                 position: relative;
//                 width: 100%;
//             }

//             .button {
//                 background: #00E1AA;
//                 border: none;
//                 border-radius: 5px;
//                 color: #ffffff;
//                 cursor: pointer;
//                 display: block;
//                 font-weight: bold;
//                 font-size: 16px;
//                 padding: 15px 0;
//                 text-align: center;
//                 text-transform: uppercase;
//                 width: 100%;
//                 -webkit-transition: background 250ms ease;
//                 -moz-transition: background 250ms ease;
//                 -o-transition: background 250ms ease;
//                 transition: background 250ms ease;
//             }

//             p {
//                 text-align: center;
//                 text-decoration: none;
//                 color: #87c1d3;
//                 font-size: 18px;
//             }

//             a {
//                 text-decoration: none;
//                 color: #ffffff;
//                 margin-top: 0%;
//             }

//             #status {
//                 text-align: center;
//                 text-decoration: none;
//                 color: #336859;
//                 font-size: 14px;
//             }
//         </style>
//     </head>

//     <body onload="getWifiScanResult()">
//         <div class="container">
//             <h2>Configuração WiFi</h2>
//             <div name="myForm">
//                 <div class="field-group">
//                 <select class='text-field' name='ssid'></select>
//                 </div>
//                 <br>
//                 <div class="field-group">
//                 <input class="text-field" type="password" name="password" length=64 placeholder="Password">
//                 </div>
//                 <br>
//                 <div id="statusDiv">
//                 <br><br>
//                 </div>
//                 <div class="button-container">
//                     <button class="button" onclick="validateForm()">Conectar</button>
//                 </div>
//             </div>
//         </div>
//         <script>
//             async function sendConfig() {
//                 let ssid = document.getElementById('ssid').value;
//                 let password = document.getElementById('password').value;
//                 await fetch('/config', {
//                     method: 'POST',
//                     headers: { 'Content-Type': 'application/json' },
//                     body: JSON.stringify({ ssid, password })
//                 })
//                 .then(response => response.json())
//                 .then(data => document.getElementById('statusDiv').innerText = "<p id='status'>" + (data.status || data.error) + "</p>")
//                 .catch(error => console.error('Erro:', error));
//             }

//             function validateForm() {
//                 let ssid = document.getElementById('ssid').value;
//                 let password = document.getElementById('password').value;
//                 const status = document.getElementById("statusDiv");
//                 if (ssid == "" && password == "") {
//                     status.innerHTML = "<p id='status' style='color:red;'>Insira SSID e senha.</p>";
//                     return false;
//                 }
//                 else if (ssid == "") {
//                     status.innerHTML = "<p id='status' style='color:red;'>Insira SSID.</p>";
//                     return false;
//                 }
//                 else if (password == "") {
//                     status.innerHTML = "<p id='status' style='color:red;'>Insira senha.</p>";
//                     return false;
//                 }
//                 else {
//                     status.innerHTML = "<p id='status'>Conectando...</p>";
//                     sendConfig()
//                     return true;
//                 }
//             }

//             async function getWifiScanResult() {
//                 await fetch('/scan', {
//                     method: 'GET',
//                     headers: { 'Content-Type': 'application/json' },
//                 })
//                 .then(response => response.json())
//                 .then((data) => {
//                     let selectTag = document.getElementsByName('ssid')
//                     if (data.leght) {
//                         data.forEach(element => {
//                             selectTag.innerHTML += `<option value='${element.ssid}'>${element.ssid}</option>`
//                         });
//                     } else {
//                         selectTag.innerHTML = "<option value='' disabled selected>Nenhuma rede encontrada</option>"
//                     }
                    
//                 })
//                 .catch((error) => {
//                     console.error('Erro:', error)
//                     document.getElementById("statusDiv").innerHTML = "<p id='status'>Erro ao buscar redes de wifi</p>"
//                 });
//             }
//         </script>
//     </body>

//     </html>
//     )rawliteral";

void handleRoot(AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", String(), false);
}

void handleWifiConfig(AsyncWebServerRequest *request) {
    DynamicJsonDocument doc(256);
    deserializeJson(doc, request->arg("plain"));

    String ssid = doc["ssid"];
    String password = doc["password"];

    if (ssid.length() > 0 && password.length() > 0) {
        request->send(200, "application/json", "{\"status\":\"Conectando...\"}");
        wifi_connect(ssid.c_str(), password.c_str());
    } else {
        request->send(400, "application/json", "{\"error\":\"SSID e senha obrigatórios\"}");
    }
}

void handleWifiScanned(AsyncWebServerRequest *request) {
    DynamicJsonDocument doc = list_wifi_ssid();

    if (doc.size() > 0) {
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    } else {
        request->send(400, "application/json", "{\"error\":\"Nenhuma rede encontrada\"}");
    }
}

void handleNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void rest_router() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/config", HTTP_POST, handleWifiConfig);
    server.on("/scan", HTTP_GET, handleWifiScanned);
    server.onNotFound(handleNotFound);
}

void rest_init() {
    rest_router();
    server.begin();
    Serial.println("Servidor REST iniciado.");
}
