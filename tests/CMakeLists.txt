project(tests)

set(SRC_LIST
        ${SRC_LIST}
        main.cpp
        TestPolynomial.cpp
        TestPolynomialRing.cpp
        TestPolynomialField.cpp
        )

add_executable(${PROJECT_NAME} ${SRC_LIST})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_NAME})
