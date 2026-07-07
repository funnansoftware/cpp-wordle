#pragma once

#include <chrono>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace wordle
{
    // A transient status line (e.g. "Not in word list") shown briefly and then
    // removed. age accumulates how long it has been on screen.
    struct Message
    {
        std::string text;
        std::chrono::duration<float> age{};
    };

    // A small, capacity-bounded queue of timed messages. Purely a data structure:
    // it holds no rendering or platform dependency, so it can be driven and
    // inspected without a window. The view reads all() to draw the toasts.
    class Messages
    {
    public:
        // Never keep more than this many at once; further adds are dropped.
        static constexpr std::size_t max_messages = 25;

        // How long a message lingers before update() removes it.
        static constexpr std::chrono::duration<float> lifetime{1.0F};

        auto add(std::string text) -> void
        {
            if (messages_.size() >= max_messages)
            {
                return;
            }

            messages_.push_back({.text = std::move(text), .age = {}});
        }

        // Age every message by dt and drop those that have outlived lifetime.
        auto update(std::chrono::duration<float> dt) -> void
        {
            for (auto& message : messages_)
            {
                message.age += dt;
            }

            std::erase_if(messages_, [](const Message& message) { return message.age >= lifetime; });
        }

        [[nodiscard]] auto all() const -> const std::vector<Message>&
        {
            return messages_;
        }

    private:
        std::vector<Message> messages_;
    };
}
