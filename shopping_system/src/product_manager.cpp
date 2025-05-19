#include "product_manager.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include "promotion.h"  

void ProductManager::addProduct() {
    Product p;
    std::cout << "输入名称：";
    std::cin >> p.name;
    std::cout << "输入分类：";
    std::cin >> p.category;
    std::cout << "输入描述：";
    std::cin.ignore();
    std::getline(std::cin, p.description);
    std::cout << "输入单价：";
    std::cin >> p.price;
    std::cout << "输入库存数量：";
    std::cin >> p.stock;

    for (auto& prod : products) {
        if (prod.name == p.name && prod.category == p.category) {
            std::cout << "该商品已存在：库存 " << prod.stock << " 件，是否叠加库存？(y/n)：";
            char ch; std::cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                prod.stock += p.stock;
                std::cout << "库存已更新为 " << prod.stock << "。\n";
            } else {
                std::cout << "取消添加。\n";
            }
            return;
        }
    }

    p.id = products.empty() ? 1 : products.back().id + 1;
    products.push_back(p);
    std::cout << "新商品添加成功！ID: " << p.id << "\n";
}


void ProductManager::updateProduct() {
    int id;
    std::cout << "输入要修改的商品ID：";
    std::cin >> id;
    for (auto& p : products) {
        if (p.id == id) {
            std::cout << "重新输入商品名称 分类 描述 价格 库存：\n";
            std::cin >> p.name >> p.category >> p.description >> p.price >> p.stock;
            saveProducts();
            return;
        }
    }
    std::cout << "未找到商品。\n";
}

void ProductManager::deleteProduct() {
    int id;
    std::cout << "输入要删除的商品ID：";
    std::cin >> id;
    products.erase(
        std::remove_if(products.begin(), products.end(), [id](const Product& p) { return p.id == id; }),
        products.end()
    );
    saveProducts();
}

void ProductManager::listProducts() {
    
    for (const auto& p : products) {
        std::cout << "ID: " << p.id << "\n"
                  << "名称: " << p.name;
        if (p.hasPromotion && isPromotionActive(p.promotion)) {
                std::cout << " 【" << p.promotion.label << "】";
        } 
        std::cout << "\n分类: " << p.category
                  << "\n描述: " << p.description
                  << "\n价格: ￥" << p.price
                  << "\n库存: " << p.stock
                  << "\n---------------------------\n";
    }
}

void ProductManager::setPromotionForProduct(int productId) {
    Product p = getProductById(productId);
    if (p.id == -1) {
        std::cout << "未找到该商品。\n";
        return;
    }

    int type;
    std::cout << "设置促销类型：1. 限时折扣  2. 满减活动\n";
    std::cin >> type;
    std::cin.ignore();
    std::cout << "请输入促销开始时间（格式：2025-05-12 00:00:00）：";
    std::getline(std::cin, p.promotion.startTime);
    std::cout << "请输入促销结束时间（格式：2025-05-20 23:59:59）：";
    std::getline(std::cin, p.promotion.endTime);

    if (type == 1) {
        double rate;
        std::cout << "输入折扣率（如 0.8 表示八折）：";
        std::cin >> rate;

        p.hasPromotion = true;
        p.promotion.type = PromotionType::Discount;
        p.promotion.discountRate = rate;
        p.promotion.threshold = 0.0;
        p.promotion.reduction = 0.0;
        p.promotion.label = "限时" + std::to_string(static_cast<int>(rate * 10)) + "折";

    } else if (type == 2) {
        double threshold, reduce;
        std::cout << "输入满减门槛金额：";
        std::cin >> threshold;
        std::cout << "输入减免金额：";
        std::cin >> reduce;

        p.hasPromotion = true;
        p.promotion.type = PromotionType::FullReduction;
        p.promotion.discountRate = 1.0;
        p.promotion.threshold = threshold;
        p.promotion.reduction = reduce;
        p.promotion.label = "满" + std::to_string((int)threshold) + "减" + std::to_string((int)reduce);
    } else {
        std::cout << "输入错误，取消设置。\n";
        return;
    }

    updateProduct(p);
    saveProducts();
    std::cout << "✅ 促销设置成功，活动将在有效期内生效。\n";
}


void ProductManager::updateProduct(const Product& updatedProduct) {
    for (auto& p : products) {
        if (p.id == updatedProduct.id) {
            p = updatedProduct;
            saveProducts();
            return;
        }
    }
}

Product ProductManager::getProductById(int id) {
    for (const auto& p : products) {
        if (p.id == id) {
            return p;
        }
    }
    Product dummy;
    dummy.id = -1;
    return dummy;
}

std::vector<Product> ProductManager::getAllProducts() {
    return products;
}

void ProductManager::updateProductStock(int productId, int newStock) {
    for (auto& p : products) {
        if (p.id == productId) {
            p.stock = newStock;
            return;
        }
    }
}

void ProductManager::loadProducts() {
    std::ifstream in("data/products.txt");
    if (!in.is_open()) return;
    products.clear();

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        Product p;
        std::string temp;

        std::getline(iss, temp, '|'); p.id = std::stoi(temp);
        std::getline(iss, p.name, '|');
        std::getline(iss, p.category, '|');
        std::getline(iss, p.description, '|');
        std::getline(iss, temp, '|'); p.price = std::stod(temp);
        std::getline(iss, temp, '|'); p.stock = std::stoi(temp);
        std::getline(iss, temp, '|'); p.hasPromotion = (temp == "1");
        std::getline(iss, temp, '|'); p.promotion.type = static_cast<PromotionType>(std::stoi(temp));
        std::getline(iss, temp, '|'); p.promotion.discountRate = std::stod(temp);
        std::getline(iss, temp, '|'); p.promotion.threshold = std::stod(temp);
        std::getline(iss, temp, '|'); p.promotion.reduction = std::stod(temp);
        std::getline(iss, p.promotion.label); 
        std::getline(iss, p.promotion.startTime, '|');
        std::getline(iss, p.promotion.endTime);
        products.push_back(p);
    }

    in.close();
}


void ProductManager::saveProducts() {
    std::ofstream out("data/products.txt");
    for (const auto& p : products) {
        out << p.id << "|" << p.name << "|" << p.category << "|" << p.description << "|"
        << p.price << "|" << p.stock << "|"
        << p.hasPromotion << "|" << static_cast<int>(p.promotion.type) << "|"
        << p.promotion.discountRate << "|" << p.promotion.threshold << "|"
        << p.promotion.reduction << "|" << p.promotion.label << "|"
        << p.promotion.startTime << "|" << p.promotion.endTime << "\n"; 
       }
    out.close();
}