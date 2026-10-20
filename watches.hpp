#ifndef WATCHES_HPP

#define WATCHES_HPP

#include <string>
#include <vector>

struct Watch {
    std::string name;
    std::string brand;
    double price;
    int year;
    std::string movement;
};

class Collection {
public:
    Collection(const std::string& ownerName);
    ~Collection() = default;
    void changeName(const std::string& newName);
    void addWatch(const Watch& watch);
    bool removeWatch(const std::string& watchName);
    void displayCollection() const;
    std::vector<Watch>::size_type getCount() const;
    const std::vector<Watch>& getWatches() const;
private:
    std::string owner;
    std::vector<Watch> watches;
    double totalValue;
};


#endif // WATCHES_HPP
