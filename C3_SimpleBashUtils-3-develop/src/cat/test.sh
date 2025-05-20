#!/bin/bash

# Тестовые данные
TEST_FILE="test1.txt"
S21_OUTPUT="s21_cat.txt"
CAT_OUTPUT="cat.txt"
FLAGS=(
    ""
    "-b"
    "-e"
    "-n"
    "-s"
    "-t"
    "-v"
)

# Проверяем, что тестовый файл существует
if [[ ! -f "$TEST_FILE" ]]; then
    echo "Ошибка: файл $TEST_FILE не найден. Создайте тестовый файл перед запуском."
    exit 1
fi

# Проходим по каждому флагу
for FLAG in "${FLAGS[@]}"; do
    echo "Тест с флагом: $FLAG"

    # Выполняем команды
    ./s21_cat $FLAG $TEST_FILE > $S21_OUTPUT
    cat $FLAG $TEST_FILE > $CAT_OUTPUT

    # Сравниваем результаты
    diff -s $S21_OUTPUT $CAT_OUTPUT

    # Удаляем временные файлы
    rm -f $S21_OUTPUT $CAT_OUTPUT
done

echo "Тест с флагом: --number-nonblank"

# Выполняем команды
./s21_cat --number-nonblank $TEST_FILE > $S21_OUTPUT
cat -b $TEST_FILE > $CAT_OUTPUT

# Сравниваем результаты
diff -s $S21_OUTPUT $CAT_OUTPUT

# Удаляем временные файлы
rm -f $S21_OUTPUT $CAT_OUTPUT

echo "Тест с флагом: --number"

# Выполняем команды
./s21_cat --number $TEST_FILE > $S21_OUTPUT
cat -n $TEST_FILE > $CAT_OUTPUT

# Сравниваем результаты
diff -s $S21_OUTPUT $CAT_OUTPUT

# Удаляем временные файлы
rm -f $S21_OUTPUT $CAT_OUTPUT

echo "Тест с флагом: --squeeze-blank"

# Выполняем команды
./s21_cat --squeeze-blank $TEST_FILE > $S21_OUTPUT
cat -s $TEST_FILE > $CAT_OUTPUT

# Сравниваем результаты
diff -s $S21_OUTPUT $CAT_OUTPUT

# Удаляем временные файлы
rm -f $S21_OUTPUT $CAT_OUTPUT

echo "Все тесты завершены!"