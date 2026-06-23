# ft_ls

> A recreation of the Unix `ls` command, developed as part of the 42 curriculum.

## 📖 About

`ft_ls` is a custom implementation of the standard Unix `ls` utility. The goal of this project is to understand how filesystems work, how metadata is stored, and how directory contents can be read and displayed using low-level system calls.

This project focuses on:

- Directory traversal
- File metadata retrieval with `stat()`
- Sorting files according to different criteria
- Handling command-line options
- Reproducing the behavior of the original `ls` command as closely as possible

---

## ✨ Features

Implemented options:

- `-l` : long format listing
- `-R` : recursive listing
- `-a` : show hidden files
- `-r` : reverse sorting order
- `-t` : sort by modification time

Examples:

```bash
./ft_ls
./ft_ls -la
./ft_ls -R
./ft_ls -ltr
./ft_ls folder1 folder2
```

---

## 🛠️ Compilation

Compile the project using:

```bash
make
```

This will generate:

```bash
ft_ls
```

Available Makefile rules:

```bash
make
make clean
make fclean
make re
```

---

## 📂 Project Structure

```
.
├── includes/
├── src/
├── obj/
├── libft/
├── Makefile
└── README.md
```

---

## ⚙️ Allowed Functions

Among others, this project uses:

- `opendir`
- `readdir`
- `closedir`
- `stat`
- `lstat`
- `getpwuid`
- `getgrgid`
- `readlink`
- `time`
- Standard library functions

---

## 📚 What I Learned

Through this project, I learned about:

- Unix directory handling
- File permissions and metadata
- Sorting algorithms
- Time formatting
- Symbolic links
- Recursive directory traversal
- Reproducing the behavior of a real system utility

---

## 🎓 42 Project

`ft_ls` is part of the Unix branch of the Common Core at 42. The objective is to reproduce the behavior of the original `ls` command while deepening the understanding of Unix internals and system programming.

---

## 📜 License

This project was developed for educational purposes at 42.