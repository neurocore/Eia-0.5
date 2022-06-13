#pragma once
#include <string>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <unordered_map>
#include "engine.h"

#define TEST(name) tests[name] = []()

using namespace eia_v0_5;

using std::cout;
using std::endl;
using std::string;
using std::function;
using std::unordered_map;

class Tests
{
    Engine * E;
    unordered_map<string, function<bool()>> tests;

public:
    Tests(Engine * E) : E(E) { register_tests(); }

    bool run()
    {
        int fails = 0;
        for (auto test : tests)
        {
            cout << "Test \"" << test.first << "\" - ";
            const bool success = test.second();
            if (success)
            {
                cout << "passed" << endl;
            }
            else
            {
                cout << "failed!" << endl;
                ++fails;
            }
        }
        cout << "Fails: " << fails << endl << endl;
        if (fails > 0)
        {
            system("pause");
            return false;
        }

        return true;
    }

private:
    void register_tests();
};
