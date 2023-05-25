# Сборка ---------------------------------------

# В качестве базового образа для сборки используем gcc:latest
FROM gcc:latest as build

# Установим рабочую директорию для сборки GoogleTest
# Скачаем все необходимые пакеты и выполним сборку GoogleTest
# Такая длинная команда обусловлена тем, что
# Docker на каждый RUN порождает отдельный слой,
# Влекущий за собой, в данном случае, ненужный оверхед
RUN apt-get update && \
    apt-get install -y \
      libboost-dev libboost-program-options-dev \
      libpqxx-dev \
      libpq-dev \
     # libpq \
     # libpqxx \
      git \
      cmake \
      postgresql-server-dev-all

# Скопируем директорию /src в контейнер
ADD . /app

# Установим рабочую директорию для сборки проекта
WORKDIR /app/build

RUN git clone https://github.com/jtv/libpqxx.git

WORKDIR /app/build/libpqxx

RUN cmake .

WORKDIR /app

# Выполним сборку нашего проекта, а также его тестирование
RUN cmake . && \
    cmake --build . -j 8

# Запуск ---------------------------------------

# В качестве базового образа используем ubuntu:latest
FROM ubuntu:latest

# Добавим пользователя, потому как в Docker по умолчанию используется root
# Запускать незнакомое приложение под root'ом неприлично :)
RUN groupadd -r sample && useradd -r -g sample sample
USER sample

# Установим рабочую директорию нашего приложения
WORKDIR /app

# Скопируем приложение со сборочного контейнера в рабочую директорию
COPY --from=build /app/server .

# Установим точку входа
ENTRYPOINT ["./server"]