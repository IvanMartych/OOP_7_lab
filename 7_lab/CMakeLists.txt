cmake_minimum_required(VERSION 3.10)
project(7_lab)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление опций компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Поддержка многопоточности
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Установка Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
    TLS_VERIFY false
)

# Отключаем использование статической библиотеки и переключаем на динамическую
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Исходные файлы
set(SOURCES
    src/npc.cpp
    src/knight.cpp
    src/pegasus.cpp
    src/squirrel.cpp
    src/factory.cpp
    src/arena.cpp
    src/combat_visitor.cpp
)

# Библиотека
add_library(${PROJECT_NAME}_lib ${SOURCES})
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Основной исполняемый файл
add_executable(${PROJECT_NAME}_exe main.cpp)
target_link_libraries(${PROJECT_NAME}_exe PRIVATE ${PROJECT_NAME}_lib Threads::Threads)

# Добавление тестов
enable_testing()

# Объединенные тесты
add_executable(${PROJECT_NAME}_gtests tests/gtests.cpp)
target_link_libraries(${PROJECT_NAME}_gtests PRIVATE ${PROJECT_NAME}_lib gtest_main Threads::Threads)
add_test(NAME 7_lab_tests COMMAND ${PROJECT_NAME}_tests)

# Копируем тестовые файлы в директорию сборки
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_data_npcs.txt
    ${CMAKE_CURRENT_BINARY_DIR}/test_data_npcs.txt
    COPYONLY
)