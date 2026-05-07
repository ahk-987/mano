# Mano CLI

A command-line assembler and simulator for the **Mano Basic Computer** — the theoretical 16-bit computer architecture described in *Computer System Architecture* by Morris Mano.

Written in **C++26**.

---

## What It Does

1. Takes Mano assembly code as input (file or stdin)
2. Runs a **2-pass assembly** to resolve labels and generate machine code
3. Loads the machine code into a simulated 4096-word RAM
4. Executes it on a simulated Mano CPU
5. Outputs the final **RAM state** and **register values**

> The simulator directly performs operations rather than emulating the full fetch-decode-execute cycle.

---

## Building

Requires **CMake 4.2.1+** and a **C++26** capable compiler (Clang 18+ or GCC 14+).

```bash
git clone https://github.com/ahk-987/mano.git
cd mano
cmake -B build
cmake --build build
```

Binary will be at `build/mano`.

---

## Usage

```
mano [options]
```

| Flag | Argument | Description |
|------|----------|-------------|
| `-i` | `file.asm` | Input assembly file |
| `-o` | `file.txt` | Save RAM state (non-zero locations only) |
| `-oa` | `file.txt` | Save RAM state (all 4096 locations) |
| `-r` | `file.txt` | Save register state |
| `--stdio-only` | | Force all I/O through stdin/stdout |
| `-h`, `--help` | | Show help |

### Examples

Run from a file, save outputs:
```bash
mano -i program.asm -o ram.txt -r registers.txt
```

Interactive stdin mode — type assembly, press `Ctrl+D` when done:
```bash
mano --stdio-only
```

Save generated hex code alongside RAM state:
```bash
mano -i program.asm -c hexcode.txt -o ram.txt
```

Run with no flags — defaults to interactive stdin mode:
```bash
mano
```

---

## Assembly Language

### Format

```asm
        ORG 0           ; set location counter to 0 (hex)
        LDA X           ; load X into AC
        ADD Y           ; AC = AC + Y
        STA Z           ; store AC into Z
        HLT             ; halt
X,      DEC 5           ; label X, decimal value 5
Y,      DEC 3           ; label Y, decimal value 3
Z,      DEC 0           ; label Z, result goes here
        END             ; end of program
```

**Rules:**
- Labels are defined with a trailing comma: `LABEL,`
- Labels are referenced without a comma: `LDA LABEL`
- Comments start with `;` — anything after is ignored
- Assembly is **case-insensitive**
- `ORG` addresses are in **hexadecimal**
- Forward references are supported — labels can be used before they are defined

---

## Instruction Set

### Memory Reference Instructions (MRI)

These operate on a memory address. Append `I` for **indirect addressing**.

| Mnemonic | Opcode | Operation |
|----------|--------|-----------|
| `AND` | `0` | `AC ← AC ∧ M[AR]` |
| `ADD` | `1` | `AC ← AC + M[AR]`, carry → E |
| `LDA` | `2` | `AC ← M[AR]` |
| `STA` | `3` | `M[AR] ← AC` |
| `BUN` | `4` | `PC ← AR` (unconditional branch) |
| `BSA` | `5` | `M[AR] ← PC`, `PC ← AR+1` (branch & save return) |
| `ISZ` | `6` | `M[AR]++`, skip next if zero |

Indirect mode example:
```asm
ADD X I     ; AC = AC + M[M[X]]  (indirect)
ADD X       ; AC = AC + M[X]     (direct)
```

### Register Reference Instructions

| Mnemonic | Operation |
|----------|-----------|
| `CLA` | Clear AC |
| `CLE` | Clear E |
| `CMA` | Complement AC |
| `CME` | Complement E |
| `CIR` | Circulate right: AC and E rotate right by 1 |
| `CIL` | Circulate left: AC and E rotate left by 1 |
| `INC` | Increment AC, carry → E |
| `SPA` | Skip next if AC > 0 (positive) |
| `SNA` | Skip next if AC < 0 (negative) |
| `SZA` | Skip next if AC = 0 |
| `SZE` | Skip next if E = 0 |
| `HLT` | Halt execution |

### Assembler Directives

| Directive | Example | Description |
|-----------|---------|-------------|
| `ORG` | `ORG 1FF` | Set location counter (hex address) |
| `END` | `END` | Mark end of program |
| `HEX` | `HEX 1A2B` | Store a 16-bit hex constant |
| `DEC` | `DEC 42` | Store a 16-bit decimal constant |

> IO instructions (`INP`, `OUT`, `SKI`, `SKO`, `ION`, `IOF`) are recognised but **not yet implemented**.

---

## Output Format

**RAM state:**
```
[LOC ]  [HEX]   [DECI]
[0000]  2005    08197
[0001]  1006    04102
[0002]  3007    12295
[0003]  7001    28673
[0005]  0005    00005
[0006]  0003    00003
```

**Register state:**
```
IR  : 7001
AC  : 0008
AR  : 0003
PC  : 0004
E   : 0
HLT : 1
```

All values are printed in **hexadecimal**.

---

## Project Structure

```
mano/
├── header/
│   ├── cpu.hpp           # Simulator — all CPU instructions and run loop
│   ├── mymemory.hpp      # Memory module declaration
│   ├── file_handler.hpp  # Assembler and I/O declarations
│   └── Cli_parser.hpp    # CLI parser declaration
├── src/
│   ├── main.cpp          # Entry point
│   ├── memory.cpp        # Memory implementation
│   ├── file_handler.cpp  # 2-pass assembler, file I/O
│   └── Cli_parser.cpp    # CLI argument parsing
├── IO/
│   ├── asmano1.txt       # Example assembly file
│   ├── ramstate.txt      # Sample RAM output
│   └── registers.txt     # Sample register output
└── CMakeLists.txt
```

---

## Known Limitations

- **IO instructions not implemented** — `INP`, `OUT`, `SKI`, `SKO`, `ION`, `IOF` are parsed and recognised but silently skipped during execution
- **No step-through or debug mode** — execution runs to completion with no way to inspect intermediate state
- **Does not emulate the fetch-decode-execute cycle** — operations are performed directly

---

## References

- *Computer System Architecture*, 3rd Edition — Morris Mano

---

Built by **Abdul Hannan Bhavnagri** — [github.com/ahk-987](https://github.com/ahk-987)