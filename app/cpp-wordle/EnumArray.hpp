#pragma once

#include <array>
#include <magic_enum/magic_enum.hpp>

namespace wordle
{
    // Defining the custom Enum concept
    template <typename T>
    concept TypeEnum = std::is_enum_v<T> && magic_enum::detail::is_sparse_v<T> == false && magic_enum::enum_count<T>() > 0;

    template <TypeEnum T, typename U>
    class EnumArray
    {
    public:
        constexpr auto operator[](T index) const -> const auto&
        {
            return data_[static_cast<std::size_t>(index)];
        }

        constexpr auto operator[](T index) -> auto&
        {
            return data_[static_cast<std::size_t>(index)];
        }

        constexpr auto size() const -> std::size_t
        {
            return data_.size();
        }

        constexpr auto empty() const -> bool
        {
            return data_.empty();
        }

        constexpr auto begin() const -> decltype(auto)
        {
            return data_.begin();
        }

        constexpr auto end() const -> decltype(auto)
        {
            return data_.end();
        }

    private:
        std::array<U, static_cast<std::size_t>(magic_enum::enum_count<T>())> data_;
    };
}