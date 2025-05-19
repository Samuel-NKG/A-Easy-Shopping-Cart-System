#ifndef FULL_REDUCTION_RULE_H
#define FULL_REDUCTION_RULE_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

// 定义 FullReductionRule 类型
struct FullReductionRule {
    double threshold = 0.0;
    double reduction = 0.0;
    std::string label;
    std::string startTime;
    std::string endTime;
};

// 实现判断是否在有效期内
inline bool isFullReductionActive(const FullReductionRule& rule) {
    if (rule.startTime.empty() || rule.endTime.empty()) return true;

    auto parse = [](const std::string& str) {
        std::tm tm = {};
        std::istringstream ss(str);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        return std::mktime(&tm);
    };

    std::time_t now = std::time(nullptr);
    std::time_t start = parse(rule.startTime);
    std::time_t end = parse(rule.endTime);

    return now >= start && now <= end;
}

#endif
