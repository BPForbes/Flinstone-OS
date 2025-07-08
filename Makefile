CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDLIBS = -lsqlite3 -lsodium
AUTH_SRC = kernel/auth/AuthManager.cpp
SHELL_SRC = kernel/shell/ShellCommand.cpp kernel/shell/main_shell.cpp
LWIP_SRC = $(wildcard lwip/src/*.cpp)
INCLUDES = -Ikernel/auth -Ikernel/shell -Ilwip/include

all: shell

shell: $(AUTH_SRC) $(SHELL_SRC) $(LWIP_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ $(LDLIBS) -o shell

clean:
	rm -f shell
