#pragma once

#include "common.hpp" // Assumed to contain EventId, IObserver, IEventData
#include <functional>
#include <vector>
#include <unordered_map>
#include <algorithm> // For std::remove, std::find
#include <mutex>     // For thread-safety

/**
 * @brief A central event bus for publishing and subscribing to events.
 *
 * Implements the Observer pattern, acting as a Mediator between
 * event publishers and subscribers.
 */
class EventBus {
public:
    EventBus() = default;
    ~EventBus() = default;

    // Prevent copying and assignment to ensure a single instance or controlled instances
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    template<typename EventT>
    void subscribe(std::function<void(const EventT&)> listener) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& vec = m_listeners[EventT::ID];
        vec.push_back([listener](const EventBase& e) {
            listener(static_cast<const EventT&>(e));
        });
    }

    // Subscribe a member function: obj->onEvent(const EventT&)
    template<typename T, typename EventT>
    void subscribe(T* obj, void (T::*method)(const EventT&)) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& vec = m_listeners[EventT::ID];
        vec.push_back([obj, method](const EventBase& e) {
            (obj->*method)(static_cast<const EventT&>(e));
        });
    }

    // Subscribe a const member function: obj->onEvent(const EventT&) const
    template<typename T, typename EventT>
    void subscribe(const T* obj, void (T::*method)(const EventT&) const) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& vec = m_listeners[EventT::ID];
        vec.push_back([obj, method](const EventBase& e) {
            (obj->*method)(static_cast<const EventT&>(e));
        });
    }

    // Bulk subscribe: register multiple event types for an object
    template<typename T, typename... EventTs>
    void subscribeAll(T* obj) {
        // For each EventT, bind T::onEvent(const EventT&)
        (subscribe<T, EventTs>(obj, &T::onEvent), ...);
    }

    void publish(const EventBase& event) {
        std::vector<std::function<void(const EventBase&)>> listeners;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = m_listeners.find(event.GetId());
            if (it != m_listeners.end()) {
                listeners = it->second;
            }
        }

        for (auto& listener : listeners) {
            listener(event);
        }
    }

private:
    std::unordered_map<EventId, std::vector<std::function<void(const EventBase&)>>> m_listeners;
    std::mutex mutex_;
};
