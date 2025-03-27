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
        auto str = std::string{command};

        if(m_Commands.contains(str)) {
            return m_Commands.at(str);
        }
        
        return nullptr;
    }

    const std::unordered_map<std::string, std::shared_ptr<AbstractConsoleCommand>>& ConsoleCommandRepository::GetCommands() const {
        return m_Commands;
    }
}