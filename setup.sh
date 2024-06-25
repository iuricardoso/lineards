#!/bin/bash

# Cria um diretório para a compilação
mkdir -p build
cd build

if [ -z "$1" ]; then
	# Configura o projeto
	cmake ../src
else
	# Configura o projeto
	cmake -DCMAKE_INSTALL_PREFIX="$1" ../src
fi

# Compila o projeto
make

# Instala a biblioteca e o cabeçalho
sudo make install || make install
