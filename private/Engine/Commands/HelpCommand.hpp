#pragma once

#include <Engine/AbstractConsoleCommand.hpp>

namespace engine::commands {
    struct HelpCommand : public AbstractConsoleCommand {
        virtual std::string GetDescription() const override {
            return "Displays this help message";
        }

        // called when the command is executed
        virtual void Execute(runtime::IConsole* console, const std::vector<std::string> &args) override;
    };
}
