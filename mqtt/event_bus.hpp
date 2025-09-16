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
    using Listener = std::function<void(const EventT&)>;

    template<typename EventT>
    void subscribe(Listener<EventT> listener) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& vec = m_listeners[typeid(EventT).name()];
        vec.push_back([listener](const EventBase& e) {
            listener(static_cast<const EventT&>(e));
        });
    }

    template<typename EventT>
    void publish(const EventT& event) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = m_listeners.find(typeid(EventT).name());
        if (it != m_listeners.end()) {
            for (auto& listener : it->second) {
                listener(event);
            }
        }
    }

private:
    std::unordered_map<std::string, std::vector<std::function<void(const EventBase&)>>> m_listeners;
    std::mutex mutex_;
};
