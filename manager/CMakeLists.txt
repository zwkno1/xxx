cmake_minimum_required(VERSION 2.6)

project(manager)

find_package(Boost COMPONENTS system filesystem REQUIRED) 

include_directories(${Boost_INCLUDE_DIRS})

aux_source_directory(. SRC_LIST)

add_executable(manager ${SRC_LIST})

target_link_libraries(manager ${Boost_LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
