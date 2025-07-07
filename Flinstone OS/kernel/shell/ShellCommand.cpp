#include "ShellCommand.hpp"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

namespace fs = std::filesystem;

ShellCommand::ShellCommand(AuthManager& auth_ref) : auth(auth_ref) {}

void ShellCommand::run() {
    std::string line;
    while (true) {
        std::cout << auth.get_logged_in_user() << " > ";
        std::getline(std::cin, line);
        if (line == "exit") break;
        execute(line);
    }
}

void ShellCommand::execute(const std::string& input) {
    std::istringstream iss(input);
    std::string cmd;
    iss >> cmd;

    if (cmd == "ls") cmd_ls();
    else if (cmd == "cd") {
        std::string path;
        iss >> path;
        cmd_cd(path);
    } else if (cmd == "mkdir") {
        std::string name;
        iss >> name;
        cmd_mkdir(name);
    } else if (cmd == "touch") {
        std::string name;
        iss >> name;
        cmd_touch(name);
    } else if (cmd == "rm") {
        std::string name;
        iss >> name;
        cmd_rm(name);
    } else if (cmd == "shutdown") {
        cmd_shutdown();
    } else {
        std::cout << "Unknown command: " << cmd << "\n";
    }
}

void ShellCommand::cmd_ls() {
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        std::cout << entry.path().filename().string() << "\n";
    }
}

void ShellCommand::cmd_cd(const std::string& path) {
    std::error_code ec;
    fs::current_path(path, ec);
    if (ec) std::cout << "cd error: " << ec.message() << "\n";
}

void ShellCommand::cmd_mkdir(const std::string& name) {
    fs::create_directory(name);
}

void ShellCommand::cmd_touch(const std::string& name) {
    std::ofstream(name).close();
}

void ShellCommand::cmd_rm(const std::string& name) {
    if (auth.get_access_level() < 1) {
        std::cout << "Permission denied: admin only\n";
        return;
    }
    fs::remove(name);
}

void ShellCommand::cmd_shutdown() {
    if (auth.get_access_level() < 1) {
        std::cout << "Permission denied: admin only\n";
        return;
    }
    std::cout << "Shutting down...\n";
    std::exit(0);
}
