#include "user_manager.h"

bool UserManager::registerUser(const std::string& username, const std::string& password) {
    if (users.count(username)) {
        std::cout << "用户已存在。\n";
        return false;
    }
    users[username] = password;
    adminFlag[username] = (username == "admin"); 
    std::cout << "注册成功。\n";
    return true;
}

bool UserManager::userExists(const std::string& username) {
    return users.find(username) != users.end();
}

bool UserManager::loginUser(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if (it != users.end() && it->second == password) {
        std::cout << "登录成功，欢迎 " << username << "！\n";
        return true;
    }
    return false;
}


bool UserManager::changePassword(const std::string& username) {
    std::string oldPwd, newPwd;
    std::cout << "请输入原密码："; std::cin >> oldPwd;
    if (users[username] != oldPwd) {
        std::cout << "原密码不正确。\n";
        return false;
    }
    std::cout << "请输入新密码："; std::cin >> newPwd;
    users[username] = newPwd;
    std::cout << "密码修改成功。\n";
    return true;
}

bool UserManager::isAdmin(const std::string& username) {
    return adminFlag.count(username) ? adminFlag[username] : false;
}

void UserManager::loadUsers() {
    std::ifstream in("data/users.txt");
    std::string user, pass;
    while (in >> user >> pass) {
        users[user] = pass;
        adminFlag[user] = (user == "admin");
    }
}

void UserManager::saveUsers() {
    std::ofstream out("data/users.txt");
    for (const auto& pair : users) {
        out << pair.first << " " << pair.second << "\n";
    }
}
