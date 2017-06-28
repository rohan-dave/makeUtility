
#ifndef _MAKEFILE_
#define _MAKEFILE_
#include <string>
#include <iostream>
#include "target.h"

const int maxTargets = 20;

struct Makefile {
    
    /*
     Stream where Makefile will print its output.
     This class should output a value x by saying out << x, rather than cout << x.
     */
    std::ostream &out;

    // An array of pointers to Target objects currently. When we encounter "a: b" we add both targets a and b to this list
    Target *targetList[maxTargets];
    
    // The number of unique files added to the list currently.
    int targetListSize;
    
    // The updated counter after a "touch" command setting the current time.
    int newTime;
    
    // Constructor and destructor
    Makefile(std::ostream &out = std::cout);
    ~Makefile();

    // Methods
    void addDepend(const std::string &t, const std::string &d);
    void changeTarget(const std::string &t);
    void rebuild(const std::string &t);
    bool buildTarget(Target *targ, int oldTime);
};

#endif
