# Simple C Parser

A custom **Recursive Descent Parser** written in C that interprets and executes a subset of the C programming language.

I built this project as part of the **Computational Models and Programming Language Concepts** module for my 2nd-year Computer Science degree. It demonstrates the core principles of compiler construction, including lexical analysis, parsing logic, and runtime execution.

---

## 🚀 Features

This isn't just a basic parser—I implemented several advanced features to make it robust and closer to a real-world compiler:

* **Custom Tokenizer:** I wrote a lexer from scratch that reads source files character-by-character to identify keywords (`int`, `print`), identifiers, numbers, and operators.
* **Recursive Descent Parsing:** The core logic uses a top-down approach. I manually implemented the grammar rules to handle nested expressions and logic flow without using external tools like Lex or Yacc.
* **Operator Precedence:** The parser correctly handles mathematical order of operations, a critical feature for any language:
    * **High Priority:** Parentheses `( )` are evaluated first.
    * **Medium Priority:** Multiplication `*` and Division `/` happen next.
    * **Low Priority:** Addition `+` and Subtraction `-` happen last.
* **Symbol Table (Memory):** I implemented a dynamic lookup system (using a struct array) to store variables and retrieve their values during runtime.
* **Error Detection:**
    * **Syntax Errors:** The parser reports exactly *where* the code is broken (e.g., "Syntax Error (Line 3): Unexpected token...").
    * **Semantic Errors:** It catches logical mistakes like using undeclared variables, re-declaring existing ones, or dividing by zero.

---

## 🛠️ Build & Run

### Prerequisites
You only need a standard C compiler (GCC), which is available on Linux, macOS, and Windows (via MinGW or WSL).

### Compilation
To compile the project, run:
```bash
gcc parser.c -o parser