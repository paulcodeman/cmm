# Roadmap: Operator Precedence in Math Expressions

Цель: добавить корректный приоритет операций (`*`, `/`, `%` перед `+`, `-`) во все арифметические выражения C--.

## Сделано

### 1. `is_high_prec()` — `toke.cpp` / `tok.h`
Вынесена функция определения high-precedence (`*`/`/`/`%`), объявлена в `tok.h`.

### 2. Константная свёртка — `toke.cpp` (calcqwordnumber)
При обработке константных выражений — если после операнда идёт `*`, `/`, `%`, цепочка сворачивается в одно значение на этапе компиляции.

### 3. `do_e_axmath2` — EAX/AX path — `tokb.cpp:7187`
- **Константная свёртка:** цепочка `*`, `/`, `%` над числами сворачивается
- **Runtime term-loop:** если операнд — переменная, то:
  1. push EAX (сохранить текущий результат)
  2. `getinto_e_ax()` загружает правый операнд
  3. mul/div/mod по цепочке
  4. pop EDX, XCHG, combine (add/sub/etc)

### 4. `doregmath_32` — произвольный 32/16-bit регистр — `tokb.cpp:12321`
- **Константная свёртка:** `BX + 2 * 3` → `add bx, 6`
- **Runtime term-loop:** `BX + b * 3` → push BX, load b, mul 3, pop BX, add BX, EAX

### 5. CI/CD — `.github/workflows/build.yml`
- Сборка `cmm.exe` через MSYS2 MINGW32 на каждый push/PR
- Авто-релиз с `cmm.exe` при пуше тэга `v*`

## Осталось

### 6. Скобки ()
Рекурсивный спуск в `factor()`: при `(` — запомнить позицию, вызвать `expr()` (с учётом приоритета), проверить `)`.
Затрагивает `do_e_axmath2`, `doregmath_32`, `calcqwordnumber`.

### 7. `doregmath64` — 64-bit math — `tokb.cpp:23459`
64-битная арифметика на двух регистрах (r1=low, r2=high). Runtime term-loop сильно сложнее — нужен 64-bit mul/div и аккуратное управление парой регистров.

### 7. Убрать `-fpermissive`
Добавить недостающий аргумент `int reg` в вызовы `getoperand()` по всему коду.

---

## Тесты
- `test_math.c` — константная свёртка
- `test_math2.c` — `EAX + 2 * 3`, `EAX + b * 3`
- `test_math3.c` — `BX + b * 3`, `CX + b * 3`, `DX + b * 3`, `SI + b * 3`, `DI + b * 3`
