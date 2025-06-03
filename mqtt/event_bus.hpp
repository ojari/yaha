#pragma once

#include "common.hpp" // Assumed to contain EventId, IObserver, IEventData
#include <vector>
#include <map>
#include <algorithm> // For std::remove, std::find
#include <mutex>     // For thread-safety


/**
 * @brief A central event bus for publishing and subscribing to events.
 *
 * Implements the Observer pattern, acting as a Mediator between
 * event publishers and subscribers.
 */
class EventBus : public IEventBus {
public:
    EventBus() = default;
    ~EventBus() = default;

    // Prevent copying and assignment to ensure a single instance or controlled instances
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    /**
     * @brief Subscribes an observer to a specific event type.
     *
     * If the observer is already subscribed to this event type,
     * the call is ignored.
     *
     * @param eventId The ID of the event to subscribe to.
     * @param observer Pointer to the observer object. Must not be null.
     *                 The observer is responsible for unsubscribing before it's destroyed.
     */
    void subscribe(EventId eventId, IObserver* observer) override {
        if (!observer) {
            // Optionally, log an error or throw an exception for null observer
            return;
        }
        std::lock_guard<std::mutex> lock(observerMutex_);
        auto& observersForEvent = observers_[eventId];
        // Avoid duplicate subscriptions
        if (std::find(observersForEvent.begin(), observersForEvent.end(), observer) == observersForEvent.end()) {
            observersForEvent.push_back(observer);
        }
    }

    /**
     * @brief Unsubscribes an observer from a specific event type.
     *
     * If the observer is not found or the event type has no subscribers,
     * the call has no effect.
     *
     * @param eventId The ID of the event to unsubscribe from.
     * @param observer Pointer to the observer object.
     */
    void unsubscribe(EventId eventId, IObserver* observer) override {
        if (!observer) {
            return;
        }
        std::lock_guard<std::mutex> lock(observerMutex_);
        auto it = observers_.find(eventId);
        if (it != observers_.end()) {
            auto& observersForEvent = it->second;
            observersForEvent.erase(
                std::remove(observersForEvent.begin(), observersForEvent.end(), observer),
                observersForEvent.end()
            );
            // Optionally, remove the eventId from the map if no observers are left
            if (observersForEvent.empty()) {
                observers_.erase(it);
            }
        }
    }

    /**
     * @brief Publishes an event to all subscribed observers for its type.
     *
     * The notification happens synchronously on the calling thread.
     * Observers are notified in the order they were subscribed.
     *
     * @param eventData The event data to publish.
     */
    void publish(const IEventData& eventData)  {
        std::vector<IObserver*> subscribersToNotify;
        EventId id = eventData.id();

        {
            std::lock_guard<std::mutex> lock(observerMutex_);
            auto it = observers_.find(id);
            if (it != observers_.end()) {
                // Copy the list of subscribers to allow modifications (unsubscribe)
                // from within an observer's onChange method without invalidating iterators.
                subscribersToNotify = it->second;
            }
        } // Mutex is released before calling observers

        for (IObserver* observer : subscribersToNotify) {
            // It's good practice to ensure observer is still valid if possible,
            // though with raw pointers, this is limited.
            // The primary responsibility is on the observer to unsubscribe.
            if (observer) {
                observer->onChange(eventData);
            }
        }
    }

private:
    std::map<EventId, std::vector<IObserver*>> observers_;
    std::mutex observerMutex_; // Protects access to the observers_ map
};
