set(HEADERS Calculator.h util.h)
set(SOURCES src/Calculator.cpp src/util.cpp)

add_library(calc ${HEADERS} ${SOURCES})

add_executable(calculator src/main.cpp)
target_link_libraries(calculator calc)
