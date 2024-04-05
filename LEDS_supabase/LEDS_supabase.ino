#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>
#include <HTTPClient.h>


#define LED_PIN     4
#define NUM_LEDS    4                                                                                                                                                                                                                                                      

CRGB leds[NUM_LEDS];

CRGB colors[NUM_LEDS];

const char* ssid = "LeMatrix";
const char* password = NULL;

const char* ssidW = "RN8";
const char* passwordW = "123456789";

const String apiURL = "https://xujfzrydvpziizkztbjp.supabase.co/rest/v1";
const String apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inh1amZ6cnlkdnB6aWl6a3p0YmpwIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MTExNDQwNTUsImV4cCI6MjAyNjcyMDA1NX0.ikspwER5xHsaSPIkO67P-XOzCPNjIaLMDa7o5dCa608";

AsyncWebServer server(80);

String htmlForm = "\
<!DOCTYPE html>\
<html>\
<head>\
  <title>Connect to the WiFi</title>\
</head>\
<body style='width: 100vw; height: 100vh; display: flex; flex-direction: column; align-items: center; justify-content: center;'>\
  <h1>Connect to the WiFi (GREEN LEDS == CONNECTED)</h1>\
  <form method='post' action='/connect'>\
    <label for='ssid'>SSID:</label><br>\
    <input type='text' id='ssid' name='ssid'><br>\
    <label for='password'>Password:</label><br>\
    <input type='password' id='password' name='password'><br><br>\
    <input type='submit' value='Connect'>\
  </form>\
</body>\
</html>";

String connected = "\
<!DOCTYPE html>\
<html>\
<head>\
  <title>Connected</title>\
</head>\
<body style='width: 100vw; height: 100vh; display: flex; flex-direction: column; align-items: center; justify-content: center;'>\
  <h1>Board connected successfully</h1>\
  <h2>The access point will turn of in a moment</h2>\
</body>\
</html>";

void setup() {
  for(CRGB color : colors){
    color = CRGB(0,0,0);
  }
  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(6);
  
  Serial.begin(115200);
  pinMode(5, OUTPUT);      // set the LED pin mode
    
  Serial.begin(115200);

  // Initialize ESP32 as Access Point
  /*WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlForm);
  });

  server.on("/connected", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", connected);
    if(WiFi.status() == WL_CONNECTED)
      WiFi.softAPdisconnect(true);
  });

  // Route to handle form submission
  server.on("/connect", HTTP_POST, [](AsyncWebServerRequest *request){
    // Extract SSID and password from form data
    String ssid = request->arg("ssid");
    String password = request->arg("password");

    // Connect to the specified WiFi network
    WiFi.begin(ssid.c_str(), password.c_str());

    int count;
    
    while (WiFi.status() != WL_CONNECTED) {
      if(count >= 5) break;
      delay(1000);
      Serial.println("Connecting...");
      count++;
    }
    
    Serial.println(WiFi.status() == WL_CONNECTED);
    
    request->redirect((WiFi.status() == WL_CONNECTED) ? "/connected" : "/");
  });
  
  // Start server
  server.begin();*/
  Serial.println("dbno: ");

  WiFi.begin("RN8", "123456789");

  while(WiFi.status() != WL_CONNECTED){
    Serial.println("connecting");
    delay(1000);
  }

  Serial.println("connected");
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;

      http.begin(apiURL + "/system");
      http.addHeader("apikey", apiKey);
      http.addHeader("Content-Type", "application/json"); 

      int httpResponseCode = http.GET();

      if(httpResponseCode > 0){
        Serial.printf("[HTTP] GET... code: %d\n", httpResponseCode);

        // Parse the response
        String payload = http.getString();
        Serial.println("Response:");
        Serial.println(payload);
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
      }
      
    http.end(); // Close connection

    for(int led = 0; led < NUM_LEDS; led++){
      leds[led] = CRGB(0,255,0);
      FastLED.show();
    }
  }
  else
    for(int led = 0; led < NUM_LEDS; led++){
      leds[led] = CRGB(255,0,0);
      FastLED.show();
    }
}
