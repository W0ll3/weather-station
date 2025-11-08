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

// Sensoren auslesen
void readSensors() {
  sensor_hum_temp.update();
  data.tempRaw = sensor_hum_temp.getTemperature();
  data.humRaw  = sensor_hum_temp.getHumidity();
}

// Rohwerte (ADC)
struct rawDataSensors { float tempRaw; float humRaw; };
rawDataSensors data = {NAN, NAN};


// Sensor(e)
// KY-015 (DHT11) am Digitalpin D2
DHTSensor sensor_hum_temp(2, DHT11);
