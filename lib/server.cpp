const char *ssid = "Tesla IoT";
const char *password = "fsL6HgjN";
const int ACMStatusPin = 14;
bool ACMStatus = false;

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void setupServer(int leftFrontIR, int rightFrontIR, int leftBackIR, int rightBackIR, int magSensor, bool ACMStatus)
{
    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Route for handling the GET request to retrieve sensor values
    server.on("/sensorValues", HTTP_GET, [leftFrontIR, rightFrontIR, leftBackIR, rightBackIR, magSensor, ACMStatus](AsyncWebServerRequest *request)
              {
        String json = "{";
        json += "\"leftFrontIR\":" + String(digitalRead(leftFrontIR)) + ",";
        json += "\"rightFrontIR\":" + String(digitalRead(rightFrontIR)) + ",";
        json += "\"leftBackIR\":" + String(digitalRead(leftBackIR)) + ",";
        json += "\"rightBackIR\":" + String(digitalRead(rightBackIR)) + ",";
        json += "\"ACMStatus\":" + String(ACMStatus) + ",";
        json += "\"magSensor\":" + String(digitalRead(magSensor));
        json += "}";
        request->send(200, "application/json", json); });

    // Route for handling the root GET request
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  String html = "<!DOCTYPE html><html>";
                  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
                  html += "<link rel=\"icon\" href=\"data:,\">";
                  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
                  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;";
                  html += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
                  html += ".button2 {background-color: #555555;}</style></head>";

                  html += "<body><h1>ESP32 Web Server</h1>";

                  html += "<p>ACMStatusPin - State ";
                  html += digitalRead(ACMStatusPin) ? "ON" : "OFF";
                  html += "</p>";

                  html += "<p><button id=\"onButton\" class=\"button\">ON</button></p>";
                  html += "<p><button id=\"offButton\" class=\"button button2\">OFF</button></p>";

                  // Add new HTML elements to display sensor values
                  html += "<h2>Sensor Values</h2>";
                  html += "<p>Left Front IR: <span id=\"leftFrontIR\"></span></p>";
                  html += "<p>Right Front IR: <span id=\"rightFrontIR\"></span></p>";
                  html += "<p>Left Back IR: <span id=\"leftBackIR\"></span></p>";
                  html += "<p>Right Back IR: <span id=\"rightBackIR\"></span></p>";
                  html += "<p>ACM Status: <span id=\"ACMStatus\"></span></p>";
                  html += "<p>Magnetic Sensor: <span id=\"magSensor\"></span></p>";

                  // Add JavaScript code to update sensor values and handle button clicks
                  html += "<script>document.getElementById(\"onButton\").addEventListener(\"click\", function() {";
                  html += "fetch(\"/turnACMOn\"); });";
                  html += "document.getElementById(\"offButton\").addEventListener(\"click\", function() {";
                  html += "fetch(\"/turnACMOff\"); });";
                  html += "function updateSensorValues() {";
                  html += "fetch(\"/sensorValues\").then(function(response) {";
                  html += "return response.json(); }).then(function(json) {";
                  html += "document.getElementById(\"leftFrontIR\").innerText = json.leftFrontIR;";
                  html += "document.getElementById(\"rightFrontIR\").innerText = json.rightFrontIR;";
                  html += "document.getElementById(\"leftBackIR\").innerText = json.leftBackIR;";
                  html += "document.getElementById(\"rightBackIR\").innerText = json.rightBackIR;";
                  html += "document.getElementById(\"ACMStatus\").innerText = json.ACMStatus;";
                  html += "document.getElementById(\"magSensor\").innerText = json.magSensor;";
                  html += "}); }";
                  html += "setInterval(updateSensorValues, 10);</script>";

                  html += "</body></html>";
                  request->send(200, "text/html", html); });

    // Route for handling the ON button request
    server.on("/turnACMOn", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  digitalWrite(ACMStatusPin, HIGH);
                  request->send(200, "text/plain", "ACM turned ON"); });

    // Route for handling the OFF button request
    server.on("/turnACMOff", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  digitalWrite(ACMStatusPin, LOW);
                  request->send(200, "text/plain", "ACM turned OFF"); });

    // Route for handling not found request
    server.onNotFound(notFound);

    // Start server
    server.begin();
}
