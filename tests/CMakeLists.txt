project(tests)

set(SRC_LIST
        main.cpp
        TestPolynomial.cpp
        TestPolynomialRing.cpp
        TestPolynomialField.cpp
        )

add_executable(tests ${SRC_LIST})
target_link_libraries(tests PRIVATE ${LIB_NAME})


add_test(NAME TestAll COMMAND tests)
