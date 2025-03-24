#include <Engine/Commands/ClearCommand.hpp>

#include <Engine/ConsoleCommandRepository.hpp>

namespace engine::commands {
    void ClearCommand::Execute(runtime::IConsole* console, const std::vector <std::string> &args) {
        console->ClearMessages();
        console->PrintMessage("Cleared the console!", core::runtime::graphics::colors::green);
    }
}
