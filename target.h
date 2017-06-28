#ifndef _TARGET_H_
#define _TARGET_H_
#include <iostream>
#include <string>

const int maxDepends = 10;

struct Target {
    
    std::ostream &out;  // Stream where Target will print its output
    
    // Name of the target.
    std::string name;
    
    // The array of target file pointers on which this is dependent on.
    // eg. a: b     a: c      a: d, then this list will have b, c, d
    Target *dependList[maxDepends];
    
    // The time at which the file was last updated.
    int lastUpdatedTime;
    
    // The counter for the number of dependency files on this object.
    int dependSize;
    
    // Boolean value for if the target is leaf or not.
    bool isTargetLeaf;
    
    // Constructor
    Target(std::ostream &out = std::cout);
    Target(const std::string &name, std::ostream &out = std::cout);
    Target &operator=(const Target &other);
    
    // Methods
    bool isLeaf() const;
    int getDependencyCount() const;
    void addDepend(Target *d);
    void update(int time);
    int lastUpdated() const;
    void build();
};

#endif
