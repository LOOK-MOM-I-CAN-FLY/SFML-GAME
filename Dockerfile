# Базовый образ
FROM ubuntu:22.04

# Установка необходимых пакетов
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    wget \
    libsfml-dev \
    libflac-dev \
    libvorbis-dev \
    libogg-dev \
    libx11-dev \
    libxrandr-dev \
    libudev-dev \
    libopenal-dev \
    libfreetype6-dev \
    libglew-dev \
    nlohmann-json3-dev \
    mesa-utils \
    libgl1-mesa-dri \
    libgl1-mesa-glx \
    libglx-mesa0 \
    && apt-get clean

# Создание рабочей директории
WORKDIR /app

# Копирование проекта в контейнер
COPY . .

# Сборка проекта
RUN make clean
RUN make all

# Установка точки входа
CMD ["./game"]



