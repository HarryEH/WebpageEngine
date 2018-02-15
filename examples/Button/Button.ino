#include "WebpageEngine.h"
#include <WiFiClient.h>
#include <ESPWebServer.h>
////////////////////////////////////////////////////////////////////////////////
// Shows usage of the utlity that I have written
const char* html;
String SSID = "testserver"
String PASSWORD = "password1"

ESPWebServer server;

void generateWebPage()
{
  WebPage test("COM3505");

  // CSS attributes
  attribute cssAttributes[3];
  cssAttributes[0] = test.attributeCreator("background", "#ffb6c1");
  cssAttributes[1] = test.attributeCreator("font-family", "Arial, Helvetica, Sans-Serif");
  cssAttributes[2] = test.attributeCreator("Color", "#c1ffb6");
  test.insertCss("body", cssAttributes, 3);

  test.insertHtmlElement(BODY, test.htmlElementCreator("h1", NULL, 0, "Press My Buttons", NULL, 0));

  // LED ON BUTTON
  attribute hrefOn = test.attributeCreator("href", "/ledOn");
  element buttonOn = test.htmlElementCreator("button", NULL, 0, "Led On", NULL, 0);
  element ledOn = test.htmlElementCreator("a", &hrefOn, 1, "", &buttonOn, 1);
  test.insertHtmlElement(BODY, ledOn);

  // LED OFF BUTTON
  attribute hrefOff = test.attributeCreator("href", "/ledOff");
  element buttonOff = test.htmlElementCreator("button", NULL, 0, "Led Off", NULL, 0);
  element ledOff = test.htmlElementCreator("a", &hrefOff, 1, "", &buttonOff, 1);
  test.insertHtmlElement(BODY, ledOff);

  html = test.renderPage().c_str();
 }
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function for when 404 errors
void error404()
{
  char *message = "404 Error - File not found\n\n";
  server.send (404, "text/plain", message);
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Setup
void setup()
{
  pinMode ( LED_BUILTIN, OUTPUT );
  Serial.begin ( 115200 );

  // Access Point code
  WiFi.mode(WIFI_AP_STA);// Access point station

  Serial.println("Creating wifi access point");
  WiFi.softAP(SSID,PASSWORD);
  Serial.print("IP address to connect to: ");
  Serial.println(WiFi.softAPIP());

  // Generate the webpage
  generateWebPage();
  // Inline page serving

  // Serve the index page inline
  server.on ( "/", []() {
    server.send ( 200, "text/html", html );
  });

  // Deal with LED on inline
  server.on ( "/ledOn", []() {
    server.send ( 200, "text/html", html );
    digitalWrite(LED_BUILTIN, HIGH);
  } );

  // Deal with LED off inline
  server.on ( "/ledOff", []() {
    server.send ( 200, "text/html", html );
    digitalWrite(LED_BUILTIN, LOW);
  } );

  server.onNotFound ( error404 );// Deal with 404 Errors
  server.begin(); // Trigger the server! Important step

}// End of setup
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Loop
void loop()
{
  server.handleClient();
}// End of loop
////////////////////////////////////////////////////////////////////////////////
