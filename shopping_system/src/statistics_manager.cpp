
#include "statistics_manager.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>

void StatisticsManager::exportChartHTML(const std::vector<Order>& orders,const std::vector<Product>& products, const std::string& htmlPath) {

    struct Stat {
        std::string name;
        double totalAmount = 0;
    };

    std::map<int, Product> productMap;
    for (const auto& p : products) {
        productMap[p.id] = p;
    }

    std::map<std::string, std::map<std::string, Stat>> stats;

    for (const auto& order : orders) {
        for (const auto& item : order.items) {
            int pid = item.productId;
            double price = item.unitPrice * item.quantity;
            if (productMap.count(pid)) {
                const Product& p = productMap[pid];
                stats[p.category][p.name].name = p.name;
                stats[p.category][p.name].totalAmount += price;
            }
        }
    }

    std::ofstream out(htmlPath);
    out << "<!DOCTYPE html>\n<html><head><meta charset='utf-8'>\n";
    out << "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>\n";
    out << "<title>购买统计图表</title></head><body>\n";

    int chartIndex = 0;
    for (const auto& [category, prodStats] : stats) {
        std::string canvasId = "chart" + std::to_string(chartIndex++);
        out << "<h2>" << category << "</h2>\n";
        out << "<canvas id='" << canvasId << "' height='300'></canvas>\n";
        out << "<script>\nconst ctx" << canvasId << " = document.getElementById('" << canvasId << "');\n";
        out << "new Chart(ctx" << canvasId << ", {\n"
            << "type: 'bar',\n"
            << "data: {\nlabels: [";

        bool first = true;
        for (const auto& [name, stat] : prodStats) {
            if (!first) out << ",";
            out << "'" << name << "'";
            first = false;
        }
        out << "],\n datasets: [{\nlabel: '购买金额 (元)',\ndata: [";

        first = true;
        for (const auto& [name, stat] : prodStats) {
            if (!first) out << ",";
            out << stat.totalAmount;
            first = false;
        }
        out << "], backgroundColor: 'rgba(54, 162, 235, 0.6)'}] }, options: {scales: {y: {beginAtZero: true}}}});\n";
        out << "</script>\n";
    }

    out << "</body></html>\n";
    out.close();
    std::cout << "✅ 图表已生成： " << htmlPath << "\n";
}

void StatisticsManager::generateStatistics(const std::vector<Order>& orders,
                                           const std::vector<Product>& products,
                                           const std::string& exportPath) {
    struct Stat {
        std::string name;
        double totalAmount = 0;
        int frequency = 0;
    };

    std::map<int, Product> productMap;
    for (const auto& p : products) {
        productMap[p.id] = p;
    }

    std::map<std::string, std::map<std::string, Stat>> stats;

    for (const auto& order : orders) {
        for (const auto& item : order.items) {
            int pid = item.productId;
            int qty = item.quantity;
            double unitPrice = item.unitPrice;

            if (productMap.count(pid)) {
                const Product& p = productMap[pid];
                auto& stat = stats[p.category][p.name];
                stat.name = p.name;
                stat.totalAmount += unitPrice * qty;
                stat.frequency += qty;
            }
        }
    }

    std::ostream* out = &std::cout;
    std::ofstream fout;

    if (!exportPath.empty()) {
        fout.open(exportPath);
        if (!fout.is_open()) {
            std::cerr << "无法写入统计文件：" << exportPath << "\n";
        } else {
            out = &fout;
        }
    }

    *out << "\n📊 商品购买统计分析（按类别）：" << std::endl;
    *out << std::left << std::setw(15) << "类别"
         << std::setw(15) << "商品"
         << std::setw(15) << "总金额"
         << std::setw(10) << "购买频度" << "\n";

    for (const auto& [category, prodStats] : stats) {
        for (const auto& [name, stat] : prodStats) {
            *out << std::left << std::setw(15) << category
                 << std::setw(15) << stat.name
                 << "￥" << std::setw(13) << std::fixed << std::setprecision(2) << stat.totalAmount
                 << std::setw(10) << stat.frequency << "\n";
        }
    }

    if (fout.is_open()) {
        fout.close();
        std::cout << "✅ 统计结果已保存至 " << exportPath << "\n";
    }
}

