#include <Engine/ConsoleCommandRepository.hpp>

namespace engine {
    ConsoleCommandRepository& ConsoleCommandRepository::Instance() {
        static ConsoleCommandRepository instance;
        return instance;
    }

    void ConsoleCommandRepository::RegisterCommand(std::string_view command, std::shared_ptr<AbstractConsoleCommand> commandItx) {
        m_Commands[std::string(command)] = std::move(commandItx);
    }

    void ConsoleCommandRepository::UnregisterCommand(std::string_view command) {
        m_Commands.erase(std::string(command));
    }

    std::shared_ptr<AbstractConsoleCommand> ConsoleCommandRepository::GetCommand(std::string_view command) {
        return m_Commands[std::string(command)];
    }

    const std::unordered_map<std::string, std::shared_ptr<AbstractConsoleCommand>>& ConsoleCommandRepository::GetCommands() const {
        return m_Commands;
    }
}