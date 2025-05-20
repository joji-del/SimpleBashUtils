# --------------------------------------------------------------------------Переменные
SHELL := /bin/bash
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic -D_POSIX_C_SOURCE=200809L
S21grep = ./s21_grep
FILE = test1.txt
TEST_SCRIPT = ./tests_grep.sh

# --------------------------------------------------------------------------Реализация (Simple Bash Utils / grep)
# Сборка
s21_grep:
	@$(CC) $(CFLAGS) *.c -o s21_grep
	@echo "grep is ready to work!"


# Удаление исполняемого файла
clean_grep:
	@rm -rf s21_grep
	@echo "deleted !"


# Пересборка исполняемого файла
rebuild_grep: clean_grep s21_grep



# --------------------------------------------------------------------------
# Запуск тестов
tests_grep: rebuild_grep
	@echo "Запуск тестов через $(TEST_SCRIPT)..."
	@chmod +x $(TEST_SCRIPT)
	@$(TEST_SCRIPT)


# Cppcheck + style test
	@echo -n "Test Cppcheck: "
	@cppcheck --enable=all --std=c11 -D_POSIX_C_SOURCE=201709L \
	--suppress=missingIncludeSystem s21_grep.c
	@echo "Проверка CPP выполнена!"

	
	@echo -n "Test style: "
	@cp ../../materials/linters/.clang-format .clang-format
	@echo "Файл clang-format скопирован в папку grep."
	@clang-format -n *.c *.h
	@echo "Проверка кода по Clang завершена."
	@rm -f .clang-format
	@echo "Файл .clang-format удален из папки grep."


# --------------------------------------------------------------------------Полезные команды
# Форматирование кода
clang:
	@cp ../../materials/linters/.clang-format .clang-format
	@echo "Файл clang-format скопирован в папку grep."
	@clang-format -n *.c *.h
	@echo "Проверка кода по Clang завершена."	
	@clang-format -i *.c *.h
	@echo "Форматирование кода по Clang завершено."		
	@rm -f .clang-format
	@echo "Файл .clang-format удален из папки grep."


# git add, commit и push
push:
	@read -p "Введите сообщение коммита: " commit_msg; \
	cd ../..; \
	git add .; \
	git commit -m "$$commit_msg"; \
	git push origin develop
	@echo "Done !"
	
# --------------------------------------------------------------------------Проверки памяти
# CPP-check
cpp:
	@cppcheck --enable=all --std=c11 -D_POSIX_C_SOURCE=201709L \
	--suppress=missingIncludeSystem s21_grep.c
	@echo "Проверка CPP выполнена!"

# Утечки leaks
leaks:
	@leaks -atExit -- ./s21_grep
