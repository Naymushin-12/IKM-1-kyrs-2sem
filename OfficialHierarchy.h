#ifndef OFFICIALHIERARCHY_H
#define OFFICIALHIERARCHY_H

#include <vector>
#include <string>

using namespace std;

class OfficialNode {
private:
    int id;
    int bribeAmount;
    OfficialNode* boss;
    vector<OfficialNode*> subordinates;

public:
    OfficialNode(int officialId, int bribe);
    
    void setBoss(OfficialNode* officialBoss);
    void addSubordinate(OfficialNode* subordinate);
    int getId() const;
    int getBribe() const;
    OfficialNode* getBoss() const;
    const vector<OfficialNode*>& getSubordinates() const;
};

class OfficialHierarchy {
private:
    OfficialNode* root;
    vector<OfficialNode*> officials;

    OfficialNode* findNodeById(int id) const;
    void calculateMinBribe(OfficialNode* node, int& totalBribe, 
        vector<int>& path) const;
    void printPath(const vector<int>& path) const;

public:
    OfficialHierarchy();
    ~OfficialHierarchy();
    
    void buildHierarchy();
    void findMinBribeSolution();
    void clearHierarchy();
};

#endif