#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Faltando um argumento"
    echo "Usage: $0 <argument>"
    exit 1
fi

echo "$1" > src.txt

echo "Source code: "
cat src.txt

compilation_errors=$(gcc first_try.c 2>&1) # Redireciona stdout e stderror para var 
# Não colocar nada entre essa var e o if!

if [ $? -eq 0 ]; then # checa status do último comando executado
    echo "Compilation successful"
    ./a.out
else
    echo "Compilation failed with the following errors:"
    echo "$compilation_errors"
fi
