#include "../auth/AuthManager.hpp"
#include "ShellCommand.hpp"
#include <iostream>
extern "C" {
#include <sodium.h>
}

int main() {
    if (sodium_init() == -1) {
        std::cerr << "libsodium init failed\n";
        return 1;
    }

    AuthManager auth("users.db");
    std::string cmd, user, pass, root;

    while (true) {
        std::cout << "(signup/login/exit)> ";
        std::cin >> cmd;
        if (cmd == "exit") return 0;

        std::cout << "Username: ";
        std::cin >> user;
        std::cout << "Password: ";
        std::cin >> pass;

        if (cmd == "signup") {
            std::cout << "Root password (leave blank for user): ";
            std::cin.ignore();
            std::getline(std::cin, root);
            if (auth.create_user(user, pass, root)) {
                std::cout << "User created.\n";
            } else {
                std::cout << "Failed to create user.\n";
            }
        } else if (cmd == "login") {
            if (auth.login(user, pass)) {
                std::cout << "Login successful.\n";
                break;
            } else {
                std::cout << "Login failed.\n";
            }
        }
    }

    ShellCommand shell(auth);
    shell.run();

    return 0;
}
