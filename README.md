*This project has been created as part of the 42 curriculum by kabasolo.*

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

Extra options:

- `-g` : like -l, but it does not list owner
- `-G` : like -l, but it does not list group ([explanation](#why-g-differs-from-the-original))
- `-S` : sort by size
- `-U` : do not sort directory entries

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

---

## Why `-G` differs from the original

The -G option works a little differently from the original. In the original implementation, -G only has an effect when used together with -l.

Here, I chose to make -G work independently of -l.

For example:

```bash
ls -U
```

-U literally does nothing on its own.

```bash
ls -lU
```

Here, -U works because it is being used together with -l.

```bash
ls -u
```

On the other hand, -u, being a very similar flag, works without needing -l:
