#include <memory>

#include <Engine/ConsoleModule.hpp>
#include <Engine/ConsoleCommandRepository.hpp>

#include <Engine/Commands/HelpCommand.hpp>
#include <Engine/Commands/ClearCommand.hpp>

namespace engine {
    void ConsoleModule::ModuleStartup() {
        ConsoleCommandRepository::Instance().RegisterCommand("help", std::move(std::make_unique<commands::HelpCommand>()));
        ConsoleCommandRepository::Instance().RegisterCommand("clear", std::move(std::make_unique<commands::ClearCommand>()));
    }

    void ConsoleModule::ModuleShutdown() {
        ConsoleCommandRepository::Instance().UnregisterCommand("help");
        ConsoleCommandRepository::Instance().UnregisterCommand("clear");
    }
}