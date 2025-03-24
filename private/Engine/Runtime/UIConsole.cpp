#include <Engine/Runtime/UIConsole.hpp>

#include <Engine/UI/ImGui.hpp>
#include <Engine/ConsoleCommandRepository.hpp>

namespace engine::runtime {
    UIConsole::UIConsole() : m_ConsoleText{}, b_IsConsoleOpen{false} {}

    void UIConsole::Init() {
        m_Messages.clear();
        b_IsConsoleOpen = true;
        b_IsAutoCompleteOpen = false;

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

    void UIConsole::ClearMessages() {
        m_Messages.clear();
    }

    void UIConsole::SetOpen(bool state) {
        b_IsConsoleOpen = state;
    }

    void UIConsole::OnLoggerCallback(LogLevel log, std::string_view loggerName, std::string_view formattedMessage) {
        auto consoleMessage = std::string("[") + LogLevel_ToString(log) + std::string("] ") + std::string(loggerName) +
                              std::string(": ") + std::string(formattedMessage);

        PrintMessage(consoleMessage, LogLevel_ToColor(log));
    }

    void UIConsole::AutoCompleteLastText(const std::string &text) {
        m_ConsoleText.fill(0);
        strncpy(m_ConsoleText.data(), text.c_str(), text.size());
    }

    void UIConsole::RenderAutoComplete(core::math::Vector2 cursorPos, core::math::Vector2 size) {
        if (b_IsAutoCompleteOpen) {
            ImGui::SetNextWindowPos({cursorPos.x, cursorPos.y});
            ImGui::SetNextWindowSize({size.x, size.y});
            ImGui::Begin("##ConsoleAutoComplete", nullptr, ImGuiWindowFlags_NoTitleBar |
                                                           ImGuiWindowFlags_NoResize |
                                                           ImGuiWindowFlags_NoMove |
                                                           ImGuiWindowFlags_HorizontalScrollbar |
                                                           ImGuiWindowFlags_NoSavedSettings);
            {
                ImGui::PushAllowKeyboardFocus(false);
                int i = 0;

                for (auto &item: m_CompletionCandidates) {
                    ImGui::PushID(i++);
                    if (ImGui::Selectable(item.data(), false)) {
                        AutoCompleteLastText(item);
                        m_CompletionCandidates.clear();
                        b_IsAutoCompleteOpen = false;
                    }
                    ImGui::PopID();
                }
            }
            ImGui::PopAllowKeyboardFocus();
            ImGui::End();
        }
    }

    int UIConsole::ConsoleInputCallback(ImGuiInputTextCallbackData *data) {
        switch (data->EventFlag) {
            case ImGuiInputTextFlags_CallbackCompletion:
                if (!b_IsAutoCompleteOpen) {
                    m_CompletionCandidates.clear();

                    std::string input(m_ConsoleText.data());

                    for (auto &command: ConsoleCommandRepository::Instance().GetCommands()) {
                        if (command.first.starts_with(input)) {
                            m_CompletionCandidates.push_back(command.first);
                        }
                    }

                    b_IsAutoCompleteOpen = m_CompletionCandidates.size() > 0;
                }

                break;
            default:
                break;
        }

        return 0;
    }

    int UIConsole::ConsoleInputCallbackWrapper(ImGuiInputTextCallbackData *data) {
        return (*(UIConsole *) data->UserData).ConsoleInputCallback(data);
    }

    void UIConsole::Render() {
        // in non-monolithic builds, we need to set the ImGui context to the global one exposed by the Rift.UI.ImGui module
        // ALWAYS make sure that the current context is the same as the global one before calling any ImGui functions
        if (ImGui::GetCurrentContext() != engine::ui::ImGui_GetGlobalContext()) {
            ImGui::SetCurrentContext(engine::ui::ImGui_GetGlobalContext());
        }

        // toggle console
        if (ImGui::IsKeyReleased(ImGuiKey_Apostrophe)) {
            b_IsConsoleOpen = !b_IsConsoleOpen;
        }

        if (b_IsConsoleOpen) {
            // hide resize grip until we hover over it, make the window background slightly transparent and set the window size constraints
            ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
            ImGui::SetNextWindowSizeConstraints({600, 400}, {1200, 800});

            auto &io = ImGui::GetIO();
            ImVec2 inputPosition;
            core::math::Vector2 autoCompleteSize;
            ImGuiChildFlags winFlags = ImGuiWindowFlags_HorizontalScrollbar;

            // if auto complete is open set the flag to show the popup on top of the window
            if (b_IsAutoCompleteOpen) {
                winFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
            }

            // draw the console window
            if (ImGui::Begin("Console", &b_IsConsoleOpen, winFlags)) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 0.3f));
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
                if (ImGui::BeginChild(
                        "##ConsoleRegion",
                        ImVec2(0, -(ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing())),
                        ImGuiChildFlags_NavFlattened | ImGuiChildFlags_Border, winFlags)) {
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

                    if (b_ScrollToBottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
                        ImGui::SetScrollHereY(1.0f);
                        b_ScrollToBottom = false;
                    }
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();

                ImGui::Separator();

                ImGui::PushItemWidth(-FLT_MIN);

                if (ImGui::InputText("##ConsoleInput", m_ConsoleText.data(), m_ConsoleText.size(),
                                     ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll |
                                     ImGuiInputTextFlags_CallbackCompletion, &UIConsole::ConsoleInputCallbackWrapper,
                                     (void *) this)) {
                    ImGui::SetKeyboardFocusHere(-1);

                    auto rawInput = std::string(m_ConsoleText.data());

                    if(rawInput.size() > 0) {
                        if (!b_IsAutoCompleteOpen) {
                            ExecuteCommand(rawInput);
                            m_ConsoleText.fill(0);
                        }

                        b_IsAutoCompleteOpen = false;
                    }
                }

                inputPosition = ImGui::GetItemRectMin();

                if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                    !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
                    ImGui::SetKeyboardFocusHere(-1);
                }

                autoCompleteSize = {
                        ImGui::GetItemRectSize().x / 2.f,
                        ImGui::GetTextLineHeightWithSpacing() * 4
                };

                inputPosition.y += ImGui::GetItemRectSize().y;

                ImGui::PopItemWidth();
            }

            ImGui::End();

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            if (b_IsAutoCompleteOpen) {
                RenderAutoComplete({inputPosition.x, inputPosition.y}, autoCompleteSize);
            }
        }
    }

    void UIConsole::PrintMessage(std::string_view message, core::runtime::graphics::Color color) {
        m_Messages.push_back({color, std::string(message)});
        b_ScrollToBottom = true;
    }

    void UIConsole::ExecuteCommand(std::string rawInput) {
        PrintMessage(">> " + rawInput, core::runtime::graphics::colors::white);

        auto c = ConsoleCommandRepository::Instance().GetCommand(rawInput);
        if (c) {
            c->Execute(this, {rawInput});
        } else {
            PrintMessage("Unknown command. Please type 'help' to see the available commands.",
                         core::runtime::graphics::colors::red);
        }
    }
}