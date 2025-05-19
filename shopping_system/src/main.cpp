#include <iostream>
#include <windows.h>
#include "user_manager.h"
#include "product_manager.h"
#include "product_query.h"
#include "shopping_cart.h"
#include "order_manager.h"
#include <thread>  
#include "full_reduction_rule.h"
#include "statistics_manager.h"

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::string user, pass;
    UserManager userManager;
    ProductManager productManager;
    OrderManager orderManager;
    ShoppingCart cart;

    userManager.loadUsers();
    productManager.loadProducts();
    orderManager.loadOrders();
    ProductQuery productQuery(productManager, orderManager, user);
    while (true) { 
while (true) {
    std::cout << "请输入用户名：";
    std::cin >> user;

    if (!userManager.userExists(user)) {
        std::cout << "用户不存在，自动进入注册流程。\n";
        std::cout << "请输入注册密码：";
        std::cin >> pass;
        if (userManager.registerUser(user, pass)) {
            std::cout << "注册成功，请重新登录。\n";
        } else {
            std::cout << "注册失败，该用户名可能已存在。\n";
        }
        continue; 
    }
        std::cout << "请输入密码：";
        std::cin >> pass;

        if (userManager.loginUser(user, pass)) {
            cart.loadFromFile("data/carts.txt", user);
            break;
        } 
        else {
            std::cout << "密码错误，请重新登录。\n";
        }
    }

        int option;
        do {
            std::cout << "\n===== 欢迎 " << user << " 登录 =====\n";
            std::cout << "1. 浏览商品\n";
            std::cout << "2. 精确查询商品\n";
            std::cout << "3. 添加商品到购物车\n";
            std::cout << "4. 查看购物车\n";
            std::cout << "5. 添加商品（仅管理员）\n";
            std::cout << "6. 修改密码\n";
            std::cout << "7. 修改购物车商品数量\n";
            std::cout << "8. 删除购物车商品\n";
            std::cout << "9. 提交订单\n";
            std::cout << "10. 查询订单\n";
            std::cout << "11. 修改订单状态（仅管理员）\n";
            std::cout << "12. 用户取消或修改订单\n";
            std::cout << "13. 设置促销活动（仅管理员）\n";
            std::cout << "14. 设置全场满减活动（仅管理员）\n";
            std::cout << "15. 查看购买统计分析（仅管理员）\n";
            std::cout << "0. 退出登录\n";
            std::cout << "请选择操作：";
            std::cin >> option;

            switch (option) {
                case 1:
                productManager.listProducts();
                break;
                case 2: {
                    std::string kw;
                    std::cout << "输入商品关键词：";
                    std::cin >> kw;
                    productQuery.searchProductByKeyword(kw);
                    break;
                }
                case 3: {
                    int id, qty;
                    std::cout << "输入商品ID和数量：";
                    std::cin >> id >> qty;
                    cart.addToCart(productManager.getProductById(id), qty);
                    cart.saveToFile("data/carts.txt", user);
                    break;
                }
                case 4: {
                    auto items = cart.getItems();
                    if (items.empty()) {
                        std::cout << "购物车为空。\n";
                        break;
                    }
                
                    double total = 0.0;
                    std::cout << "您的购物车：\n";
                    for (const auto& item : items) {
                        Product p = productManager.getProductById(item.first);
                        std::cout << "商品ID: " << p.id << " 名称: " << p.name;
                        bool promoActive = (p.hasPromotion && isPromotionActive(p.promotion));
                        if (promoActive) {
                            std::cout << " 【" << p.promotion.label << "】";
                        }
                    
                        double unitPrice = p.price;
                        if (promoActive && p.promotion.type == PromotionType::Discount) {
                            unitPrice *= p.promotion.discountRate;
                        }
                    
                        double subtotal = unitPrice * item.second;
                        total += subtotal;
                    
                        std::cout << "\n原价: ￥" << p.price
                                  << " 折后单价: ￥" << unitPrice
                                  << " 数量: " << item.second
                                  << " 小计: ￥" << subtotal << "\n";
                    }
                    double finalTotal = total;
                    if (orderManager.fullReduction.threshold > 0 &&
                        total >= orderManager.fullReduction.threshold &&
                        isFullReductionActive(orderManager.fullReduction)) {
                        std::cout << "🎉 满减活动生效：" << orderManager.fullReduction.label << "\n";
                        finalTotal -= orderManager.fullReduction.reduction;
                    }
                    std::cout << "总计金额（满减前）：￥" << total << "\n";
                    std::cout << "应付金额（满减后）：￥" << finalTotal << "\n";
                    break;
                }                
                case 5:
                    if (userManager.isAdmin(user))
                        productManager.addProduct();
                    else
                        std::cout << "无权限，只有管理员可以添加商品。\n";
                    break;
                case 6:
                    userManager.changePassword(user); break;
                case 7: {
                    int id, newQty;
                    std::cout << "输入商品ID和新的数量（0表示删除）：";
                    std::cin >> id >> newQty;
                    if (newQty == 0) {
                        std::cout << "确认删除该商品？(y/n)：";
                        char confirm;
                        std::cin >> confirm;
                        if (confirm == 'y') cart.removeFromCart(id);
                        else std::cout << "取消删除。\n";
                    } else {
                        cart.updateQuantity(id, newQty);
                    }
                    cart.saveToFile("data/carts.txt", user);
                    break;
                }
                case 8: {
                    std::cout << "输入要删除的商品ID（可输入多个，以空格分隔，输入0结束）：\n";
                    std::vector<int> ids;
                    int id;
                    while (std::cin >> id && id != 0) {
                        ids.push_back(id);
                    }
                    for (int pid : ids) {
                        cart.removeFromCart(pid);
                    }
                    std::cout << "已删除 " << ids.size() << " 件商品。\n";
                    cart.saveToFile("data/carts.txt", user);
                    break;
                }
                case 9: {
                    std::string addr;
                    std::cout << "输入收货地址：";
                    std::cin.ignore(); 
                    std::getline(std::cin, addr);
                    int orderId = orderManager.generateOrder(cart, productManager, addr, user);
                    if (orderId != -1) {
                        std::thread t(&OrderManager::autoUpdateOrderStatus, &orderManager);
                        t.detach();
                    }
                    break;
                }
                case 10:
                    orderManager.queryOrders(user, userManager.isAdmin(user));break;
                case 11:
                    if (userManager.isAdmin(user)) {
                        int oid;
                        std::cout << "输入订单号：";
                        std::cin >> oid;
                        orderManager.updateOrderStatus(oid);
                    } else {
                        std::cout << "无权限，仅管理员可修改订单状态。\n";
                    }
                    break;
                case 12: {
                    int oid;
                    std::cout << "输入订单号：";
                    std::cin >> oid;
                    orderManager.modifyOrder(oid, productManager);
                    break;
                }
                case 13: {
                    if (!userManager.isAdmin(user)) {
                        std::cout << "仅管理员可设置促销活动。\n";
                        break;
                    }
                    int pid;
                    std::cout << "请输入要设置促销的商品ID：";
                    std::cin >> pid;
                    productManager.setPromotionForProduct(pid);
                    break;
                }       
                case 14: {
                    if (!userManager.isAdmin(user)) {
                        std::cout << "仅管理员可设置全场满减活动。\n";
                        break;
                    }
                    double threshold, reduction;
                    std::cout << "请输入满减门槛金额：";
                    std::cin >> threshold;
                    std::cout << "请输入减免金额：";
                    std::cin >> reduction;
                
                    orderManager.fullReduction.threshold = threshold;
                    orderManager.fullReduction.reduction = reduction;
                    orderManager.fullReduction.label = "满" + std::to_string((int)threshold) + "减" + std::to_string((int)reduction);
                
                    std::cout << "请输入开始时间（格式：2025-05-12 00:00:00）：";
                    std::cin.ignore();
                    std::getline(std::cin, orderManager.fullReduction.startTime);
                    std::cout << "请输入结束时间（格式：2025-05-31 23:59:59）：";
                    std::getline(std::cin, orderManager.fullReduction.endTime);

                    std::cout << "设置成功，全场满减活动：" << orderManager.fullReduction.label << "\n";
                    break;
                }   
                case 15: {
                    if (!userManager.isAdmin(user)) {
                        std::cout << "无权限，仅管理员可查看统计分析。\n";
                        break;
                    }
                    StatisticsManager statsMgr;
                    statsMgr.generateStatistics(orderManager.getAllOrders(), productManager.getAllProducts(),
                                                "data/stats.txt");  
                    break;
                }
                case 16: {
                    if (!userManager.isAdmin(user)) {
                        std::cout << "无权限，仅管理员可导出图表。\n";
                        break;
                    }
                    StatisticsManager statsMgr;
                    statsMgr.exportChartHTML(orderManager.getAllOrders(), productManager.getAllProducts(), "data/stats.html");
                    break;
                }                                                               
                case 0:
                    std::cout << "已退出当前账户，返回登录页。\n";
                    break;
                default:
                    std::cout << "无效输入，请重新选择。\n";
                    break;
            }

        } while (option != 0);
        userManager.saveUsers();
        productManager.saveProducts();
        orderManager.saveOrders();  
    }

    return 0;
}
