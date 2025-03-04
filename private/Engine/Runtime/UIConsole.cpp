#include <Engine/Runtime/UIConsole.hpp>

#include <Engine/UI/ImGui.hpp>

namespace engine::runtime {
    void UIConsole::Init() {
        m_ConsoleText.reserve(256);
        b_IsConsoleOpen = true;

        Logger::SubscribeLogger(
                [this](LogLevel log, std::string_view loggerName, std::string_view formattedMessage) {
                    OnLoggerCallback(log, loggerName, formattedMessage);
                });

        PrintMessage("UIConsole Initialized!", core::runtime::graphics::colors::cyan);
    }

    void UIConsole::Shutdown() {
        m_Messages.clear();

        Logger::UnsubscribeLogger(
                [this](LogLevel log, std::string_view loggerName, std::string_view formattedMessage) {
                    OnLoggerCallback(log, loggerName, formattedMessage);
                });
    }

    core::runtime::graphics::Color LogLevel_ToColor(LogLevel level) {
        switch (level) {
            case LOG_LEVEL_ERROR:
                return core::runtime::graphics::colors::red;
            case LOG_LEVEL_WARNING:
                return core::runtime::graphics::colors::yellow;
            case LOG_LEVEL_INFO:
                return core::runtime::graphics::colors::cyan;
            default:
                return core::runtime::graphics::colors::white;
        }
    }

    void UIConsole::OnLoggerCallback(LogLevel log, std::string_view loggerName, std::string_view formattedMessage) {
        auto consoleMessage = std::string("[") + LogLevel_ToString(log) + std::string("] ") + std::string(loggerName) +
                              std::string(": ") + std::string(formattedMessage);

        PrintMessage(consoleMessage, LogLevel_ToColor(log));
    }

    void UIConsole::Render() {
        if(b_IsConsoleOpen) {
            // in non-monolithic builds, we need to set the ImGui context to the global one exposed by the Rift.UI.ImGui module
            // ALWAYS make sure that the current context is the same as the global one before calling any ImGui functions
            if(ImGui::GetCurrentContext() != engine::ui::ImGui_GetGlobalContext()) {
                ImGui::SetCurrentContext(engine::ui::ImGui_GetGlobalContext());
            }

            // hide resize grip until we hover over it, make the window background slightly transparent and set the window size constraints
            ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
            ImGui::SetNextWindowSizeConstraints({600, 400}, {1200, 800});

            auto& io = ImGui::GetIO();

            // draw the console window
            if (ImGui::Begin("Console", &b_IsConsoleOpen, ImGuiWindowFlags_NoCollapse)) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 0.3f));
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
                if (ImGui::BeginChild(
                        "##ConsoleRegion",
                        ImVec2(0, -(ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing())),
                        ImGuiChildFlags_NavFlattened | ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar)) {
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
                    ImGui::PushFont(io.Fonts->Fonts[1]); // Use second font

                    for (const auto &message: m_Messages) {
                        ImGui::PushStyleColor(ImGuiCol_Text, {
                                (float) message.first.r / 255.f,
                                (float) message.first.g / 255.f,
                                (float) message.first.b / 255.f,
                                (float) message.first.a / 255.f
                        });

                        ImGui::TextUnformatted(message.second.c_str());
                        ImGui::PopStyleColor();
                    }

                    ImGui::PopFont();
                    ImGui::PopStyleVar();
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();

                ImGui::Separator();

                ImGui::PushItemWidth(-FLT_MIN);
                if (ImGui::InputText("##ConsoleInput", m_ConsoleText.data(), 256,
                                     ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll)) {
                    m_ConsoleText.clear();
                }
                ImGui::PopItemWidth();
            }

            ImGui::End();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }
    }

    void UIConsole::PrintMessage(std::string_view message, core::runtime::graphics::Color color) {
        m_Messages.push_back({color, std::string(message)});
    }
}