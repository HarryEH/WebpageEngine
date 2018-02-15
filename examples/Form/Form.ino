#include "WebpageEngine.h"
#include <WiFiClient.h>
#include <ESPWebServer.h>

String dynamicSsid = "";
String dynamicPassword = "";

String SSID = "testserver"
String PASSWORD = "password1"

ESPWebServer server;

////////////////////////////////////////////////////////////////////////////////
// Setup test consts;
// Create the page
WebPage test("COM3505 Wifi List");
attribute cssAttributes[3];
attribute formAttributes[2];
attribute selectAttributes[1];
attribute passAttributes[3];
attribute inputAttributes[2];
element formElements[3];

////////////////////////////////////////////////////////////////////////////////
// Setup
void setupNine()
{

  Serial.begin ( 115200 );
  pinMode ( LED_BUILTIN, OUTPUT );

  Serial.println("Starting setup...");

  // Access Point code
  WiFi.mode(WIFI_AP_STA);// Access point station

  Serial.println("Creating wifi access point");
  WiFi.softAP(SSID,PASSWORD);
  Serial.print("IP address to connect to: ");
  Serial.println(WiFi.softAPIP());

  setupWebPage();// Call function to setup webpage

  // Serve the wifi list page
  server.on ( "/", wifiListPage);

  // Deal with handling the post request
  server.on ( "/wificonnect", handlePostRequest);

  server.onNotFound ( error404 );// Deal with 404 Errors
  server.begin(); // Trigger the server! Important step

}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Loop
void loop()
{
  server.handleClient();
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Shows usage of the utlity that I have written - wifi connection
void wifiListPage()
{
  unsigned long t1 = millis();
  int n = WiFi.scanNetworks();
  Serial.print("Network scan took: ");
  Serial.print((millis()-t1));
  Serial.println("ms");

  if (n == 0)
  {
    test.insertHtmlElement(BODY,
        test.htmlElementCreator("form", NULL, 0, "no networks found", formElements, 3));
  }
  else
  {

    for (int i = 0; i < n; i++)
    {
      wifiElements[i] = test.htmlElementCreator("option",
                        NULL, 0, WiFi.SSID(i), NULL, 0);
    }

    formElements[0] = test.htmlElementCreator("select", selectAttributes, 1, "", wifiElements, number);
    // add to form html element
    test.insertHtmlElement(BODY,
      test.htmlElementCreator("form", formAttributes, 2, "", formElements, 3));
  }

  server.send ( 200, "text/html", test.renderPage() );

  test.removeHtmlElement(BODY, "form", "", 2, 3);
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Handle post request
void handlePostRequest() {

 // Extract the arguments from the request
	for ( int i = 0; i < server.args(); i++ )
 {

   Serial.print(server.argName(i));
   Serial.print(" : ");
   Serial.println(server.arg(i));

   if( (server.argName(i)).equals("password") )
   {
     dynamicPassword = server.arg (i);
   }

   if( (server.argName(i)).equals("ssid") )
   {
     dynamicSsid = server.arg (i);
   }

	}

 int test = connectToWifi(dynamicSsid.c_str(), dynamicPassword.c_str());

 if (test == 1 ) // Success case
 {
   server.send ( 200, "text/plain", "Connected Successfully");
 }
 else if (test == 0) // Failure Case
 {
   server.send ( 200, "text/plain", "Failed To Connect. Go back and try again!");
 }
 else // Already connected to wifi case
 {
   String str = "Already Connected to Wifi called ";
   str.concat(WiFi.SSID());
   server.send ( 200, "text/plain", str);
 }

}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Does all the basic setting up of the webpage
void setupWebPage()
{

 test.insertHtmlElement(BODY, test.htmlElementCreator("h1", NULL, 0, "Connect to WiFi", NULL, 0));
 // CSS attributes
 cssAttributes[0] = test.attributeCreator("background", "#ffb6c1");
 cssAttributes[1] = test.attributeCreator("font-family", "Arial, Helvetica, Sans-Serif");
 cssAttributes[2] = test.attributeCreator("Color", "#c1ffb6");
 test.insertCss("body", cssAttributes, 3); // Insertion of the css

 // HTML Attributes
 formAttributes[0] = test.attributeCreator("action", "/wificonnect");
 formAttributes[1] = test.attributeCreator("method", "post");

 selectAttributes[0] = test.attributeCreator("name", "ssid");

 passAttributes[0] = test.attributeCreator("id", "password");
 passAttributes[1] = test.attributeCreator("type", "password");
 passAttributes[2] = test.attributeCreator("name", "password");

 inputAttributes[0] = test.attributeCreator("type", "submit");
 inputAttributes[1] = test.attributeCreator("value", "Connect");

 // HTML Elements
 formElements[1] = test.htmlElementCreator("input", passAttributes, 3, "", NULL, 0);
 formElements[2] = test.htmlElementCreator("input", inputAttributes, 2, "", NULL, 0);

}
////////////////////////////////////////////////////////////////////////////////
