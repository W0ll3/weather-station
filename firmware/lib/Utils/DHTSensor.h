#include <DHT.h>

class DHTSensor { 
  private: 
  uint8_t pin; 
  uint8_t type; 
  DHT dht;
  float temperature;
  float humidity;
  //      Objekt          Parameter von außen                    Übergabe der Werte und die Ini
  public: DHTSensor(uint8_t pin, uint8_t type = DHT11) : pin(pin), type(type), dht(pin, type), temperature(0), humidity(0) {}
  
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