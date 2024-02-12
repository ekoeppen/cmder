#pragma once

#include <printer.h>
#include <remix/bytes_view.h>

namespace cmder {

using namespace remix::literals;

typedef void (&CommandHandler)(remix::bytes_view);

struct Command {
    remix::bytes_view command;
    remix::bytes_view description;
    CommandHandler handler;

    auto handles(remix::bytes_view input) const -> bool
    {
        if (input == command) {
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
    std::span<remix::bytes_view> info;

    auto handleCommand(std::span<uint8_t> input) -> void
    {
        auto command = remix::bytes_view(input);
        auto n = command.substr(0, command.find(';'));
        for (auto const& c : commands) {
            if (c.handles(n)) {
                return;
            }
        }
        if (n == "ping"_b) {
            console.write("pong;;;\n"_b);
            return;
        }
        if (n == "info"_b) {
            showInfo();
            return;
        }
        if (n == "commands"_b) {
            showCommands();
            return;
        }
        console.write("error;;;Unknown command: "_b);
        console.write(input);
        console.send('\n');
    }

    auto showInfo() -> void
    {
        auto print = printer::printer { console };
        for (int n = 1; const auto& i : info) {
            console.write("info;"_b);
            print(n);
            console.send(';');
            print(std::size(info));
            console.send(';');
            console.write(i);
            console.send('\n');
            n++;
        }
    }

    auto showCommands() -> void
    {
        auto print = printer::printer { console };
        for (int n = 1; const auto& c : commands) {
            console.write("command;"_b);
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
};

}
