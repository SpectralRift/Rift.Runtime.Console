#pragma once

#include <string>
#include <string_view>

#include <Engine/Runtime/Logger.hpp>
#include <Engine/Core/Runtime/Graphics/Color.hpp>

namespace engine::runtime {
    struct UIConsole {
        void Init();
        void Shutdown();

        void Render();
        void PrintMessage(std::string_view message, core::runtime::graphics::Color color);
    protected:
        void OnLoggerCallback(LogLevel log, std::string_view loggerName, std::string_view formattedMessage);

        std::vector<std::pair<core::runtime::graphics::Color, std::string>> m_Messages;
        std::string m_ConsoleText;
        bool b_IsConsoleOpen;
    };
}