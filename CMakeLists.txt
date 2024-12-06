cmake_minimum_required(VERSION 3.15)

# Название проекта
project(SFML_Game LANGUAGES CXX)

# Установим стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к исходникам
set(SOURCES
    src/main.cpp
    src/Player.cpp
    src/Monster.cpp
    src/Bullet.cpp
    src/Bonus.cpp
    src/Level.cpp
    src/Menu.cpp
)

set(HEADERS
    include/Player.h
    include/Monster.h
    include/Bullet.h
    include/Bonus.h
    include/Level.h
    include/Menu.h
)

# Опционально включаем режим отладки
set(CMAKE_BUILD_TYPE Release)

# Проверяем наличие SFML
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.5.1
)
FetchContent_MakeAvailable(SFML)

# Установка исполняемого файла
add_executable(SFML_Game ${SOURCES} ${HEADERS})

# Подключение библиотеки SFML
target_link_libraries(SFML_Game PRIVATE sfml-graphics sfml-audio sfml-window sfml-system)

# Включение папки include в список поиска
target_include_directories(SFML_Game PRIVATE include)

# Добавим ресурсные файлы (шрифты, текстуры и т.д.)
file(GLOB RESOURCES "resources/*")
foreach(RESOURCE_FILE ${RESOURCES})
    add_custom_command(
        TARGET SFML_Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${RESOURCE_FILE}
                $<TARGET_FILE_DIR:SFML_Game>
    )
endforeach()

# Инструкция для пользователя
message(STATUS "Для сборки выполните команды:")
message(STATUS "1. mkdir build && cd build")
message(STATUS "2. cmake ..")
message(STATUS "3. cmake --build .")
message(STATUS "Для запуска выполните ./SFML_Game")
