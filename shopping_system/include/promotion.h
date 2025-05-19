#ifndef PROMOTION_H
#define PROMOTION_H
#include <string>

enum class PromotionType {
    Discount,
    FullReduction
};
struct Promotion {
    PromotionType type;
    double discountRate;
    double threshold;//门槛
    double reduction;//减多少钱
    std::string label;
    std::string startTime;
    std::string endTime;
};
bool isPromotionActive(const Promotion& promo); 

#endif
