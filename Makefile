# Имя исполняемого файла
TARGET = game

# Компилятор
CXX = g++

# Папка с заголовками и библиотеками SFML и TinyXML
SFML_DIR = /Users/sindi_hall/Downloads/SFML_ROOT
TINYXML_DIR = /Users/sindi_hall/Downloads/tinyxml2-master
TINYXML_BUILD_DIR = /Users/sindi_hall/Downloads/tinyxml2-master/build

# Пути к заголовочным файлам
INCLUDE_PATHS = -I$(SFML_DIR)/include -I$(TINYXML_DIR)

# Пути к библиотекам
LIBRARY_PATHS = -L$(SFML_DIR)/lib -L$(TINYXML_BUILD_DIR)

# Библиотеки для линковки
LIBRARIES = -lsfml-graphics -lsfml-window -lsfml-system -ltinyxml2

# Флаги компиляции
CXXFLAGS = -std=c++17 -O2 -Wall -g

# Указание rpath для SFML библиотек
LDFLAGS = -Wl,-rpath,$(SFML_DIR)/lib

# Исходники
SRC = main.cpp Player.cpp Bullet.cpp level.cpp

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
