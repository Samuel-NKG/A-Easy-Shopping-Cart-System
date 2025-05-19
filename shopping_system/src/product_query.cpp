#include "product_query.h"
#include <thread>

void ProductQuery::showAllProducts() {
    std::vector<Product> products = productManager.getAllProducts();
    for (const auto& p : products) {
        std::cout << "ID: " << p.id << " 名称: " << p.name
                  << " 分类: " << p.category
                  << " 单价: " << p.price
                  << " 库存: " << p.stock
                  << "\n描述: " << p.description << "\n";

        std::cout << "是否立即购买该商品？(y/n)：";
        char ch;
        std::cin >> ch;
        if (ch == 'y' || ch == 'Y') {
            int qty;
            std::cout << "请输入购买数量：";
            std::cin >> qty;
            if (qty > p.stock) {
                std::cout << "库存不足，仅剩 " << p.stock << " 件。\n";
                continue;
            }

            std::string addr;
            std::cout << "请输入收货地址：";
            std::cin.ignore();
            std::getline(std::cin, addr);

            ShoppingCart tempCart;
            tempCart.addToCart(p, qty);
            int orderId = orderManager->generateOrder(tempCart, productManager, addr, currentUser);
            if (orderId != -1) {
                std::thread t(&OrderManager::autoUpdateOrderStatus, orderManager);
                t.detach();
            }
        }
        std::cout << "----------------------\n";
    }
}


void ProductQuery::searchProductByKeyword(const std::string& keyword) {
    auto products = productManager.getAllProducts();
    bool found = false;
    for (const auto& p : products) {
        if (p.name.find(keyword) != std::string::npos || p.category.find(keyword) != std::string::npos) {
            std::cout << "ID: " << p.id
                      << " 名称: " << p.name
                      << " 分类: " << p.category
                      << " 描述: " << p.description
                      << " 价格: ￥" << p.price
                      << " 库存: " << p.stock << "\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "未找到匹配的商品。\n";
    }
}
