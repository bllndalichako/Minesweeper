#pragma once
#include <random>
using namespace std;

class Random {
    static mt19937 random;

public:
    static int RandomInt(int min, int max);
    static float RandomFloat(float min, float max);
};