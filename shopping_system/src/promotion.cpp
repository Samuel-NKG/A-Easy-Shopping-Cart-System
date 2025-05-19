#include "promotion.h"
#include <ctime>
#include <sstream>
#include <iomanip>

bool isPromotionActive(const Promotion& promo) {
    auto parse = [](const std::string& str) {
        std::tm tm = {};
        std::istringstream ss(str);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        return std::mktime(&tm);
    };

    std::time_t now = std::time(nullptr);
    std::time_t start = parse(promo.startTime);
    std::time_t end = parse(promo.endTime);

    return now >= start && now <= end;
}