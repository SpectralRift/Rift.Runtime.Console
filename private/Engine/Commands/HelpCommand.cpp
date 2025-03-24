#include <Engine/Commands/HelpCommand.hpp>

#include <Engine/ConsoleCommandRepository.hpp>

namespace engine::commands {
    void HelpCommand::Execute(runtime::IConsole* console, const std::vector <std::string> &args) {
        for(const auto& command : ConsoleCommandRepository::Instance().GetCommands()) {
            console->PrintMessage(command.first + " - " + command.second->GetDescription(), core::runtime::graphics::colors::white);
        }
    }
}
