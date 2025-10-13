find_package(PostgreSQL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)

add_library(Database
    db.cpp
)

target_include_directories(Database
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PostgreSQL_INCLUDE_DIRS}
        ${LIBPQXX_INCLUDE_DIRS}
)

target_link_libraries(Database 
    PRIVATE 
        ${PostgreSQL_LIBRARIES}
        ${LIBPQXX_LIBRARIES}
)

target_link_directories(Database
    PRIVATE
        ${LIBPQXX_LIBRARY_DIRS}
)