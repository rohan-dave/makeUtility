
#include "target.h"
#include <string>

using namespace std;


// Helper Methods

/**
 Returns the index of the target with matching stringName in the Target list of pointers to Target objects. Returns -1 if not found.

 @param list The list of target pointers.
 @param size The size of filled array.
 @param matchString The matching string name to search for.
 @return The index in the array.
 */
int getDependIndexFromList(Target *list[maxDepends], int size, const string &matchString) {
    
    for (int i = 0; i < size; i++) {
        if (list[i]->name.compare(matchString) == 0) {
            return i;
        }
    }
    
    return -1;
}


// Target Methods

// Default Constructor
Target::Target(ostream &out) : out{out} {}

// Custom Constructor
Target::Target(const string &name, ostream &out) : out{out}, name{name} {
    
    this->name = name;
    this->dependSize = 0;
    this->lastUpdatedTime = 0;
    this->isTargetLeaf = true;
}

/**
 Returns a boolean value indicating if the target object is a leaf or not.

 @return true - if it is a leaf, false - otherwise.
 */
bool Target::isLeaf() const {
    
  return isTargetLeaf;
}

/**
 Adds the target to the dependencyList.

 @param d The pointer to the target object.
 */
void Target::addDepend(Target *d) {
    
    if (dependSize == maxDepends) {
        // If max dependency size is reached, then return.
        out << "Max dependencies exceeded" << endl;
        return;
    }
    
    int index = getDependIndexFromList(dependList, dependSize, d->name);
    if (index != -1) {
        // If the dependency is already in the list, then don't do anything and return.
        return;
    }
    
    dependList[dependSize] = d;
    ++dependSize;
    
    // This Target is no longer a leaf, as it now has a dependency added.
    this->isTargetLeaf = false;
}

/**
 Updates the time of target with the new time passed in.

 @param time The new time.
 */
void Target::update(int time) {
    
    lastUpdatedTime = time;
}

/**
 Returns the last updated time of the Target.

 @return The last updated time.
 */
int Target::lastUpdated() const {
    
    return lastUpdatedTime;
}

int Target::getDependencyCount() const {
    
    return dependSize;
}

/**
 Builds the target. Not needed currently as this is handled in makefile.cc
 */
void Target::build() {}
