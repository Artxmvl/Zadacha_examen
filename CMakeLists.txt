cmake_minimum_required(VERSION 3.10)
project(OnlineStore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PostgreSQL REQUIRED)
find_library(PQXX_LIB pqxx)
find_path(PQXX_INCLUDE pqxx/pqxx)

include_directories(${PQXX_INCLUDE} ${PostgreSQL_INCLUDE_DIRS} include)

set(SOURCES
    src/main.cpp
    src/User.cpp
    src/Order.cpp
)

set(HEADERS
    include/DatabaseConnection.h
    include/User.h
    include/Order.h
    include/Payment.h
)

add_executable(online_store ${SOURCES} ${HEADERS})
target_link_libraries(online_store ${PQXX_LIB} ${PostgreSQL_LIBRARIES})
