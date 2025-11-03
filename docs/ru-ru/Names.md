# Нейминг

## Функции
- **Тип:** `camelCase`
- **Пример:**
    ```c++
    void someFunction() { ... }
    ```

## Переменнные
- **Тип:** `snake_case`
- **Пример:**
    ```c++
    int some_variable = 0;
    ```

## Константы
- **Тип:** `SCARED_SNAKE_CASE`
- **Пример:**
    ```c++
    const float SOME_CONST = 0.1f;
    constexpr int SOME_CONSTEXPR = 0;
    ```
## Классы и структуры
- **Тип:** `PascalCase`
- **Дополнительно:**
- **Префиксы**:

  | Тип                      | Префикс |
  |--------------------------|---------|
  | Перечисляемый тип (Enum) | E       |
  | Структура (Struct)       | D       |
  | Генерируемая Ошибка      | ERR     |
  | Вариативный тип (Union)  | U       |
- **Пример:**
    ```c++
    class SomeClass { ... }
    struct DSomeStruct { ... }
    enum class ESomeEnum { ... }
    ```

## Папки и модули
- **Тип:** `snake_case`
- **Пример:**
    ```
    src
        some_module
    ```

## Файлы
- **Тип:** `PascalCase`
- **Пример:**
    ```
    src
        SomeFile.cpp
    ```