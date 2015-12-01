//
//  KarkkainenSanders.hpp
//  Karkkainen-Sanders
//
//  Created by Maxim Grishkin on 29/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

#ifndef KarkkainenSanders_hpp
#define KarkkainenSanders_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <numeric>

typedef int int32;
typedef long long int64;
typedef unsigned int uint32;
typedef unsigned long long uint64;

namespace KarkkainenSanders {
    
    uint32 getByIndex(const std::vector<uint32> &array, uint32 position) {
        return position < array.size() ? array[position] : 0;
    }
    
    bool equalSubsegments(const std::vector<uint32> &array, uint32 firstSubsegment, uint32 secondSubsegment, uint32 size) {
        for (uint32 offset = 0; offset < size; ++offset)
            if (getByIndex(array, firstSubsegment + offset) != getByIndex(array, secondSubsegment + offset))
                return false;
        return true;
    }
    
    bool lowerSubsegment(const std::vector<uint32> &array, uint32 firstSubsegment, uint32 secondSubsegment, uint32 size) {
        uint32 offset;
        for (offset = 0;
             offset < size && getByIndex(array, firstSubsegment + offset) == getByIndex(array, secondSubsegment + offset);
             ++offset)
            ;
        return offset < size && getByIndex(array, firstSubsegment + offset) < getByIndex(array, secondSubsegment + offset);
    }
    
    uint32 getMax(const std::vector<uint32> &array) {
        uint32 maxValue = 0;
        for (auto value: array)
            maxValue = maxValue < value ? value : maxValue;
        return maxValue;
    }
    
    std::vector<uint32> suffixRadixPass(const std::vector<uint32> &array, std::vector<uint32> &suffixes, uint32 offset) {
        std::vector<uint32> result(suffixes.size());
        std::vector<uint32> counts(getMax(array) + 1, 0);
        for (auto suffix: suffixes)
            ++counts[getByIndex(array, suffix + offset)];
        for (uint32 index = 0, sum = 0; index < counts.size(); ++index) {
            std::swap(counts[index], sum);
            sum += counts[index];
        }
        for (auto suffix: suffixes)
            result[counts[getByIndex(array, suffix + offset)]++] = suffix;
        return result;
    }

    void suffixRadixSort(const std::vector<uint32> &array, std::vector<uint32> &suffixes, uint32 suffixSize) {
        for (uint32 offset = 1; offset <= suffixSize; ++offset)
            suffixes = suffixRadixPass(array, suffixes, suffixSize - offset);
    }
    
    uint32 getPositionIn12(uint32 arraySize, uint32 suffix) {
        uint32 position = (suffix - (suffix + 2) / 3) / 2;
        position += suffix % 3 == 2 ? (arraySize + 1) / 3 : 0;
        return position;
    }
    
    std::vector<uint32> getSuffixes12(const std::vector<uint32> &array) {
        std::vector<uint32> suffixes;
        for (uint32 suffix = 0; suffix < array.size(); ++suffix)
            if (suffix % 3)
                suffixes.push_back(suffix);
        suffixRadixSort(array, suffixes, 3);
        std::vector<uint32> sortedNames(suffixes.size(), 1), names(suffixes.size());
        for (uint32 index = 1; index < suffixes.size(); ++index)
            sortedNames[index] = sortedNames[index - 1] + (int)(!equalSubsegments(array, suffixes[index - 1], suffixes[index], 3));
        for (uint32 index = 0; index < suffixes.size(); ++index)
            names[getPositionIn12((uint32)array.size(), suffixes[index])] = sortedNames[index];
        return names;
    }
    
    bool lower(const std::vector<uint32> &array, uint32 suffix0, uint32 suffix12, const std::vector<uint32> &reversedSuffixArray12) {
        uint32 offset = suffix12 % 3;
        if (equalSubsegments(array, suffix0, suffix12, offset)) {
            uint32 positionIn12For0, positionIn12For12;
            positionIn12For0 = getPositionIn12((uint32)array.size(), suffix0 + offset);
            positionIn12For12 = getPositionIn12((uint32)array.size(), suffix12 + offset);
            return getByIndex(reversedSuffixArray12, positionIn12For0) < getByIndex(reversedSuffixArray12, positionIn12For12);
        }
        return lowerSubsegment(array, suffix0, suffix12, offset);
    }

    std::vector<uint32> getSuffixArray(const std::vector<uint32> &array) {
        std::vector<uint32> suffixes12 = getSuffixes12(array);
        std::vector<uint32> suffixArray12;
        if (getMax(suffixes12) == suffixes12.size()) {
            suffixArray12.resize(suffixes12.size());
            for (uint32 suffix = 0; suffix < suffixes12.size(); ++suffix)
                suffixArray12[suffixes12[suffix] - 1] = suffix;
        }
        else
            suffixArray12 = getSuffixArray(suffixes12);
        
        std::vector<uint32> reversedSuffixArray12(suffixArray12.size());
        for (uint32 index = 0; index < suffixArray12.size(); ++index)
            reversedSuffixArray12[suffixArray12[index]] = index + 1;
        
        std::vector<uint32> sortedSuffixes12(suffixes12.size());
        for (uint32 index = 0; index < array.size(); ++index) {
            if (index % 3) {
                uint32 positionIn12 = getPositionIn12((uint32)array.size(), index);
                sortedSuffixes12[reversedSuffixArray12[positionIn12] - 1] = index;
            }
        }
        
        std::vector<uint32> forRadix0(array), sortedSuffixes0;
        for (uint32 index = 0; index < array.size(); ++index) {
            if (index % 3)
                forRadix0[index] = reversedSuffixArray12[getPositionIn12((uint32)array.size(), index)];
            else
                sortedSuffixes0.push_back(index);
        }
        suffixRadixSort(forRadix0, sortedSuffixes0, 2);
        
        std::vector<uint32> suffixArray;
        uint32 index0, index12, suffix0, suffix12;
        for (index0 = 0, index12 = 0;
             index0 < sortedSuffixes0.size() && index12 < sortedSuffixes12.size();
             ) {
            suffix0 = sortedSuffixes0[index0];
            suffix12 = sortedSuffixes12[index12];
            if (lower(array, suffix0, suffix12, reversedSuffixArray12)) {
                suffixArray.push_back(suffix0);
                ++index0;
            }
            else {
                suffixArray.push_back(suffix12);
                ++index12;
            }
        }
        for (; index0 < sortedSuffixes0.size(); ++index0)
            suffixArray.push_back(sortedSuffixes0[index0]);
        for (; index12 < sortedSuffixes12.size(); ++index12)
            suffixArray.push_back(sortedSuffixes12[index12]);
        
        return suffixArray;
    }
    
    std::vector<uint32> getSuffixArray(const std::string &str) {
        std::vector<uint32> intString(str.size());
        for (uint32 index = 0; index < str.size(); ++index)
            intString[index] = (int)str[index];
        return getSuffixArray(intString);
    }

}
#endif /* KarkkainenSanders_hpp */
