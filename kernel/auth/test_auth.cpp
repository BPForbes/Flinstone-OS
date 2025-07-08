#include "AuthManager.hpp"
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
        if (cmd == "exit") break;

        std::cout << "Username: ";
        std::cin >> user;
        std::cout << "Password: ";
        std::cin >> pass;

        if (cmd == "signup") {
            std::cout << "Enter root password (leave blank for regular user): ";
            std::cin.ignore();
            std::getline(std::cin, root);

            if (auth.create_user(user, pass, root))
                std::cout << "User created.\n";
            else
                std::cout << "Failed to create user.\n";
        } else if (cmd == "login") {
            if (auth.login(user, pass)) {
                std::cout << "Login successful. Welcome, " << auth.get_logged_in_user()
                          << " (level " << auth.get_access_level() << ")\n";
            } else {
                std::cout << "Login failed.\n";
            }
        }
    }
    return 0;
}
