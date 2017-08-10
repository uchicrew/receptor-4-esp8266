#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "service.h"

Service::Service(String url) {
  this->url = url;
}

Service::~Service() {
  if (this->items != NULL) {
    delete[] this->items;
  }
}

bool Service::handleResponse(HTTPClient& http) {
  // parse json
  DynamicJsonBuffer jsonBuffer(512);
  JsonObject& root = jsonBuffer.parse(http.getStream());
  if (!root.success() && root["result"] != true) {
    this->status = SERVICE_PARSE_ERROR;
    return false;
  }

  JsonArray& items = root["data"];
  int size = items.size();
  if (size > MAX_ITEM) size = MAX_ITEM;

  if (size != this->itemSize) {
    // re-create
    if (this->items) delete[] this->items;
    this->items = new Card[size];
    for (int i=0; i<size; i++) {
      this->items[i] = {};
    }
    this->itemSize = size;
  }

  for (int i=0; i<size; i++) {
    this->items[i].label = items[i]["label"].as<String>();
    this->items[i].value = items[i]["value"].as<String>();
    this->items[i].summary = items[i]["summary"].as<String>();
  }

  this->status = SERVICE_OK;
  return true;
}

bool Service::fetch() {
  bool ret;
  HTTPClient http;
  http.begin(this->url);
  this->httpCode = http.GET();
  if (this->httpCode >= 200 && this->httpCode < 300) {
    ret = this->handleResponse(http);
  } else {
    this->status = SERVICE_HTTP_ERROR;
    ret = false;
  }
  http.end();
  return ret;
}

int Service::getSize() {
  return this->itemSize;
}

int Service::getMaxSize() {
  return MAX_ITEM;
}

Card Service::getItem(int idx) {
  if (this->itemSize == 0) {
    Card card = {"No data", "", ""};
    return card;
  }
  return this->items[idx % this->itemSize];
}

ServiceStatus Service::getStatus() {
  return this->status;
}

String Service::getStatusMessage() {
  switch (this->status) {
    case SERVICE_UNAVAILABLE:
      return "Service Unavailable";
    case SERVICE_HTTP_ERROR:
      return "Remote service returned error code.";
    case SERVICE_PARSE_ERROR:
      return "Remote service returned invalid data.";
    // case SERVICE_OK:
    default:
      return "OK";
  }
}

int Service::getLastHttpCode() {
  return this->httpCode;
}
