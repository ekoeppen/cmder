#pragma once

#include <printer.h>
#include <span>
#include <string_view>

namespace cmder {

typedef void (&CommandHandler)(std::string_view);

struct Command {
    std::string_view command;
    std::string_view description;
    CommandHandler handler;

    auto handles(std::string_view cmd, std::string_view input) const -> bool
    {
        if (cmd == command) {
            handler(input);
            return true;
        }
        return false;
    }
};

template<typename Console>
struct Cmder {
    Console& console;
    std::span<Command> commands;
    std::span<std::string_view> info;

    auto handleCommand(std::string_view input) -> void
    {
        auto cmd = input.substr(0, input.find(';'));
        for (auto const& c : commands) {
            if (c.handles(cmd, input)) {
                return;
            }
        }
        if (cmd == "ping") {
            console.write(std::string_view { "pong\n" });
            return;
        }
        if (cmd == "info") {
            showInfo();
            return;
        }
        if (cmd == "commands") {
            showCommands();
            return;
        }
        console.write(std::string_view { "error;;;Unknown command: " });
        console.write(input);
        console.send('\n');
    }

    auto showInfo() -> void { sendResponse("info", info); }

    auto showCommands() -> void
    {
        auto print = printer::printer { console };
        for (int n = 1; const auto& c : commands) {
            console.write(std::string_view { "command;" });
            print(n);
            console.send(';');
            print(std::size(commands));
            console.send(';');
            console.write(c.command);
            console.send(';');
            console.write(c.description);
            console.send('\n');
            n++;
        }
    }

    auto sendResponse(std::string_view response, std::span<std::string_view> const& values)
    {
        auto print = printer::printer { console };
        for (int n = 1; const auto& v : values) {
            console.write(response);
            console.send(';');
            print(n);
            console.send(';');
            print(std::size(values));
            console.send(';');
            console.write(v);
            console.send('\n');
            n++;
        }
    }
};

}
