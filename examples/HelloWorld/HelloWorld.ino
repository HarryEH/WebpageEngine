#include "WebpageEngine.h"
#include <WiFiClient.h>
#include <ESPWebServer.h>

String SSID = "testserver"
String PASSWORD = "password1"

ESPWebServer server;

////////////////////////////////////////////////////////////////////////////////
// Shows usage of the utlity that I have written
WebPage test("Hello World");

void generateWebPage()
{
  test.insertHtmlElement(BODY,
      test.htmlElementCreator("h1", NULL, 0, "Hello World!", NULL, 0));
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
    server.send ( 200, "text/html", test.renderPage() );
  });

  server.onNotFound ( error404 );// Deal with 404 Errors
  server.begin(); // Trigger the server! Important step

}// End of setup function
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Loop
void loop()
{
  server.handleClient();
}// End of loop
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function for when 404 errors
void error404()
{
  char *message = "404 Error - File not found\n\n";
  server.send (404, "text/plain", message);
}
////////////////////////////////////////////////////////////////////////////////
