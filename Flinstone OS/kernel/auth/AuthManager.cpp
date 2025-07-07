#include "AuthManager.hpp"
#include <sodium.h>
#include <iostream>

AuthManager::AuthManager(const std::string& db_path) {
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open auth DB\n";
        db = nullptr;
    } else {
        init_db();
    }
}

AuthManager::~AuthManager() {
    if (db) sqlite3_close(db);
}

void AuthManager::init_db() {
    const char* create_table_sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "username TEXT PRIMARY KEY, "
        "password_hash TEXT NOT NULL, "
        "access_level INTEGER NOT NULL);";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
}

bool AuthManager::create_user(const std::string& username, const std::string& password, const std::string& root_password) {
    char hash[crypto_pwhash_STRBYTES];
    if (crypto_pwhash_str(hash, password.c_str(), password.length(),
                          crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE) != 0) {
        return false;
    }

    int level = (root_password == hardcoded_root) ? 1 : 0;

    const char* insert_sql = "INSERT INTO users (username, password_hash, access_level) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, level);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}

bool AuthManager::login(const std::string& username, const std::string& password) {
    const char* query_sql = "SELECT password_hash, access_level FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query_sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    std::string stored_hash;
    int level = 0;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        stored_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        level = sqlite3_column_int(stmt, 1);
    }
    sqlite3_finalize(stmt);

    if (!stored_hash.empty() &&
        crypto_pwhash_str_verify(stored_hash.c_str(), password.c_str(), password.length()) == 0) {
        current_user = username;
        current_access_level = level;
        return true;
    }

    return false;
}

std::string AuthManager::get_logged_in_user() const {
    return current_user;
}

int AuthManager::get_access_level() const {
    return current_access_level;
}
