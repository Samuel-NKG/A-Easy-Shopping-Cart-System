#include "order_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <ctime>
#include "promotion.h"  
#include "full_reduction_rule.h"  
#include <ctime>
#include <sstream>
#include <iomanip>

std::string getCurrentTimestamp() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

int OrderManager::generateOrder(ShoppingCart& cart, ProductManager& pm, const std::string& address, const std::string& username) {
    Order order;
    order.orderId = orderCounter++;
    order.totalAmount = 0.0;
    order.address = address;
    order.timestamp = getCurrentTimestamp();
    order.status = OrderStatus::Pending;
    order.username = username;

    auto& items = cart.getItems();
    std::vector<int> toRemove;

    for (auto& item : items) {
        Product p = pm.getProductById(item.first);
        if (p.id == -1 || p.stock < item.second) {
            toRemove.push_back(item.first);
            continue;
        }

        p.stock -= item.second;
        pm.updateProductStock(p.id, p.stock);

        double unitPrice = p.price;
        if (p.hasPromotion && p.promotion.type == PromotionType::Discount && isPromotionActive(p.promotion)) {
            unitPrice *= p.promotion.discountRate;
        }        

        OrderItem oi = {p.id, p.name, unitPrice, item.second};
        order.items.push_back(oi);
        order.totalAmount += unitPrice * item.second;
    }

    for (int pid : toRemove) {
        cart.removeFromCart(pid);
    }

    if (order.items.empty()) {
        std::cout << "订单为空，无法生成。\n";
        return -1;
    }

    if (fullReduction.threshold > 0 &&
        order.totalAmount >= fullReduction.threshold &&
        isFullReductionActive(fullReduction)) {
        std::cout << "🎉 满减生效：" << fullReduction.label << "\n";
        order.totalAmount -= fullReduction.reduction;
    }
    
    orders.push_back(order);
    cart.clearCart();
    std::cout << "订单生成成功！订单号：" << order.orderId << "\n";

    saveOrders();
    pm.saveProducts();
    return order.orderId;
}

void OrderManager::queryOrders(const std::string& currentUser, bool isAdmin) const {
    for (const auto& order : orders) {
        std::cout << "订单号: " << order.orderId
                  << " 状态: " << statusToString(order.status)
                  << " 时间: " << order.timestamp << "\n";

        std::cout << "下单用户: " << order.username << "\n";
        std::cout << "收货地址: " << order.address
                  << " 总金额: ￥" << order.totalAmount << "\n";

        for (const auto& item : order.items) {
            std::cout << "  - " << item.productName << " 单价: " << item.unitPrice
                      << " 数量: " << item.quantity << "\n";
        }
        std::cout << "---------------------------\n";
    }
}

std::vector<Order> OrderManager::getAllOrders() const {
    return orders;
}

std::string OrderManager::statusToString(OrderStatus status) const {
    switch (status) {
        case OrderStatus::Pending: return "待发货";
        case OrderStatus::Shipped: return "已发货";
        case OrderStatus::Received: return "已收货";
        default: return "未知";
    }
}

void OrderManager::saveOrders() {
    std::ofstream out("data/orders.txt");
    for (const auto& o : orders) {
        out << o.orderId << "|" << o.username << "|" << static_cast<int>(o.status)
            << "|" << o.timestamp << "|" << o.address << "|" << o.totalAmount << "|" << o.items.size() << "\n";
        for (const auto& item : o.items) {
            out << item.productId << "|" << item.productName << "|" << item.unitPrice << "|" << item.quantity << "\n";
        }
    }
    out.close();
}

void OrderManager::loadOrders() {
    std::ifstream in("data/orders.txt");
    if (!in.is_open()) return;
    orders.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
    
        std::istringstream iss(line);
        Order o;
        std::string temp;
        int statusInt, itemCount;
    
        std::getline(iss, temp, '|'); o.orderId = std::stoi(temp);
        std::getline(iss, o.username, '|');
        std::getline(iss, temp, '|'); statusInt = std::stoi(temp);
        std::getline(iss, o.timestamp, '|');
        std::getline(iss, o.address, '|');
        std::getline(iss, temp, '|'); o.totalAmount = std::stod(temp);
        std::getline(iss, temp); itemCount = std::stoi(temp);
    
        o.status = static_cast<OrderStatus>(statusInt);
    
        for (int i = 0; i < itemCount; ++i) {
            if (!std::getline(in, line)) break;
            std::istringstream itemss(line);
            OrderItem item;
            std::getline(itemss, temp, '|'); item.productId = std::stoi(temp);
            std::getline(itemss, item.productName, '|');
            std::getline(itemss, temp, '|'); item.unitPrice = std::stod(temp);
            std::getline(itemss, temp); item.quantity = std::stoi(temp);
            o.items.push_back(item);
        }
    
        orders.push_back(o);
    }
    

    if (!orders.empty()) {
        orderCounter = orders.back().orderId + 1;
    }
    in.close();
}

void OrderManager::updateOrderStatus(int orderId) {
    for (auto& order : orders) {
        if (order.orderId == orderId) {
            int choice;
            std::cout << "当前状态: " << statusToString(order.status) << "\n";
            std::cout << "选择新的状态：0-待发货 1-已发货 2-已收货\n";
            std::cin >> choice;
            order.status = static_cast<OrderStatus>(choice);
            std::cout << "订单状态已更新为: " << statusToString(order.status) << "\n";
            saveOrders();
            return;
        }
    }
    std::cout << "未找到订单号为 " << orderId << " 的订单。\n";
}

void OrderManager::modifyOrder(int orderId, ProductManager& pm) {
    for (auto& order : orders) {
        if (order.orderId == orderId) {
            std::cout << "[模拟] 修改订单功能未完全实现，仅作演示。\n";
            std::cout << "原地址: " << order.address << "\n";
            std::cout << "输入新地址: ";
            std::cin.ignore();
            std::getline(std::cin, order.address);
            saveOrders();
            std::cout << "订单地址已更新。\n";
            return;
        }
    }
    std::cout << "未找到订单号为 " << orderId << " 的订单。\n";
}

void OrderManager::autoUpdateOrderStatus() {
    for (auto& order : orders) {
        if (order.status == OrderStatus::Pending) {
            order.status = OrderStatus::Shipped;
        } else if (order.status == OrderStatus::Shipped) {
            order.status = OrderStatus::Received;
        }
    }
    std::cout << "所有订单状态已自动推进。\n";
    saveOrders();
}
