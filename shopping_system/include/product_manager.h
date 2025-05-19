#ifndef PRODUCT_MANAGER_H
#define PRODUCT_MANAGER_H

#include <string>
#include <vector>
#include <iostream>
#include "promotion.h" 

struct Product {
    int id;
    std::string name;
    std::string category;
    std::string description;
    double price;
    int stock;

    bool hasPromotion = false;
    Promotion promotion;
};

class ProductManager {
public:
    void addProduct();
    void updateProduct();
    void deleteProduct();
    void listProducts();
    Product getProductById(int id);
    std::vector<Product> getAllProducts();
    void updateProductStock(int productId, int newStock);
    void setPromotionForProduct(int productId);  // 设置促销
    void updateProduct(const Product& updatedProduct); // 外部更新商品信息（含促销）
    void loadProducts();
    void saveProducts();
private:
    std::vector<Product> products;
};

#endif

