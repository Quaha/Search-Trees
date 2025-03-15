cmake_minimum_required(VERSION 3.13)

project(SearchTrees LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MTd")

# Добавляем GoogleTest
add_subdirectory(${CMAKE_SOURCE_DIR}/external/googletest)

# Собираем список заголовочных файлов
file(GLOB TREES_HEADERS "${CMAKE_SOURCE_DIR}/trees/*.hpp")

# Создаём интерфейсную библиотеку trees
add_library(trees INTERFACE)
target_include_directories(trees INTERFACE ${CMAKE_SOURCE_DIR}/trees)

# Создаём тесты
file(GLOB TESTS_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cpp")
add_executable(tests ${TESTS_SOURCES} ${TREES_HEADERS})  # Добавляем заголовочные файлы

# Линкуем тесты с trees и GoogleTest
target_link_libraries(tests trees gtest_main)
