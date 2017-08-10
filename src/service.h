#include <ESP8266HTTPClient.h>

const int MAX_ITEM = 10;

typedef struct {
  String label;
  String value;
  String summary;
} Card;

enum ServiceStatus {
  SERVICE_OK,
  SERVICE_UNAVAILABLE,
  SERVICE_HTTP_ERROR,
  SERVICE_PARSE_ERROR
};

class Service {

  private:
    String url;
    ServiceStatus status;
    int httpCode;
    int itemSize;
    Card *items;
    bool handleResponse(HTTPClient& http);

  public:
    Service(String url);
    ~Service();
    bool fetch();
    int getSize();
    int getMaxSize();
    Card getItem(int idx);
    ServiceStatus getStatus();
    String getStatusMessage();
    int getLastHttpCode();
};
