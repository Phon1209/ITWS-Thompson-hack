# Thompson's Hack

## Description

This project is part of RPI's ITWS-4370: Information System Security class. This project is about the bug(quine) that we can inject into the compiler to make it self-replicate.
The compiler used is [TinyCC](https://github.com/TinyCC/tinycc) by Fabrice Bellard.
For the bug we introduces here, we use the exact bug Thompson mentioned in his speech, which is login injection. This allows unautorized user to bypass the login system and get access without changing the source code.


```c++
static int
do_login(const char *username)
{
  // we can inject something here without changing the source code
  if (!strcmp(username, "root"))
  {
    return 0;
  }

  return 1;
}
```


This repository divided into 3 folders:
1. Quine - contains a helper function and quine testing utilities
2. cleantcc - contains a clean, unedited version of tcc snapshot
3. bugtcc - the modified version that contains quine attack
4. cleantcc-bugcompiler - a clean version but change the compiler to tcc

## Development Platform

This hack was developed and tested on a Window 10 PC.

## How it works

1. Compile a clean version of tcc on your system.
```
  ./configure
  make
  make install
```

2. If we compile `Attack/login.c` right away, the only username that will work is `root` which is expected.

3. In `bugtcc/libtcc.c`, we modified it so that if it detect a file named `login.c`, it will create a temporary `login.c` file in the local directory and scan for a login signature below.
```c++
char login_sig[] = "do_login(const char *username)\r\n{"            
```
If we found the signature, we can inject `if (!strcmp(username, \"phon\"))return 0;` after that signature to make it return `0` if the username is `phon`

4. Then build this bug compiler. Again use 
```
  ./configure
  make
  make install
```
to compile and install the bug compiler

5. Try the newly compiled compiler on `Attack/login.c` again, we should see the result executable to accept username `phon` as well.

6. But this compiler is not self-replicate, therefore, the next step is too create a quine for out modified source. The quine string I used can be found in `Quine/attack_string.txt`.

7. With the same trick as `login attack` we can detect the `libtcc.c` file and inject our quine into it.

8. Now, we can use the compiler from `bugtcc` to compile `cleantcc-bugcompiler`.
and the result compiler for `cleantcc-bugcompiler` will also contains the attack.

9. We can test that by compile `Attack/login.c` again with the newly created `cleantcc-bugcompiler/tcc`, it should accept the username `phon` as well.

#### Side Note

- `./configure` will output `./config.h` that contains a line break, fix that by deleteing the line break;
- a full modified `tcc_compile` function can be found at [`bugtcc\libtcc.c:721`](https://github.com/Phon1209/ITWS-Thompson-hack/blob/main/bugtcc/libtcc.c)
