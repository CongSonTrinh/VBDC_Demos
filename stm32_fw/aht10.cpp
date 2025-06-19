#include "aht10.hpp"

#include <Adafruit_AHTX0.h>

class CustomAhtSensor : public Adafruit_AHTX0 {
public:
  CustomAhtSensor() {
    _humidity = 0.0f;
    _temperature = 0.0f;
  }

  enum DataEvent {
    START_CMD_TRIGGER = 0,
    WAIT_FOR_DATA_READY, 
    READY_FOR_READ,
  };

  void onNewData(void)
  {
    uint8_t data[6];
    uint32_t h, tdata;

    switch(e_state_) {
      case START_CMD_TRIGGER:
        t_trigger_ = millis();
        if (i2c_dev->write(TRIGGER_CMD, 3)) {
          e_state_ = WAIT_FOR_DATA_READY;
        }
        data_ready_ = false;
        break;

      case WAIT_FOR_DATA_READY:
        if (getStatus() & AHTX0_STATUS_BUSY) {
          // Do nothing
        } else {
          e_state_ = READY_FOR_READ;
        }
        data_ready_ = false;
        break;

      case READY_FOR_READ:
        if (i2c_dev->read(data, 6)) {
          h = data[1];
          h <<= 8;
          h |= data[2];
          h <<= 4;
          h |= data[3] >> 4;
          _humidity = ((float)h * 100) / 0x100000;

          tdata = data[3] & 0x0F;
          tdata <<= 8;
          tdata |= data[4];
          tdata <<= 8;
          tdata |= data[5];
          _temperature = ((float)tdata * 200 / 0x100000) - 50;

          data_ready_ = true;
        }
        e_state_ = START_CMD_TRIGGER;
        break;

      default:
        break;
    }
  }

  bool hasNewData() { return data_ready_; }
  float temperature() { return _temperature; }
  float humidity() { return _humidity; }

private:
  bool data_ready_;
  DataEvent e_state_;
  uint64_t t_trigger_;
  static constexpr uint8_t TRIGGER_CMD[3] = { AHTX0_CMD_TRIGGER, 0x33, 0 };
};

// static constexpr CustomAhtSensor::TRIGGER_CMD[3] = { AHTX0_CMD_TRIGGER, 0x33, 0 };
static CustomAhtSensor aht;
static float temp, humidity;

void initAHT10(TwoWire *wire)
{
  aht.begin(wire);
}

void updateAHT10(void)
{
  aht.onNewData();
}

float tempAHT10(void)
{
  return aht.temperature();
}

float humidityAHT10(void)
{
  return aht.humidity();
}

bool hasNewDataAHT10(void)
{
  return aht.hasNewData();
}
