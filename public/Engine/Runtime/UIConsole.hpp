#pragma once

#include <string>
#include <string_view>
#include <array>

#include <Engine/Runtime/Logger.hpp>
#include <Engine/Core/Math/Vector2.hpp>
#include <Engine/Core/Runtime/Graphics/Color.hpp>

#include <Engine/Runtime/IConsole.hpp>

// forward definition for ImGui types
struct ImGuiInputTextCallbackData;

namespace engine::runtime {
    struct UIConsole : public IConsole {
        UIConsole();

        void Init();
        void Shutdown();

        void Render();

        virtual void PrintMessage(std::string_view message, core::runtime::graphics::Color color) override;
        virtual void ClearMessages() override;

        void SetOpen(bool state);
    protected:
        void OnLoggerCallback(LogLevel log, std::string_view loggerName, std::string_view formattedMessage);

        void RenderAutoComplete(core::math::Vector2 cursorPos, core::math::Vector2 size);
        void AutoCompleteLastText(const std::string& text);
        int ConsoleInputCallback(ImGuiInputTextCallbackData* data);

        static int ConsoleInputCallbackWrapper(ImGuiInputTextCallbackData* data);

        void ExecuteCommand(std::string rawInput);

        std::vector<std::pair<core::runtime::graphics::Color, std::string>> m_Messages;
        std::vector<std::string> m_CompletionCandidates;
        std::array<char, 256> m_ConsoleText;

        bool b_IsConsoleOpen{};
        bool b_IsAutoCompleteOpen{};
        bool b_ScrollToBottom{};
    };
}