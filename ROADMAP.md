# Roadmap

## 1. Quick win: `for (int i=0; ...)` in loop headers

Allow type keywords (`int`, `dword`, `byte`, etc.) inside `for()` initializer
within function bodies.  Scope of the declared variable = the `for` loop body
only.

### What to change

- **tokc.cpp:648-657** — remove the `"cannot declare variables within
  function { } block"` error for type keywords
- **tokc.cpp `dofor()`** — allow type + identifier after `(`, add variable
  to local table, remove it after the loop body
- No full block-scope system needed — just track the one for-loop variable

### Status

Estimated effort: **3-5 hours**.

---

## 2. Full block-scope variables

After the quick `for (int)` works, add proper nested scope:
`{ int x; }`, `if (...) { int y; }`, etc.

### What to change

- Push/pop scope on `{` / `}`
- Allow type keywords in any block inside a function
- Remove variables from symbol table when leaving scope
- Fix register reuse / ESP tracking for scoped variables

### Status

Blocked by item 1.

---

## 3. Expression parser: struct member + `*` / `/` / `%`

C-- cannot resolve struct member access (`this.y`, `list.font_w`,
`size.height`) when combined with `*`, `/`, `%` inside class methods.

### Workaround

Extract the struct member into a local variable before use.

### Root cause

Struct member access resolves to an ESI-relative address, but the expression
evaluator loses track that the result is a *value* (not an address) for
multiplicative operators.  The `BackTextBlock` memory corruption (now fixed)
masked this pre-existing limitation.

### Debug

Build with `-DDEBUGMODE` and run a test case.  The `verbosedebug` printf
in `operatorexpected()` / `unuseableinput()` already outputs: `tok`, `tok2`,
`itok.name`, `itok2.name`, `linenumber`, `searchteg`.

### Fix scope

Touch `tokb.cpp` — the expression evaluator's handling of RM values from
struct member resolution.

### Known triggers
- `yy - y / item_h`
- `for(yi=0; yi<size.height; yi++)`
- `draw_x - left_gap / list.font_w + strlen(...) % 4`
