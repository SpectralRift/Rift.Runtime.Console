#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include <Engine/AbstractConsoleCommand.hpp>

namespace engine {
    class ConsoleCommandRepository {
    public:
        virtual ~ConsoleCommandRepository() = default;

        static ConsoleCommandRepository& Instance();

        void RegisterCommand(std::string_view command, std::shared_ptr<AbstractConsoleCommand> commandItx);
        void UnregisterCommand(std::string_view command);

        std::shared_ptr<AbstractConsoleCommand> GetCommand(std::string_view command);
        const std::unordered_map<std::string, std::shared_ptr<AbstractConsoleCommand>>& GetCommands() const;
    protected:
        std::unordered_map<std::string, std::shared_ptr<AbstractConsoleCommand>> m_Commands;
    };
}