#include "datastore.h"
#include <fstream>
#include <iostream>

void DataStore::loadProducts(std::vector<Product>& products) {
    std::ifstream in("data/products.txt");
    if (!in.is_open()) {
        std::cout << "products.txt 文件不存在，自动跳过加载。\n";
        return;
    }
    Product p;
    while (in >> p.id >> p.name >> p.category >> p.description >> p.price >> p.stock) {
        products.push_back(p);
    }
    in.close();
}

void DataStore::saveProducts(const std::vector<Product>& products) {
    std::ofstream out("data/products.txt");
    if (!out.is_open()) {
        std::cout << "无法打开 products.txt 进行写入！\n";
        return;
    }
    for (const auto& p : products) {
        out << p.id << " " << p.name << " " << p.category << " " << p.description
            << " " << p.price << " " << p.stock << "\n";
    }
    out.close();
}

void DataStore::loadUsers(std::unordered_map<std::string, std::string>& users) {
    std::ifstream in("data/users.txt");
    if (!in.is_open()) {
        std::cout << "users.txt 文件不存在，自动跳过加载。\n";
        return;
    }
    std::string user, pass;
    while (in >> user >> pass) {
        users[user] = pass;
    }
    in.close();
}

void DataStore::saveUsers(const std::unordered_map<std::string, std::string>& users) {
    std::ofstream out("data/users.txt");
    if (!out.is_open()) {
        std::cout << "无法打开 users.txt 进行写入！\n";
        return;
    }
    for (const auto& pair : users) {
        out << pair.first << " " << pair.second << "\n";
    }
    out.close();
}
