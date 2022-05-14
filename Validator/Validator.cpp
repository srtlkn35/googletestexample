#include <stdexcept>
#include <algorithm> 
#include "Validator.hpp"

namespace validatorlibnamespace
{
    Validator::Validator(int low, int high)
    {
        this->mLow = low;
        this->mHigh = high;
    }

    bool Validator::inRange(int valueToTest)
    {
        return ((valueToTest >= std::min(mLow, mHigh)) && (valueToTest <= std::max(mLow, mHigh)));
    }
}