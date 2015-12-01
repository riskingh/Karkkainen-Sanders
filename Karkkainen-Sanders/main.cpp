//
//  main.cpp
//  Karkkainen-Sanders
//
//  Created by Maxim Grishkin on 29/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "KarkkainenSanders.hpp"

int main(int argc, const char * argv[]) {
    freopen("input.txt", "r", stdin);
    std::string s;
    std::cin >> s;
    std::vector<uint32> v = KarkkainenSanders::getSuffixArray(s);
    for (auto a: v) {
        std::cout << a << " ";
    }
    std::cout << "\n";
//
//    std::vector<uint32> array = {1, 4, 2, 3, 5};
//    std::vector<uint32> suffixes = {3, 0, 1, 2};
//    
//    KarkkainenSanders::suffixRadixSort(array, suffixes, 2);
//    
//    for (auto suffix: suffixes)
//        std::cout << suffix << " " << array[suffix] << " " << KarkkainenSanders::getByIndex(array, suffix + 1) << "\n";
//    std::cout << "\n";
    return 0;
}
