# Simple C Parser

A custom **Recursive Descent Parser** written in C that interprets and executes some simple code of C programming language.

I built this project as part of the **Computational Models and Programming Language Concepts** module for my 2nd-year Computer Science degree. It demonstrates the core principles of compiler construction, including lexical analysis, parsing logic, and runtime execution.

---


* **Custom Tokenizer:** Wrote a lexer from scratch that reads source files character-by-character to identify keywords (`int`, `print`), identifiers, numbers, and operators.

* **Recursive Descent Parsing:** The core logic uses a top-down approach. I manually implemented the grammar rules to handle nested expressions and logic flow.

* **Operator Precedence:**
    * **High Priority:** Parentheses `( )` are evaluated first.
    * **Medium Priority:** Multiplication `*` and Division `/` happen next.
    * **Low Priority:** Addition `+` and Subtraction `-` happen last.

* **Symbol Table (Memory):** I implemented a dynamic lookup system (using a struct array) to store variables and retrieve their values during runtime.

* **Error Detection:**
    * **Syntax Errors:** The parser reports exactly *where* the code is broken (e.g., "Syntax Error (Line 3): Unexpected token...").
    * **Semantic Errors:** It catches logical mistakes like using undeclared variables, re-declaring existing ones, or dividing by zero.

---

## To Build & Run

Download the zip.
Compile parser.c with GCC.
Run parser.exe with input.txt

: You can modify the input.txt to test errors and usability.


### Prerequisites
You only need a standard C compiler (GCC), which is available on Linux, macOS, and Windows.

to install GCC, 

* **MacOS:** Open your terminal and run:
    ```bash
    xcode-select --install
    ```
* **Linux (Ubuntu/Debian):** Run:
    ```bash
    sudo apt update
    sudo apt install gcc
    ```
* **Windows:** You can install **MinGW** or use **WSL** (Windows Subsystem for Linux).

### Compilation
Once you have GCC, Compile parser.c
```bash
gcc parser.c -o parser
```

To run the executable on input.txt
```bash
./parser input.txt
```
