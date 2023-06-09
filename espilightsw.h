#pragma once
#ifndef _ESPILIGHTSW_INCLUDE
#define _ESPILIGHTSW_INCLUDE

#include <ESPiLight.h>
#include "esphome.h"

static const char *const TAG = "espilightsw_switch";

/// This class implements calculation of absolute humidity from temperature and relative humidity.
class EspilightComponent : public Switch, public Component
{
public:
  EspilightComponent() = default;

  void set_protocol_name(String protocol_name)
  {
    ESP_LOGD(TAG, "espilight->protocol_name %s ", protocol_name);
    this->protocol_name_ = protocol_name.c_str();
  }
  void set_protocol_data(String protocol_data_on, String protocol_data_off)
  {
    ESP_LOGD(TAG, "espilight->protocol_data_on %s ", protocol_data_on.c_str());
    ESP_LOGD(TAG, "espilight->protocol_data_off %s ", protocol_data_off.c_str());
    this->protocol_data_on_ = protocol_data_on.c_str();
    this->protocol_data_off_ = protocol_data_off.c_str();
  }
  void set_pin(int pin) { pin_ = pin; }

  void dump_config()
  {
    LOG_SWITCH("", "Espilight Switch", this);
  }
  // void loop() override;
  switch_::Switch *source_{nullptr};

  ESPiLight *pespilight{nullptr};

  void setup()
  {
    ESP_LOGCONFIG(TAG, "Espilight '%s'...", this->get_name().c_str());
  }

protected:
  void do_it(int onoff)
  {
    if (pespilight == nullptr)
    {
      ESP_LOGD(TAG, "Create New espilight %i ", pin_);
      pespilight = new ESPiLight(pin_);
    }
    ESP_LOGD(TAG, "onoff: %i ", onoff);
    ESP_LOGD(TAG, "espilight->send_name %s ", this->protocol_name_.c_str());

    if (onoff == 1) {
      ESP_LOGD(TAG, "espilight->send_data %s ", this->protocol_data_on_.c_str());      
      pespilight->send(this->protocol_name_.c_str(), this->protocol_data_on_.c_str());

    } else {
      ESP_LOGD(TAG, "espilight->send_data %s ", this->protocol_data_off_.c_str());      
      pespilight->send(this->protocol_name_.c_str(), this->protocol_data_off_.c_str());  
    }    
  }
  float get_setup_priority() const { return setup_priority::DATA; }

  void write_state(bool state)
  {
    if (state)
    {
      ESP_LOGD(TAG, "Turning ON.");
      do_it(1);
    }
    else
    {
      ESP_LOGD(TAG, "Turning OFF.");
      do_it(0);
    }
    publish_state(state);
  }

  String protocol_name_ = "";
  String protocol_data_on_ = "";
  String protocol_data_off_ = "";

  int pin_ = 0;
};

#endif
