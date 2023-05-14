#pragma once

#include <ncurses.h>

enum MenuItems {
    INPUT_PATTERN, // Ввод шаблонов
    SELECT_DIR,    // Выбор директории
    FILES_LIST,
    PROCESS,    // Переименование
    SELECT_OPT, // Выбор опции
    INFO,       // Показ информации о программе
    EXIT,       // Выход
};

// Принимает: x -  координата по x
//            y -  координата по y
// Функция проверяет размер текущего терминала
// Если он меньше минимального, то возвращает сообщение
// об ошибки и текущей размер терминала
void check_term_size(int x, int y);

// Принимает: menu - указатель на окно с меню
//
// В окне функция выводит само меню и даёт
// пользователю выбрать нужную ему функцию
void start(WINDOW* menu);