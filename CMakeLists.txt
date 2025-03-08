cmake_minimum_required(VERSION 3.22)

project(TConsole)

set(SOUR_PATH ${CMAKE_HOME_DIRECTORY}/source)

set(SOURCE ${SOUR_PATH}/main.cpp ${SOUR_PATH}/minivim.cpp)

include_directories(${CMAKE_HOME_DIRECTORY}/inc/)

add_executable(app ${SOURCE})

target_link_libraries(app ncurses)