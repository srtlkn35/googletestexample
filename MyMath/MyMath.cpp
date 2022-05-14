#include <iostream>
#include <cstring>
#include "MyMath.hpp"

namespace mymathlibnamespace 
{
    double mySqrt(double x)
    {
        if (x < 0)
        {
            throw std::runtime_error("Negative argument!!!");
        }
        
        return sqrt(x);
    }

    bool isPositive(int x) 
    {
        return x >= 0;
    }

    int countPositives(std::vector<int> const& inputVector)
    {
        return std::count_if(inputVector.begin(), inputVector.end(), isPositive);
    }

    void toUpper(char *inputString)
    {
        for (unsigned i = 0; i < strlen(inputString); i++)
        {
            inputString[i] = toupper(inputString[i]);
        }
    }
    
    std::vector<int> generateNumbers(int n, int limit)
    {
        std::vector<int> result;
        if (limit <=0)
        {
            throw std::runtime_error("Argument must be > 0");
        }
        for (int i=0; i<n; i++) 
        {
            result.push_back(i % limit);
        }
        return result;
    }
}
