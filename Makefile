# Имя исполняемого файла
TARGET = game

# Компилятор
CXX = g++

# Папка с заголовками и библиотеками SFML
SFML_DIR = /Users/sindi_hall/Downloads/SFML_ROOT

# Пути к заголовочным файлам
INCLUDE_PATHS = -I$(SFML_DIR)/include -I/opt/homebrew/Cellar/nlohmann-json/3.11.3/include

# Пути к библиотекам
LIBRARY_PATHS = -L$(SFML_DIR)/lib -L/opt/homebrew/lib

# Библиотеки для линковки
LIBRARIES = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lFLAC -lvorbisenc -lvorbisfile -lvorbis -logg

# Флаги компиляции
CXXFLAGS = -std=c++17 -O2 -Wall -g

# Указание rpath для SFML библиотек и зависимостей Homebrew
LDFLAGS = -Wl,-rpath,$(SFML_DIR)/lib -Wl,-rpath,/opt/homebrew/lib

# Исходники
SRC = main.cpp Player.cpp Bullet.cpp level.cpp Monster.cpp view.cpp Bonus.cpp Menu.cpp

# Объектные файлы
OBJ = $(SRC:.cpp=.o)

# Сборка объекта
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Сборка основного файла
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LIBRARY_PATHS) $(LIBRARIES) $(LDFLAGS) -o $(TARGET)

# Очистка
clean:
	rm -f $(OBJ) $(TARGET)

# Запуск сборки
all: $(TARGET)
