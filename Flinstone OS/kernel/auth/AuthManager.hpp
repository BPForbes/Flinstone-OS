#ifndef AUTH_MANAGER_HPP
#define AUTH_MANAGER_HPP

#include <string>
#include <sqlite3.h>

class AuthManager {
public:
    AuthManager(const std::string& db_path);
    ~AuthManager();

    bool create_user(const std::string& username, const std::string& password, const std::string& root_password = "");
    bool login(const std::string& username, const std::string& password);
    std::string get_logged_in_user() const;
    int get_access_level() const;

private:
    sqlite3* db;
    std::string current_user;
    int current_access_level = 0;

    void init_db();
    static constexpr const char* hardcoded_root = "letmeinroot"; // Change in production
};

#endif
