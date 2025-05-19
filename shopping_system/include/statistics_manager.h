#pragma once
#include <map>
#include <string>
#include <vector>
#include "product_manager.h"
#include "order.h"

class StatisticsManager {
public:
    void generateStatistics(const std::vector<Order>& orders,
        const std::vector<Product>& products,
        const std::string& exportPath = "");
    void exportChartHTML(const std::vector<Order>& orders,
        const std::vector<Product>& products,
        const std::string& htmlPath);
};

#endif