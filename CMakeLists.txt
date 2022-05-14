cmake_minimum_required(VERSION 3.23)                                        # version can be different

project(googletestexample)                                                  # name of your project

include(FetchContent)                                                       # fetchContent
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0)
FetchContent_GetProperties(googletest)
if (NOT googletest_POPUATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()

set(INLCUDE_DIRECTORIES 
    Account
    Validator
    MyMath
    Database
)

include_directories(${INLCUDE_DIRECTORIES})

add_library(AccountLibrary Account/Account.cpp)                             # add library
add_library(ValidatorLibrary Validator/Validator.cpp)                       # add library
add_library(MyMathrary MyMath/MyMath.cpp)                                   # add library
add_library(DatabaseConnectionLibrary Database/IDatabaseConnection.cpp)     # add library
add_library(EmployeeManagerLibrary Database/EmployeeManager.cpp)            # add library

set(USER_LIBRARIES 
    AccountLibrary 
    ValidatorLibrary 
    MyMathrary
    DatabaseConnectionLibrary
    EmployeeManagerLibrary
)

add_executable(MainApp Main.cpp)                                            # add this executable
target_link_libraries(MainApp ${USER_LIBRARIES})                            # link google test to this executable

add_executable(TestRunnerApp TestRunner.cpp)                                # add this executable
target_link_libraries(TestRunnerApp PRIVATE                                 # link google test to this executable
    gtest
    gmock
    ${USER_LIBRARIES})
