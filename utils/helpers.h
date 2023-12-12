#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <climits>

/* Helper functions */


template <typename T>
// Template not needed. Just to keep definition in header
std::string uCharToBits(const unsigned char* pChar) {
    std::string output;
    unsigned char mask = 128;

    for (int i = 7; i >= 0; --i) {
        if ((*pChar & mask) == 0) output += '0';
        else output += '1';
        mask >>= 1;
    }
    return output;
}

template <typename T>
std::string renderBits(const T& input, bool removeTrailingZeros = false) {
    std::string output;
    int length = sizeof(T);
    int lastPos = length - 1;

    const T* pInput = &input;
    auto pArray = reinterpret_cast<const unsigned char*>(pInput);
    pArray += lastPos;

    for (int i = lastPos; i >= 0; --i) {
        if (i != lastPos) output += ' ';
        output += uCharToBits<T>(pArray);
        --pArray;
    }

    if (removeTrailingZeros) {
        int counter = 0;
        while (output[counter] == '0' || output[counter] == ' ') ++counter;
        output.erase(0, counter);
    }

    return output;
}

template<typename T>
void printVector(const std::vector<T>& V, int maxN = 20, const char* delimeter = " ") {
    for (int i = 0; i < V.size(); ++i) {
        if (i >= maxN) {
            std::cout << "...";
            break;
        }
        std::cout << V[i];
        if (i != V.size() - 1)
            std::cout << delimeter;
    }
    std::cout << '\n';
}

template<typename T>
void print2dVector(const std::vector<std::vector<T>>& V, int maxN = 20, const char* delimeter = " ") {
    for (int i = 0; i < V.size(); ++i) {
        if (i >= maxN) {
            std::cout << "...";
            break;
        }
        printVector<T>(V[i], maxN, delimeter);
    }
    std::cout << '\n';
}

template<typename>
int random(int lowerBound = INT_MIN, int upperBound = INT_MAX)
{
	static std::random_device dev;
	static std::mt19937 rng(dev());
	static std::uniform_int_distribution<std::mt19937::result_type> dist6(lowerBound, upperBound);

	return dist6(rng);
}