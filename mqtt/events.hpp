#pragma once
#include <vector>
#include <algorithm>
#include <ostream>

struct Values{
    // Getters
    int getTime() const {
        return time;
    }

    float getTemperature() const {
        return temperature;
    }

    int getElPrice() const {
        return elPrice;
    }

    // Setters
    bool setTime(int newTime) {
      if (time != newTime) {
        time = newTime;
        return true;
      }
      return false;
    }

    bool setTemperature(float newTemperature) {
      if (temperature != newTemperature) {
        temperature = newTemperature;
        return true;
      }
      return false;
    }

    bool setElPrice(int newElPrice) {
      if (elPrice != newElPrice) {
        elPrice = newElPrice;
        return true;
      }
      return false;
    }

    friend std::ostream& operator<<(std::ostream& os, const Values& values) {
      os << "Time: " << values.getTime() << std::endl;
      os << "Temperature: " << values.getTemperature() << std::endl;
      os << "Electricity Price: " << values.getElPrice() << std::endl;
      return os;
    }

private:
    int time;
    float temperature;
    int elPrice;
};


struct Observer {
    virtual void onChange(const Values& state) = 0;
};


struct Observable {
  void notify(const Values& state) {
    for (auto observer : observers)
      observer->onChange(state);
  }

  void subscribe(Observer& observer) {
    observers.push_back(&observer);
  }

  void unsubscribe(Observer& observer) {
    observers.erase(
      remove(observers.begin(), observers.end(), &observer),
      observers.end()
    );
  }

private:
  std::vector<Observer*> observers;
};
