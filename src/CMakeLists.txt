cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(lineards)

# Adiciona o arquivo de cabeçalho ao projeto
include_directories(${CMAKE_SOURCE_DIR})

# Adiciona o arquivo de código-fonte ao projeto
add_library(lineards STATIC lineards.c)

# Alvo para instalação
install(TARGETS lineards DESTINATION lib)
install(FILES lineards.h DESTINATION include)
