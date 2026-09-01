# Vibe Coding Log — ECE 309 Project 1

This log documents the prompts, AI responses, and debugging iterations used to build `harness.c` via Specification Driven Development (SDD).

---

## 1. Initial Specification Prompt

**Prompt given to AI:**

> "I need to write a simple command-line program in C. I am a beginner, so please keep the code as simple as possible. Do not use external libraries, only standard ones like `<stdio.h>` and `<string.h>`. Here is the specification for the program:
> 1. It should run an infinite while loop that asks for user input using fgets.
> 2. If the user types 'exit', the loop should break and the program should end.
> 3. If the user types a sentence containing the word 'hello', the program should print a hardcoded greeting.
> 4. If the user types anything else, it should echo their input back to them.
> 5. Please add clear, line-by-line comments explaining what the code is doing."

**AI Response (summary):**
Generated a single-file C program (`harness.c`) using `fgets` for input, `strcspn` to strip the trailing newline, `strcmp` to detect the exact "exit" command, and `strstr` to detect "hello" as a substring anywhere in the input. Included line-by-line comments explaining each library function.

---

## 2. Compiler Setup Debugging

**Issue:** `gcc` was not recognized in PowerShell.

**Resolution:** AI explained that VS Code, the C/C++ extension, and CMake are all tools that sit *around* a compiler but are not compilers themselves. Directed to install a real compiler via one of:
- MSYS2/MinGW-w64 (native Windows), or
- WSL (Windows Subsystem for Linux) — chosen path, per assignment instructions.

**Steps taken:**
```bash
wsl --install
sudo apt update && sudo apt install gcc
gcc --version
```
Installed the WSL extension in VS Code and connected the editor to the Ubuntu environment via `code .`.

---

## 3. Linker Error: `undefined reference to 'main'`

**Error encountered:**
```
/usr/bin/x86_64-linux-gnu-ld.bfd: .../Scrt1.o: in function `_start':
(.text+0x1b): undefined reference to `main'
collect2: error: ld returned 1 exit status
```

**Debugging process:**
1. First suspected a stray string (`penis`) accidentally prepended to `#include <stdio.h>` in the file, corrupting the preprocessor directive.
2. After removing it, the error persisted. Ran `wc -l harness.c`, which returned `0` — the file was completely empty, meaning `main` genuinely didn't exist in the compiled unit.
3. Root cause: edits made in VS Code were not being reliably saved to the file on the WSL-mounted path (`/mnt/c/Users/.../harness.c`), possibly due to a Windows/WSL filesystem sync issue or an unsaved buffer.
4. **Fix:** Rewrote the file directly from the WSL terminal using a heredoc, bypassing the editor save step entirely:
   ```bash
   cat > harness.c << 'EOF'
   ... (program contents) ...
   EOF
   ```
5. Verified with `wc -l harness.c` (returned nonzero) before recompiling.

**Result:** Compiled cleanly with `gcc harness.c -o harness`.

---

## 4. Manual Testing

Ran `./harness` and tested interactively:

```
Enter text (type 'exit' to quit): hello bjdhlj
Hello there! Nice to see you.
Enter text (type 'exit' to quit): exit
Exiting the program. Goodbye!
```

Confirmed: substring "hello" detection works mid-sentence, exact "exit" match ends the loop, and other input is echoed back correctly.

---

## 5. Automated Testing Script Prompt

**Prompt given to AI:**

> "I have a compiled C program named harness. Write a very simple Bash script (for Linux/Mac) that automatically sends the word 'hello', followed by the word 'exit', into the program to test if it works."

**AI Response:**
Generated `test.sh`, using `printf` piped into `./harness` to simulate user input deterministically without manual typing:

```bash
#!/bin/bash
# test.sh - Automated test script for the harness program
printf "hello\nexit\n" | ./harness
```

Made executable and run via:
```bash
chmod +x test.sh
bash test.sh
```

---

## 6. Next Steps (Planned)

The current implementation satisfies only part of the full project spec. Remaining work identified:

- [ ] Refactor hardcoded response logic into a separate **mock model function**, decoupled from `main`'s input loop.
- [ ] Add **context management**: store the last 5 conversation turns using dynamically allocated memory.
- [ ] Add **tool execution**: detect calculation requests (e.g., "calculate 5 + 3") and compute results directly in C.
- [ ] Expand `test.sh` to validate conversation state management.
- [ ] Add a `valgrind`-based memory leak check once dynamic memory is introduced:
  ```bash
  sudo apt install valgrind
  valgrind --leak-check=full ./harness < input.txt
  ```
- [ ] Write `README.md` and push repository (source, tests, logs) to GitHub.
