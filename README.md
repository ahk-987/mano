# 🖥️ Mano CLI

> A Mano Basic Computer assembler and simulator for the command line, written in C++26.

Feed it Mano assembly → get back RAM state and register values.

---

## ⚡ Quick Start

```bash
git clone https://github.com/ahk-987/mano.git
cd mano
cmake -B build && cmake --build build
./build/mano -i program.asm -o ram.txt -r registers.txt
```

Or use it interactively — type your assembly, press `Ctrl+D` when done:
```bash
./build/mano --stdio-only
```

---

## 🔧 CLI Flags

| Flag | Description |
|------|-------------|
| `-i file.asm` | Input assembly file |
| `-o file.txt` | Save RAM output (non-zero locations) |
| `-oa file.txt` | Save RAM output (all locations) |
| `-r file.txt` | Save register state |
| `-c file.txt` | Save generated hex code |
| `--stdio-only` | Use stdin/stdout for everything |
| `-h / --help` | Show help |

---

## 📝 Assembly Example

```asm
        ORG 0
        LDA X       ; load X into AC
        ADD Y       ; AC = X + Y
        STA Z       ; store result
        HLT
X,      DEC 5
Y,      DEC 3
Z,      DEC 0
        END
```

- Labels end with a comma: `LABEL,`
- Comments start with `;`
- `ORG` address is hexadecimal
- Case-insensitive

---

## 📟 Instruction Set

**Memory Reference (MRI)**
`AND` `ADD` `LDA` `STA` `BUN` `BSA` `ISZ`
— append `I` for indirect mode: `ADD X I`

**Register Reference**
`CLA` `CLE` `CMA` `CME` `CIR` `CIL` `INC` `SPA` `SNA` `SZA` `SZE` `HLT`

**Directives**
`ORG` `END` `HEX` `DEC`

---

## 📤 Sample Output

```
[LOC ]  [HEX]   [DECI]
[0000]  2005    08197
[0001]  1006    04102
[0002]  3007    12295
[0003]  7001    28673

IR  : 7001    AC  : 0008
PC  : 0004    AR  : 0003
E   : 0       HLT : 1
```

---

## ⚠️ Limitations

- IO instructions (`INP`, `OUT` etc.) not yet supported
- No step-through / debug mode

---

*Built as a learning project for computer architecture.*