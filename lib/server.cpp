const char *ssid = "Tesla IoT";
const char *password = "fsL6HgjN";
const int ACMStatusPin = 14;
bool ACMStatus = false;

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void setupServer()
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

    // Route for handling the GET request to turn ACMStatusPin ON
    server.on("/ACMStatusPin/on", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    digitalWrite(ACMStatusPin, HIGH);
    ACMStatus = true;

    request->send(200, "text/plain", "ACMStatusPin turned ON"); });

    // Route for handling the GET request to turn ACMStatusPin OFF
    server.on("/ACMStatusPin/off", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    digitalWrite(ACMStatusPin, LOW);
    ACMStatus =false;

    request->send(200, "text/plain", "ACMStatusPin turned OFF"); });

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
    html += "<script>";
    html += "document.getElementById('onButton').addEventListener('click', function() {";
    html += "fetch('/ACMStatusPin/on');";
    html += "});";
    html += "document.getElementById('offButton').addEventListener('click', function() {";
    html += "fetch('/ACMStatusPin/off');";
    html += "});";
    html += "</script>";

    html += "</body></html>";

    request->send(200, "text/html", html); });

    // Catch-all handler for not found routes
    server.onNotFound(notFound);

    server.begin();
}