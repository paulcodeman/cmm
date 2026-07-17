# Roadmap: Expression Parser Fix

## Problem

C--'s expression parser cannot correctly resolve struct member access
(`this.y`, `list.font_w`, `size.height`) when combined with `*`, `/`, `%`
operators in expressions inside class methods — both in loop/condition
headers and in plain assignment statements.  The parser produces
`'X' not expected in this context` or `unexpected 'X'` errors, where X
is the token that follows the struct member expression.

**Root cause**: the memory corruption in `BackTextBlock` (now fixed) masked
this pre-existing parser limitation.

## Current workaround

Extract the struct member (or the entire expression) into a local variable
before the loop/condition:

```c
// before (error)
for(yi=0; yi<height; yi++)       // height is this.size.height
{
}

// after (works)
int h = height;
for(yi=0; yi<h; yi++)
{
}
```

## Next steps to fix the parser

### 1. Understand the failing pattern

Create minimal test cases that trigger the error.  Known triggers:
- Struct member as operand of `*`, `/`, `%` inside `for(;;)` condition
- Struct member in `if()` condition followed by `{` on next line
- Struct member as operand of `/` or `%` in an assignment expression
  (`yy - y / item_h`)
- Function call result (`strlen(...)`) combined with struct member via `%`

### 2. Locate the parser code

The error is thrown from `default:` cases in switch statements in:
- `tokb.cpp` — main expression parser (~14 call sites for `operatorexpected()`, ~16 for `unuseableinput()`)
- `tokc.cpp` — additional parsing paths

Key functions:
- `calcrm()` — resolves register/variable to a register machine address
- `dostructvar2()` — handles struct member access (`this.xxx`)
- Internal expression evaluator in `tokb.cpp` (several thousand lines)

### 3. Debug approach

Build with `-DDEBUGMODE` and run with a test case to see the token flow
just before the error.  The `verbosedebug` printf in `operatorexpected()`
and `unuseableinput()` already outputs: `tok`, `tok2`, `itok.name`,
`itok2.name`, `linenumber`, `searchteg`.

### 4. Hypothesis

The struct member access is resolved into an ESI-relative address, but the
expression evaluator does not track that the result is a value (not an
address) for the purposes of `*`/`/`/`%` operator precedence.  The fix
likely involves adding an intermediate `RM` node or adjusting the operator
handling in the expression tree builder.

### 5. Fix scope

A proper fix would touch the expression parser in `tokb.cpp` to correctly
handle RM (register machine) values produced by struct member resolution
as operands of multiplicative operators.

### 6. Validation

After the fix, all known failure cases must compile:
- `list_box.h` line 108: `new_cur_y = yy - y / item_h + first`
- `kfont.h::symbol()`: `for(yi=0; yi<height; yi++) { ... }`
- `kfont.h::ApplySmooth()`: `for(i=raw; i < to; i+=KFONT_BPP) { ... }`
- `TWB.c::ParseHtml()`: `draw_x - left_gap / list.font_w + strlen(...) % 4`

The current workarounds (extracting to local variables) should be kept in
the library code until a parser-level fix is confirmed.
