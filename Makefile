# Имя исполняемого файла
TARGET = sfml-app

# Папка, где находятся заголовки и библиотеки SFML
SFML_DIR = /Users/sindi_hall/Downloads/SFML_ROOT

# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -I$(SFML_DIR)/include
LDFLAGS = -L$(SFML_DIR)/lib -lsfml-graphics -lsfml-window -lsfml-system

# Исходные файлы
SRC = main.cpp Player.cpp Bullet.cpp

# Заголовочные файлы (добавляем для отслеживания изменений)
HEADERS = Player.h map.h view.h Bullet.h 

# Правило для сборки
$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# Очистка
clean:
	rm -f $(TARGET)
