cmake_minimum_required(VERSION 3.20)
project(server)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_BUILD_TYPE Debug)



#set(PostgreSQL_ROOT /opt/homebrew/opt/libpq/include)
find_package(PostgreSQL REQUIRED)


set(libpqxx_ROOT /app/build/libpqxx)
find_package(libpqxx REQUIRED)


include_directories (${PostgreSQL_INCLUDE_DIR} ${libpqxx_ROOT}/include)
link_directories(${PostgreSQL_LIBRARY_DIR} ${libpqxx_ROOT}/lib)




add_executable(server main.cpp src/pkg/http_utlis/http_utils.h src/pkg/shali_server/shali_server.h src/internal/Application.h src/internal/chat/usecase.cpp src/internal/chat/usecase/usecase.cpp src/internal/chat/dto.cpp src/internal/chat/models.cpp src/internal/chat/repository.cpp src/internal/chat/repository/postgres.cpp)
#target_link_libraries(server PRIVATE PostgreSQL::PostgreSQL pqxx)
#target_link_libraries(server PRIVATE PostgreSQL::libpqxx)
#target_link_libraries(server ${PQXX_LIBRARIES})