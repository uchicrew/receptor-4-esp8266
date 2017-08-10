#include <ESP8266WiFi.h>


/* ========== wifi ========== */

// Your WiFi SSID.
const char* SSID = "**YOUR SSID**";

// Your WiFi password.
const char* PASSWORD = "**YOUR PASSWORD**";

// Set to true to assign a fixed IP address.
const bool STATIC_IP = true;

const IPAddress IP = IPAddress(192,168,1,18);
const IPAddress GATEWAY = IPAddress(192,168,1,1);
const IPAddress SUBNET = IPAddress(255,255,255,0);
const IPAddress DNS = IPAddress(192,168,1,1);


/* ========== service ========== */

// Copy & paste your collection Endpoint URL.
//
// Feel free to register and customize the collection.
// http://receptor-4.appspot.com
//
const String ENDPOINT_URL = "http://receptor-4.appspot.com/api/transmitter/w5l6W0RHe0PLJZFkURC4Kkpuguh2:9a833905861545d79fe9d76ec0722d72";

// Sets the collection synchronization interval in milis.
// Please specify more than 1 minute (60000), because it is heavily cached on the remote server.
const int UPDATE_INTERVAL_MS = 180000;


/* ========== ui ========== */

// Set it to true to flip the screen vertically.
const bool FLIP_SCREEN_VERTICALLY = true;
