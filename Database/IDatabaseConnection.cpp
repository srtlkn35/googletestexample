#include "IDatabaseConnection.hpp"
#include <iostream>

IDatabaseConnection::IDatabaseConnection(std::string serverAddress)
{
    mServerAddress = serverAddress;
}

void IDatabaseConnection::connect()
{
    std::cout << "Connecting to database server " << mServerAddress << "\n";
    onConnect();
}

void IDatabaseConnection::disconnect()
{
    std::cout << "Disconnecting from database\n";
    //close the real connection
}
