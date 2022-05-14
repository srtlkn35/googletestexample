#include <string>
#include <stdexcept>
#include "Account.hpp"

namespace accountlibnamespace 
{
    Account::Account()
    {
        this->mBalance = 0;
    }

    void Account::deposit(double sum)
    {
        mBalance += sum;
    }

    void Account::withdraw(double sum)
    {
        if (mBalance < sum)
        {
            throw std::runtime_error("Insufficient funds!!!");
        }
        mBalance -= sum;
    }

    double Account::getBalance() const
    {
        return mBalance;
    }

    void Account::transfer(Account& to, double sum)
    {
        withdraw(sum);  //Will throw an error if mBalance not enough
        to.deposit(sum);
    }
}
