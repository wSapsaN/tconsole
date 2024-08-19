# Минимально допустимая версия CMake
cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

# Настройка параметров сборки CMake
set(CMAKE_BUILD_TYPE "Release")

# Настройка параметров следования стандарту языка C++ 
set(CMAKE_CXX_STANDARD 17)

# Рабочее название
set(APP "main")

# Основные параметры проекта
project(${APP} VERSION 0.1 LANGUAGES CXX)

# Настройка параметров компилятора
# add_compile_options(-c -O2 -Wall -Wextra -Werror -pedantic)

# Определить цель
add_executable(${APP})

# Библиотечные фйлы
target_link_libraries(${APP}
  tinfo
  ncurses
)

# Директории с подключаемыми файлами
target_include_directories(${APP}
  PRIVATE  
    core/inc
)

# Определение, какие файлы используются при сборке цели
target_sources(${APP}
  PRIVATE
    core/src/main.cpp
)