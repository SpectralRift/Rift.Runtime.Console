#pragma once

#include <string_view>

#include <Engine/Core/Runtime/Graphics/Color.hpp>

namespace engine::runtime {
    struct IConsole {
        virtual ~IConsole() = default;

        virtual void PrintMessage(std::string_view message, core::runtime::graphics::Color color) = 0;

        virtual void ClearMessages() = 0;
    };
}