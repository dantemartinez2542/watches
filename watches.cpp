#include "watches.hpp"
#include <iostream>

Collection::Collection(const std::string& ownerName): owner(ownerName), totalValue(0.0) {}

//Collection::~Collection() = default;

void Collection::changeName(const std::string& newName) {
    owner = newName;
}

void Collection::addWatch(const Watch& watch) {
    watches.push_back(watch);
    totalValue += watch.price;
}

bool Collection::removeWatch(const std::string& watchName) {
    for (auto it = watches.begin(); it != watches.end(); ++it) {
        if (it->name == watchName) {
            totalValue -= it->price;
            watches.erase(it);
            return true;
        }
    }
    return false;
}

void Collection::displayCollection() const {
    std::cout << "Owner: " << owner << std::endl;
    std::cout << "Total Watches: " << getCount() << std::endl;
    std::cout << "Total Value: $" << totalValue << std::endl;
    for (const auto& watch : watches) {
        std::cout << "Name: " << watch.name
                  << ", Brand: " << watch.brand
                  << ", Price: $" << watch.price
                  << ", Year: " << watch.year
                  << ", Movement: " << watch.movement
                  << std::endl;
    }
}

std::vector<Watch>::size_type Collection::getCount() const {
    return watches.size();
}

const std::vector<Watch>& Collection::getWatches() const {
    return watches;
}
