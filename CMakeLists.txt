cmake_minimum_required(VERSION 3.20)

project(AccountingBook VERSION 1.0.0 LANGUAGES CXX)

# -------------------------
# Global settings
# -------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Helpful for VSCode/clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(ACCOUNTING_BUILD_APP "Build the CLI application" ON)
option(BUILD_TESTS "Build unit tests (GoogleTest)" ON)
option(ENABLE_COVERAGE "Enable coverage flags (GCC/Clang only)" OFF)

# Warnings
if(MSVC)
  add_compile_options(/W4 /permissive- /utf-8)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# -------------------------
# Core library (your project logic)
# -------------------------
add_library(accounting_core
  src/AccountingSystem.cpp
  src/BirthdayReminder.cpp
  src/BudgetReminder.cpp
  src/Category.cpp
  src/CategoryManager.cpp
  src/ChartGenerator.cpp
  src/Date.cpp
  src/FinanceAdvisor.cpp
  src/JsonStorage.cpp
  src/Record.cpp
  src/StatisticsManager.cpp
  src/Subscription.cpp
  src/SubscriptionManager.cpp
  src/ThemeManager.cpp
  src/User.cpp
)

target_include_directories(accounting_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------------------------
# Coverage flags (Linux/macOS; GCC/Clang)
# Apply only to core + tests (NOT to the app)
# -------------------------
if(ENABLE_COVERAGE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Coverage enabled for accounting_core + unit_tests")
    target_compile_options(accounting_core PRIVATE -O0 -g --coverage)
    target_link_options(accounting_core PRIVATE --coverage)
  else()
    message(WARNING "ENABLE_COVERAGE is ON but compiler is not GCC/Clang; ignoring")
  endif()
endif()

# -------------------------
# CLI app (optional)
# -------------------------
if(ACCOUNTING_BUILD_APP)
  add_executable(accounting_app src/main.cpp)
  target_link_libraries(accounting_app PRIVATE accounting_core)
endif()

# -------------------------
# Tests (GoogleTest)
# -------------------------
if(BUILD_TESTS)
  include(CTest)
  enable_testing()

  # Silence FetchContent timestamp warning (CMP0135)
  if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
  endif()

  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )

  # On Windows, keep CRT consistent
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  add_executable(unit_tests
    tests/test_category.cpp
    tests/test_category_manager.cpp
    tests/test_record.cpp
    # tests/test_integration_category_record.cpp  # 可选：集成测试
  )

  target_link_libraries(unit_tests PRIVATE GTest::gtest_main accounting_core)

  # Coverage also needs to link the test executable on GCC/Clang
  if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(unit_tests PRIVATE -O0 -g --coverage)
    target_link_options(unit_tests PRIVATE --coverage)
  endif()

  include(GoogleTest)
  gtest_discover_tests(unit_tests)


  # -------------------------
  # Integration tests (separate executable for reporting)
  # -------------------------
  add_executable(integration_tests
    tests/test_integration_record_flow.cpp
    tests/test_integration_storage_roundtrip.cpp
    tests/test_integration_statistics.cpp
  )
  target_link_libraries(integration_tests PRIVATE GTest::gtest_main accounting_core)

  if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(integration_tests PRIVATE -O0 -g --coverage)
    target_link_options(integration_tests PRIVATE --coverage)
  endif()

  gtest_discover_tests(integration_tests)
endif()
