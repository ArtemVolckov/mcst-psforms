#!/bin/bash
# Для запуска скрипта: ./run_tests.sh

MAIN_DIR=".."
INPUTS_DIR="inputs"
OUTPUTS_DIR="outputs"

# Создание исполняемого файла
make -C "$MAIN_DIR"

# Запуск теста
for i in {1..11}
do
  # перенаправление stderr в файл
  "$MAIN_DIR/./main" < "$INPUTS_DIR/test${i}.in" > "$OUTPUTS_DIR/test${i}.out" 2>&1
done

# test{1-6} - примеры из условия задания
