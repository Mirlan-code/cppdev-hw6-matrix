cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION $ENV{RUN_NUMBER} CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(matrix VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_executable(matrix_cli main.cpp)
add_library(matrix lib.cpp matrix.cpp matrix.h tuple_helper.cpp tuple_helper.h)
add_executable(tests tests.cpp)

set_target_properties(matrix_cli matrix tests PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        )
target_include_directories(matrix
        PRIVATE "${CMAKE_BINARY_DIR}"
        )

target_link_libraries(matrix_cli PRIVATE
        matrix
        )

if (MSVC)
    target_compile_options(matrix_cli PRIVATE
            /W4
            )
    target_compile_options(matrix PRIVATE
            /W4
            )
    target_compile_options(tests PRIVATE
            /W4
            )
else ()
    target_compile_options(matrix_cli PRIVATE
            -Wall -Wextra -pedantic -Werror
            )
    target_compile_options(matrix PRIVATE
            -Wall -Wextra -pedantic -Werror
            )
    target_compile_options(tests PRIVATE
            -Wall -Wextra -pedantic -Werror
            )
endif()



install(TARGETS matrix_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

target_link_libraries(tests GTest::gtest_main matrix)

include(GoogleTest)
gtest_discover_tests(tests)