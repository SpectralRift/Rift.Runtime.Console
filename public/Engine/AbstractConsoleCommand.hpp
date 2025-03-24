#pragma once

#include <string>
#include <vector>
#include <optional>

#include <Engine/Runtime/IConsole.hpp>

namespace engine {
    struct AbstractConsoleCommand {
        virtual ~AbstractConsoleCommand() = default;

        // optional description
        virtual std::string GetDescription() const { return "No description provided."; }

        // called when an auto complete is requested
        virtual std::vector<std::string> ProvideAutoComplete(const std::vector<std::string>& args) {
            return {};
        }

        // called when the command is executed
        virtual void Execute(runtime::IConsole* console, const std::vector<std::string>& args) = 0;
    };
}