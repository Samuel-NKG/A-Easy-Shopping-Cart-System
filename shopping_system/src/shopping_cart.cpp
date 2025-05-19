#include "shopping_cart.h"
#include <iostream>
#include <fstream>
#include <sstream>

void ShoppingCart::loadFromFile(const std::string& filename, const std::string& userId) {
    cartItems.clear(); 
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string uid, pidStr, qtyStr;
        std::getline(ss, uid, ',');
        std::getline(ss, pidStr, ',');
        std::getline(ss, qtyStr, ',');

        if (uid == userId) {
            int pid = std::stoi(pidStr);
            int qty = std::stoi(qtyStr);
            cartItems[pid] = qty;
        }
    }
}

void ShoppingCart::saveToFile(const std::string& filename, const std::string& userId) const {
    std::ifstream infile(filename);
    std::ofstream outfile("data/tmp_cart.txt");
    std::string line;
    while (std::getline(infile, line)) {
        if (line.rfind(userId + ",", 0) != 0) {
            outfile << line << "\n";
        }
    }
    for (const auto& [pid, qty] : cartItems) {
        outfile << userId << "," << pid << "," << qty << "\n";
    }
    infile.close();
    outfile.close();
    std::remove(filename.c_str());
    std::rename("data/tmp_cart.txt", filename.c_str());
}

void ShoppingCart::addToCart(const Product& product, int quantity) {
    if (cartItems.count(product.id)) {
        std::cout << "商品已在购物车中，已更新数量。\n";
        cartItems[product.id] += quantity;
    } else {
        cartItems[product.id] = quantity;
        std::cout << "已加入购物车。\n";
    }
}

void ShoppingCart::removeFromCart(int productId) {
    if (cartItems.erase(productId)) {
        std::cout << "商品已从购物车移除。\n";
    } else {
        std::cout << "购物车中没有该商品。\n";
    }
}

void ShoppingCart::updateQuantity(int productId, int newQuantity) {
    if (newQuantity == 0) {
        removeFromCart(productId);
    } else {
        cartItems[productId] = newQuantity;
        std::cout << "商品数量已更新。\n";
    }
}

void ShoppingCart::viewCart(ProductManager& pm) {
    double total = 0.0;
    std::cout << "您的购物车：\n";
    for (const auto& item : cartItems) {
        Product p = pm.getProductById(item.first);
        if (p.id == -1) {
            std::cout << "商品ID " << item.first << " 无效，已被跳过。\n";
            continue;
        }
        double subtotal = item.second * p.price;
        std::cout << "商品ID: " << p.id << " 名称: " << p.name
                  << " 单价: ￥" << p.price << " 数量: " << item.second
                  << " 小计: ￥" << subtotal << "\n";
        total += subtotal;
    }
    std::cout << "总计: ￥" << total << "\n";
}

std::unordered_map<int, int>& ShoppingCart::getItems() {
    return cartItems;
}

void ShoppingCart::clearCart() {
    cartItems.clear();
}
