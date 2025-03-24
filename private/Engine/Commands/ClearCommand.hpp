#pragma once

#include <Engine/AbstractConsoleCommand.hpp>

namespace engine::commands {
    struct ClearCommand : public AbstractConsoleCommand {
        virtual std::string GetDescription() const override {
            return "Clear the console messages.";
        }

        virtual void Execute(runtime::IConsole* console, const std::vector<std::string> &args) override;
    };
}
