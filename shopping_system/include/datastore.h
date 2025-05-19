#ifndef DATASTORE_H
#define DATASTORE_H
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "product_manager.h"

class DataStore {
public:
    void loadProducts(std::vector<Product>& products);
    void saveProducts(const std::vector<Product>& products);
    void loadUsers(std::unordered_map<std::string, std::string>& users);
    void saveUsers(const std::unordered_map<std::string, std::string>& users);
};

#endif