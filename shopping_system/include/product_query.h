#ifndef PRODUCT_QUERY_H
#define PRODUCT_QUERY_H
#include <string>
#include <iostream>
#include "product_manager.h"
#include "order_manager.h" 
#include "shopping_cart.h"    

class ProductQuery {
    public:
        ProductQuery(ProductManager& pm, OrderManager& om, const std::string& username)
            : productManager(pm), orderManager(&om), currentUser(username) {}
        ProductQuery(ProductManager& pm, OrderManager& om)
            : productManager(pm), orderManager(&om), currentUser("") {}
        void showAllProducts();
        void searchProductByKeyword(const std::string& keyword);
    private:
        ProductManager& productManager;
        OrderManager* orderManager;
        std::string currentUser;
    };

#endif