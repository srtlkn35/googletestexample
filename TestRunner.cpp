#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MyMath/MyMath.hpp"
#include "Account/Account.hpp"
#include "Validator/Validator.hpp"
#include "Database/IDatabaseConnection.hpp"
#include "Database/Employee.hpp"
#include "Database/EmployeeManager.hpp"

//##############################################################################
//### MyMathTestCase
//##############################################################################
TEST(MyMathTestCase, Sqrt9) 
{
    ASSERT_EQ(3, mymathlibnamespace::mySqrt(9));
}

TEST(MyMathTestCase, NegativeSqrt) 
{
    ASSERT_ANY_THROW(mymathlibnamespace::mySqrt(-9));
    ASSERT_THROW(mymathlibnamespace::mySqrt(-9), std::runtime_error);
}

TEST(MyMathTestCase, PositiveSqrt) 
{
    ASSERT_NO_THROW(mymathlibnamespace::mySqrt(100));
}

TEST(MyMathTestCase, CountPositives) 
{
    std::vector<int> inputVector{1, -2, 3, -4, 5, -6, -7};

    int count = mymathlibnamespace::countPositives(inputVector);
    
    ASSERT_EQ(3, count);
}

TEST(MyMathTestCase, CountPositivesEmptyVector) 
{
    std::vector<int> inputVector{};

    int count = mymathlibnamespace::countPositives(inputVector);
    
    ASSERT_EQ(0, count);
}

TEST(MyMathTestCase, CountPositivesAllNegativeVector) 
{
    std::vector<int> inputVector{-1, -2, -3, -4, -5, -6, -7};

    int count = mymathlibnamespace::countPositives(inputVector);
    
    ASSERT_EQ(0, count);
}

TEST(MyMathTestCase, ToUpper) 
{
    char inputString[] = "Hello World";

    mymathlibnamespace::toUpper(inputString);

    std::string str(inputString);
    ASSERT_EQ("HELLO WORLD", str);
}

TEST(MyMathTestCase, GenerateNumbersOK)
{
    std::vector<int> v = mymathlibnamespace::generateNumbers(5, 3);

    ASSERT_THAT(v, testing::ElementsAre(0, 1, 2, 0, 1));
}

TEST(MyMathTestCase, GenerateNumbersRangeTest)
{
    std::vector<int> v = mymathlibnamespace::generateNumbers(5, 3);

    ASSERT_THAT(v, testing::Each(testing::AllOf(testing::Ge(0), testing::Lt(3))));
}

//##############################################################################
//### AccountTestFixture
//##############################################################################
class AccountTestFixture: public testing::Test
{
    public:
        AccountTestFixture();
        virtual ~AccountTestFixture();
        void SetUp() override;
        void TearDown() override;
        static void SetUpTestCase();
        static void TearDownTestCase();
    protected:
        accountlibnamespace::Account *account;
};

AccountTestFixture::AccountTestFixture()
{
    //std::cout << "Constructor called \n";
}

AccountTestFixture::~AccountTestFixture()
{
    //std::cout << "Destructor called \n";
}

void AccountTestFixture::SetUp()
{
    //std::cout << "SetUp called \n";
    account = new accountlibnamespace::Account();
    account->deposit(10.5);
}

void AccountTestFixture::TearDown()
{
    //std::cout << "TearDown called \n";
    delete account;
}

void AccountTestFixture::SetUpTestCase()
{
    //std::cout << "SetUpTestCase called \n";
}

void AccountTestFixture::TearDownTestCase()
{
    //std::cout << "TearDownTestCase called \n";
}

TEST(AccountTestCase, TestEmptyAccount)
{
    accountlibnamespace::Account account;

    double balance = account.getBalance();

    ASSERT_EQ(0, balance);
}

TEST_F(AccountTestFixture, TestDeposit)
{
    ASSERT_EQ(10.5, account->getBalance());
}

TEST_F(AccountTestFixture, TestWithdrawOK)
{
    account->withdraw(3);

    ASSERT_EQ(7.5, account->getBalance());
}

TEST_F(AccountTestFixture, TestWithdrawInsufficientFunds)
{    
    ASSERT_THROW(account->withdraw(300), std::runtime_error);
}

TEST_F(AccountTestFixture, TestTransferOK)
{    
    accountlibnamespace::Account to;

    account->transfer(to, 2);

    ASSERT_EQ(8.5, account->getBalance());
    ASSERT_EQ(2, to.getBalance());
}

TEST_F(AccountTestFixture, TestTransferInsufficientFunds)
{    
    accountlibnamespace::Account to;

    ASSERT_THROW(account->transfer(to, 200), std::runtime_error);
}

//##############################################################################
//### ValidatorTestFixture
//##############################################################################
class ValidatorTestFixture: public testing::TestWithParam<std::tuple<int, bool>>
{
    public:
    protected:
        validatorlibnamespace::Validator mValidator{5, 10};
};

