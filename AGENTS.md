# AGENTS.md

## 🧠 Overview  
This document defines the **AI Agent** roles and responsibilities for working with the **Flinstone‑OS** C++ project (BPForbes/Flinstone‑OS). It’s designed for use with Codex-style toolchains, enabling automated analysis, assistance, and enhancements.

---

## Agent Roles & Responsibilities

### 1. Project Loader  
- **Load and parse** the full repository, including key directories (`kernel/`, `shell/`, `drivers/`, etc.).  
- **Detect build artifacts** such as `Makefile` or `CMakeLists.txt`, and list source files.  
- **Identify external dependencies**, e.g., `libsodium`, `sqlite3`.

### 2. Architecture Agent  
- **Map out system structure**, identifying modules like authentication (`kernel/auth/`), shell I/O (`kernel/shell/`), and drivers (`kernel/drivers/`).  
- **Draw interaction diagrams** between modules, including authentication flows, shell command dispatch, and database access.

### 3. Build/Dependency Agent  
- **Parse and execute** the project’s build instructions (e.g., from `Makefile` or `Makefile.make`).  
- **Compile source files** with correct flags (`-std=c++17`, `-lsqlite3`, `-lsodium`).  
- **Verify build success** and report issues like missing headers or libraries.

### 4. Documentation Agent  
- **Generate module summaries** (e.g., explain `AuthManager`, `ShellCommand`).  
- **Document workflows**, e.g., user login steps, shell command registration, DB query paths.  
- **Produce Markdown docs** or update README based on project structure and logic.

### 5. Query/Assist Agent  
- **Answer developer questions**, such as “How do I add a new command?” or “Where is the user authentication flow?”  
- **Suggest or apply code changes**, like “Add `logout` command to `ShellCommand.cpp`” or refactor database calls.  
- **Provide annotated diffs** when patching code to meet requested changes.

### 6. Test Agent  
- **Propose test cases**, for example:
  - Valid and invalid user login sequences
  - Database initialization and persistence
  - Shell command behavior on edge cases  
- **Autogenerate test code**, e.g. `login_test.cpp` using a unit test framework or shell scripts testing `./shell`.

---

## Usage with Codex

1. **Load environment**  
   Clone or open the repository in your workspace.

2. **Build the project**  
   Run:
   ```bash
   g++ -std=c++17 \
     kernel/auth/AuthManager.cpp \
     kernel/shell/ShellCommand.cpp \
     kernel/shell/main_shell.cpp \
     -lsqlite3 -lsodium -o shell
    ```