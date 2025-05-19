#ifndef ORDER_H
#define ORDER_H
#include <vector>
#include <string>

enum class OrderStatus { Pending, Shipped, Received };

struct OrderItem {
    int productId;
    std::string productName;
    double unitPrice; 
    int quantity;
};

struct Order {
    int orderId;
    std::string username; 
    std::vector<OrderItem> items;
    double totalAmount;
    std::string address;
    std::string timestamp;
    OrderStatus status;
};

#endif