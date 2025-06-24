#ifndef OFFICIALHIERARCHY_H
#define OFFICIALHIERARCHY_H

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

class OfficialNode {
public:
    OfficialNode(int id, int bribe);
    
    void setBoss(OfficialNode* boss);
    void addSubordinate(OfficialNode* subordinate);
    
    int getId() const;
    int getBribe() const;
    const std::vector<OfficialNode*>& getSubordinates() const;
    
private:
    int id;
    int bribeAmount;
    std::vector<OfficialNode*> subordinates;
};

class OfficialHierarchy {
public:
    OfficialHierarchy();
    ~OfficialHierarchy();
    
    void buildFromConsole();
    void findOptimalBribePath() const;
    
private:
    std::unordered_map<int, std::unique_ptr<OfficialNode>> officials;
    OfficialNode* root = nullptr;
    
    void clear();
    OfficialNode* findNode(int id) const;
    void findAllPaths(OfficialNode* node, 
                     std::vector<int>& currentPath,
                     std::vector<std::vector<int>>& allPaths,
                     int currentSum) const;
    void printResults(const std::vector<int>& bestPath, int minBribe) const;
};
#endif
