#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <vector>
#include <string>
#include "order.h"
#include "shopping_cart.h"
#include "product_manager.h"
#include "full_reduction_rule.h" 
#include "order.h"

class OrderManager {
public:
    int generateOrder(ShoppingCart& cart, ProductManager& pm, const std::string& address, const std::string& username);
    void updateOrderStatus(int orderId);
    void autoUpdateOrderStatus();
    void queryOrders(const std::string& currentUser, bool isAdmin) const;
    void modifyOrder(int orderId, ProductManager& pm);
    void deleteOrder(int orderId, ProductManager& pm);
    void saveOrders();
    void loadOrders();
    std::string statusToString(OrderStatus status) const;
    std::vector<Order> getAllOrders() const;
    FullReductionRule fullReduction; 
private:
    std::vector<Order> orders;
    int orderCounter = 1;
    
};

#endif
