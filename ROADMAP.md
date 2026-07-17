# Roadmap

## 1. Quick win: `for (int i=0; ...)` in loop headers ✅ DONE

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

**Blocked by BackTextBlock fix** → now unblocked. Next milestone.

---

## 2. Full block-scope variables ✅ UNBLOCKED

After the quick `for (int i)` works, add proper nested scope:
`{ int x; }`, `if (...) { int y; }`, etc.

### What to change

- Push/pop scope on `{` / `}`
- Allow type keywords in any block inside a function
- Remove variables from symbol table when leaving scope
- Fix register reuse / ESP tracking for scoped variables

### Status

Blocked by item 1.

---

## 3. Expression parser: struct member + `*` / `/` / `%` ✅ FIXED

C-- could not resolve struct member access (`this.y`, `list.font_w`,
`size.height`) when combined with `*`, `/`, `%` inside class methods.

### Root cause

Memory corruption in `BackTextBlock` (use-after-free in `dofor`,
`RunBackText`, `calcrm`, `dostructvar2`) injected stale tokens (`"this; "`)
into the token stream, which cascaded into `operatorexpected()` errors.

### Fix

Fixed 3 use-after-free bugs in `toka.cpp` / `tokc.cpp`:
- `dofor()` / `RunBackText()` / `calcrm()` / `dostructvar2()` — now
  properly save/restore `BackTextBlock` and NULL it after `free()`.

### Result

**Original failing patterns now compile cleanly:**

| File | Pattern | Status |
|------|---------|--------|
| `kfont.h` | `for(yi=0; yi<height; yi++)` | ✅ |
| `kfont.h` | `size.width * KFONT_BPP` | ✅ |
| `kfont.h` | `i - _raw % line_w + KFONT_BPP` | ✅ |
| `list_box.h` | `if (MouseOver(xx, yy)) {` | ✅ |
| `list_box.h` | `new_cur_y = yy - y / item_h + first` | ✅ |
| `TWB.c` | `draw_x - left_gap / list.font_w + strlen(...) % 4` | ✅ |

**All workarounds removed** (commits `4def170`, `674a3da`, `4e4c467`).

### Workarounds removed (no longer needed)

- `kfont.h` — extracted `height`, `size.width`, `raw`, `block` to locals
- `list_box.h` — inlined `MouseOver` check instead of `if (MouseOver(xx, yy))`
- `TWB.c` — extracted `strlen(#linebuf)` and struct members to locals

---

### Next steps

1. Implement **item 1** (`for (int i=0; ...)` quick win)
2. Implement **item 2** (full block scope)
3. Re-evaluate if any other parser issues remain