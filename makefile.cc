
#include "makefile.h"
#include <iostream>
#include <string>

using namespace std;


// Helper Methods

/*
 Returns the index of the target object whose name matches with the string passed in the parameter. Returns -1 if not found.

 @param list The array of Target objects.
 @param size The size of the filled up array.
 @param matchString The string to match.
 @return The index in the array.
 */
int getTargetIndexFromList(Target *list[maxTargets], int size, const string &matchString) {
    
    for (int i = 0; i < size; i++) {
        if (list[i]->name.compare(matchString) == 0) {
            return i;
        }
    }
    
    return -1;
}

/**
 Builds the passed Target object pointed to by the pointer. If successful, it builds all the dependency files in the chain and updates their build time to the latest time of the system.
 
 @note: This is a recursive method to access the depth-first leaf.
 
 @param targ The pointer to the target object.
 @param oldTime The lastUpdated time of the target.
 @return Returns true if successfully build, else false if there was no requirement to build the file.
 */
bool Makefile::buildTarget(Target *targ, int oldTime) {
    
    bool shouldBuild = false;
    bool loopedBuild = false;   // Tracks if a build was succesfull during a loop run of the dependency list.
    
    if (targ->getDependencyCount() == 0) {
        return false;
    }
    
    for (int i = 0; i < targ->getDependencyCount(); i++) {
        // Loop through the dependency list for the target.
        
        Target *depend = targ->dependList[i];
        
        if (oldTime < depend->lastUpdatedTime) {
            // The dependency file was updated, just return.
            return true;
        }
        
        shouldBuild = buildTarget(depend, oldTime);
        
        if (shouldBuild) {
            // If it required a build, we note that the loop successfully required a build before moving on to another loop cycle.
            loopedBuild = true;
        }
        
        if (shouldBuild) {
            out << "Building " << depend->name << endl;
            depend->update(newTime);
        }
    }
    
    if (loopedBuild) {
        // After traversing all the loops, if any of them required a build, we continue traversing back the list in order to build the remaining files. Hence, mark shouldBuild as true irrespective of the result of last loop run.
        shouldBuild = true;
    }
    
    return shouldBuild;
}


// Makefile Methods

/*
 Makefile constructor that initilizes the object with default values.
 */
Makefile::Makefile(ostream &out) : out{out} {
    
    targetListSize = 0;
    newTime = 0;
}

/*
 Makefile destructor that destructs the used memory by the object (eg. targetList).
 */
Makefile::~Makefile() {
    
    for (int i = 0; i < targetListSize; i++) {
        delete targetList[i];
    }
}

/**
 This method adds the dependecy input to the Makefile's targetList accordingly.

 @param t The target file name.
 @param d The dependency file name.
 */
void Makefile::addDepend(const string &t, const string &d) {
    
    // Get indexes. It will be -1 if the files are not already present.
    int targetIndex = getTargetIndexFromList(targetList, targetListSize, t);
    int dependIndex = getTargetIndexFromList(targetList, targetListSize, d);
    
    if ((targetIndex == -1 || dependIndex == -1) && (targetListSize == maxTargets)) {
        
        // If any of the files to be added are new ones AND max file limit of 20 has been
        // rached, then output the error and return.
        out << "Max targets exceeded" << endl;
        return;
    }
    
    if (targetIndex != -1 && dependIndex != -1) {
        
        // If target and depend both exists.
        Target *targ = targetList[targetIndex];
        Target *depend = targetList[dependIndex];
        
        targ->addDepend(depend);
    }
    else if (targetIndex != -1) {
        
        // If only target exists but dependecy is new. Then just add dependency.
        Target *targ = targetList[targetIndex];
        Target *depend = new Target(d);
        
        targ->addDepend(depend);
        
        targetList[targetListSize] = depend;
        ++targetListSize;
    }
    else if (dependIndex != -1) {
        
        // If the dependency exists but the target is new, then just add the target.
        Target *depend = targetList[dependIndex];
        Target *targ = new Target(t);
        
        targ->addDepend(depend);
        
        targetList[targetListSize] = targ;
        ++targetListSize;
    }
    else if (targetIndex == -1 && dependIndex == -1) {
        
        // If both the target AND the dependecy doesn't exist in the list, then add both of them.
        Target *targ = new Target(t);
        Target *depend = new Target(d);
        
        targ->addDepend(depend);
        
        targetList[targetListSize] = targ;
        targetList[targetListSize+1] = depend;
        targetListSize += 2;
    }
}

void Makefile::changeTarget(const string &t) {
    
    // Update the current time, irrespective of success or not later.
    ++newTime;
    
    int index = getTargetIndexFromList(targetList, targetListSize, t);
    
    if (index == -1) {
        // If file not found, return.
        return;
    }
    
    Target *targ = targetList[index];
    
    if (targ->isLeaf() == false) {
        out << "Cannot update non-leaf object" << endl;
    } else {
        // Update the time of the target object to the current newTime of the system.
        targ->update(newTime);
        out << targ->name << " updated at time " << targ->lastUpdated() << endl;
    }
}

/**
 Build the target with the given string name and handles building of other files in its dependency chain as necessary.

 @param t The name of the target to be built.
 */
void Makefile::rebuild(const string &t) {
    
    int index = getTargetIndexFromList(targetList, targetListSize, t);
    
    if (index == -1) {
        // If file not found, return.
        return;
    }
    
    Target *targ = targetList[index];
    bool shouldBuild = buildTarget(targ, targ->lastUpdated());
    
    if (shouldBuild) {
        out << "Building " << targ->name << endl;
        targ->update(newTime);
    }
}

