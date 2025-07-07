CXX = g++
CXXFLAGS = -std=c++17 -Wall -lsqlite3 -lsodium
AUTH_SRC = kernel/auth/AuthManager.cpp
SHELL_SRC = kernel/shell/ShellCommand.cpp kernel/shell/main_shell.cpp
INCLUDES = -Ikernel/auth -Ikernel/shell

all: shell

shell: $(AUTH_SRC) $(SHELL_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o shell $^

clean:
	rm -f shell
