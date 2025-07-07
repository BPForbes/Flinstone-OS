#ifndef SHELL_COMMAND_HPP
#define SHELL_COMMAND_HPP

#include "../auth/AuthManager.hpp"
#include <string>

class ShellCommand {
public:
    ShellCommand(AuthManager& auth_ref);
    void run();

private:
    AuthManager& auth;
    void execute(const std::string& input);
    void cmd_ls();
    void cmd_cd(const std::string& path);
    void cmd_mkdir(const std::string& name);
    void cmd_touch(const std::string& name);
    void cmd_rm(const std::string& name);
    void cmd_shutdown();
};

#endif
