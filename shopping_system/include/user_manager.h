#ifndef USER_MANAGER_H
#define USER_MANAGER_H
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <map>

class UserManager {
    public:
        bool registerUser(const std::string& username, const std::string& password);
        bool loginUser(const std::string& username, const std::string& password);
        bool changePassword(const std::string& username);
        bool isAdmin(const std::string& username);
        bool userExists(const std::string& username);  
        void loadUsers();
        void saveUsers();
    private:
        std::unordered_map<std::string, std::string> users; 
        std::map<std::string, bool> adminFlag;
    };

#endif