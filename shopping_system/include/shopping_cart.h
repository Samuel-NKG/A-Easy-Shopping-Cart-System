#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H
#include <unordered_map>
#include <iostream>
#include "product_manager.h"
#include <map>
#include <string>

class ShoppingCart {
public:
    void addToCart(const Product& product, int quantity);
    void removeFromCart(int productId);
    void updateQuantity(int productId, int newQuantity);
    void viewCart(ProductManager& pm);
    std::unordered_map<int, int>& getItems();
    void clearCart();
    void loadFromFile(const std::string& filename, const std::string& userId);
    void saveToFile(const std::string& filename, const std::string& userId) const;
private:
    std::unordered_map<int, int> cartItems; 
    std::map<std::string, int> items;
};

#endif