TEST_P(ValidatorTestFixture, InRange)
{
    std::tuple<int, bool> tuple = GetParam();
    int param = std::get<0>(tuple);
    bool expectedValue = std::get<1>(tuple);
    
    //std::cout << "Param: " << param << ", ExpectedResult: " << expectedValue << " \n";

    bool isInside = mValidator.inRange(param);

    ASSERT_EQ(expectedValue, isInside);
}

INSTANTIATE_TEST_SUITE_P(InRangeTrue, ValidatorTestFixture, 
    testing::Values(
        std::make_tuple(4, false),
        std::make_tuple(5, true),
        std::make_tuple(6, true),
        std::make_tuple(7, true),
        std::make_tuple(8, true),
        std::make_tuple(9, true),
        std::make_tuple(10, true),
        std::make_tuple(11, false)
    )
);

//##############################################################################
//### SomeMockedTests
//##############################################################################
class SomeClass
{
    public:
        SomeClass() = default;
        virtual void someMethod() {
            //std::cout << "Some Content! \n";
        };
};

class MockedClass : public SomeClass
{
    public:
        MockedClass() = default;
        MOCK_METHOD0(someMethod, void());
};

TEST(SomeMockedTests, TestMock)
{
    MockedClass mc;
    EXPECT_CALL(mc, someMethod()).Times(1);
    mc.someMethod();
}

//##############################################################################
//### DatabaseTestCase
//##############################################################################
class MockDatabaseConnection : public IDatabaseConnection
{
    public:
        MockDatabaseConnection(std::string serverAddress);

        MOCK_METHOD0(connect, void());
        MOCK_METHOD0(disconnect, void());

        MOCK_CONST_METHOD1(getSalary, float(int));
        MOCK_METHOD2(updateSalary, void(int, float) );

        MOCK_CONST_METHOD1(getSalariesRange, std::vector<Employee>(float));
        MOCK_CONST_METHOD2(getSalariesRange, std::vector<Employee>(float, float));
        MOCK_METHOD(void, onConnect, (), (override));

        void someMemberMethod(std::string param)
        {
            //std::cout << "Member Method Called! (Param:" << param << ")\n";
            throw std::runtime_error("dummyError");
        }
};

MockDatabaseConnection::MockDatabaseConnection(std::string serverAddress) : IDatabaseConnection(serverAddress)
{

}

TEST(DatabaseTestCase, TestConnectionError)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::Throw(std::runtime_error("dummyError")));

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

ACTION(myThrow)
{
    //std::cout << "Throwing an error!\n";
    throw std::runtime_error("dummyError");
}

TEST(DatabaseTestCase, TestConnectionErrorAction)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect()).WillOnce(myThrow());

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

void someFreeFunction()
{
    //std::cout << "Free Function!\n";
    throw std::runtime_error("dummyError");
}

TEST(DatabaseTestCase, TestConnectionErrorInvoke)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::Invoke(someFreeFunction));

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

TEST(DatabaseTestCase, TestConnectionErrorInvokeLambda)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::Invoke(
        [](){
            //std::cout << "Lambda Called!\n";
            throw std::runtime_error("dummyError");
        }
    ));

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

TEST(DatabaseTestCase, TestConnectionErrorInvokeMethod)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    auto boundMethod = std::bind(&MockDatabaseConnection::someMemberMethod, &dbConnection, "Some Param");
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::InvokeWithoutArgs(boundMethod));

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

TEST(DatabaseTestCase, TestConnection)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());

    EmployeeManager employeeManager(&dbConnection);
}

TEST(DatabaseTestCase, TestUpdateSalary)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());
    EXPECT_CALL(dbConnection, updateSalary(testing::_, testing::_)).Times(1);   

    EmployeeManager employeeManager(&dbConnection);

    employeeManager.setSalary(50, 6000);
}

TEST(DatabaseTestCase, TestGetSalary)
{
    const int employeeId = 50;
    const float salary = 6100.0;
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());
    EXPECT_CALL(dbConnection, getSalary(testing::_)).Times(1).WillOnce(testing::Return(salary));

    EmployeeManager employeeManager(&dbConnection);

    float returnedSalary = employeeManager.getSalary(employeeId);

    ASSERT_EQ(salary, returnedSalary);
}

TEST(DatabaseTestCase, TestGetSalaryInRange)
{
    const int low = 5000, high = 8000;
    std::vector<Employee> returnedVector{Employee{1, 5600, "John"},
                                    Employee{2, 7000, "Jane"},
                                    Employee{3, 6600, "Alex"}};

    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());
    EXPECT_CALL(dbConnection, getSalariesRange(low, high)).WillOnce(testing::Return(returnedVector));

    EmployeeManager employeeManager(&dbConnection);

    std::map<std::string, float> returnedMap = employeeManager.getSalariesBetween(low, high);

    for(auto it=returnedMap.begin(); it!=returnedMap.end(); ++it)
    {
        //std::cout << it->first << " " << it->second << '\n';
        ASSERT_THAT(it->second, testing::AllOf(testing::Gt(low), testing::Lt(high)));
    }
}

//##############################################################################
//### Test
//##############################################################################
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "DatabaseTestCase";
    ::testing::GTEST_FLAG(output) = "xml:gtest_output.xml";
    return RUN_ALL_TESTS();
}