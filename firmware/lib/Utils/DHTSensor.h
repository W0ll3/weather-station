#include <DHT.h>

class DHTSensor { 
  private: 
  uint8_t pin; 
  uint8_t type; 
  DHT dht;
  float temperature = 0;
  float humidity = 0;
  //      Objekt        Parameter von außen        Übergabe der Werte und die Ini
  public: 
  explicit DHTSensor(uint8_t pin, uint8_t type) : pin(pin), type(type), dht(pin, type) {}
  
  void begin() { 
    dht.begin(); 
  } 
  void update() { // Hier liest du BEIDE Werte aus
    temperature = dht.readTemperature(); 
    humidity = dht.readHumidity(); 
    } 
  float getTemperature() const { return temperature; } 
  float getHumidity() const { return humidity; }
};