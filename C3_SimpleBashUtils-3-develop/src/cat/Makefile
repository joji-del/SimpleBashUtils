# --------------------------------------------------------------------------Переменные
SHELL := /bin/bash
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic -D_POSIX_C_SOURCE=200809L
S21CAT = ./s21_cat
FILE = test1.txt
TEST_SCRIPT = ./test.sh

# --------------------------------------------------------------------------Реализация (Simple Bash Utils / cat)
# Сборка
s21_cat:
	@$(CC) $(CFLAGS) *.c -o s21_cat
	@echo "cat is ready to work!"


# Удаление исполняемого файла
clean_cat:
	@rm -rf s21_cat
	@echo "deleted !"


# Пересборка исполняемого файла
rebuild_cat: clean_cat s21_cat



# --------------------------------------------------------------------------
# Запуск тестов
tests_cat: rebuild_cat
	@echo "Запуск тестов через $(TEST_SCRIPT)..."
	@chmod +x $(TEST_SCRIPT)
	@$(TEST_SCRIPT)


# Cppcheck + style test
	@echo -n "Test Cppcheck: "
	@cppcheck --enable=all --std=c11 -D_POSIX_C_SOURCE=201709L \
	--suppress=missingIncludeSystem s21_cat.c
	@echo "Проверка CPP выполнена!"

	
	@echo -n "Test style: "
	@cp ../../materials/linters/.clang-format .clang-format
	@echo "Файл clang-format скопирован в папку cat."
	@clang-format -n *.c *.h
	@echo "Проверка кода по Clang завершена."
	@rm -f .clang-format
	@echo "Файл .clang-format удален из папки cat."


# --------------------------------------------------------------------------Полезные команды
# Форматирование кода
clang:
	@cp ../../materials/linters/.clang-format .clang-format
	@echo "Файл clang-format скопирован в папку cat."
	@clang-format -n *.c *.h
	@clang-format -n *.c *.h
	@echo "Проверка кода по Clang завершена."	
	@clang-format -i *.c *.h
	@clang-format -i *.c *.h
	@echo "Форматирование кода по Clang завершено."		
	@rm -f .clang-format
	@echo "Файл .clang-format удален из папки cat."


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
	--suppress=missingIncludeSystem s21_cat.c
	@echo "Проверка CPP выполнена!"

# Утечки leaks
leaks:
	@leaks -atExit -- ./s21_cat
