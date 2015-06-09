#include <stdarg.h>
#include <stdint.h>
#include "./include/stdlib.h"
#include "./include/stdio.h"
#include "./include/ctype.h"
#include "./include/string.h"
#include "../syscallModule/include/syscall.h"

FILE _stdin = { STDIN_FILENO };
FILE _stdout = { STDOUT_FILENO };

FILE *stdin = &_stdin;
FILE *stdout = &_stdout;

int
putc(int ch, FILE *stream)
{
    if (write(stream->fd, (void *)&ch, 1) == -1)
        return EOF;
    return ch;
}

int
getc(FILE *stream)
{
    char ch;
    while (!read(stream->fd, (void *)&ch, 1))
    {
        _hlt();
    }
    return (int)ch;
}

int
puts(const char *str)
{
    int res;
    while (*str != '\0')
    {
        res = putchar(*str);
        if (res == -1)
            return EOF;
        str++;
    }
    res = putchar('\n');
    if (res == -1)
        return EOF;
    return 0;
}

int
putchar(int ch)
{
    return putc(ch, stdout);
}

int
getchar(void)
{
    return getc(stdin);
}

char *
gets(char *s, int size)
{
    return fgets(s, size, stdin);
}


char *
fgets(char *s, int size, FILE *stream)
{
    char c = getc(stream);
    int i = 0; 

    while (i < size - 1 && c != '\n' && c != EOF){
        if (c == '\b') {
            if (i > 0) {
                putchar(c);
                i--;
            }
        } else {
            s[i++] = c;
            putchar(c);
        }        
        c = getc(stream);
    }
    s[i] = 0;
    if (i > 0)
    {
        putchar('\n');
    }
    return (i != 0) ? s : NULL;
}

int
printf(const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    return vfprintf(stdout, fmt, ap);
    va_end(ap);

    return retval;
}

int
fprintf(FILE *f, const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = vfprintf(f, fmt, ap);
    va_end(ap);

    return retval;
}

int
printf_s(char *str, FILE *f)
{
    int i = 0;
    while (str[i] != '\0')
    {
        putc(str[i++], f);
    }
    return i;
}

int
printf_d(int i, FILE *f)
{
    char buf[32];
    itoa(i, buf, 10);
    return printf_s(buf, f);
}

int
printf_u(unsigned int i, FILE *f)
{
    char buf[32];
    utoa(i, buf, 10);
    return printf_s(buf, f);
}

int
printf_x(unsigned int i, FILE *f)
{
    char buf[32];
    utoa(i, buf, 16);
    strtolower(buf);
    return printf_s(buf, f);
}

int
printf_X(unsigned int i, FILE *f)
{
    char buf[32];
    utoa(i, buf, 16);
    return printf_s(buf, f);
}

int
vfprintf(FILE *f, const char *fmt, va_list ap)
{
    int i = 0;
    int acum = 0;

    while (fmt[i] != '\0')
    {
        if (fmt[i] == '%')
        {
            i++;
            switch (fmt[i])
            {
                case 's':
                    acum += printf_s(va_arg(ap, char *), f);
                    break;
                case 'd':
                    acum += printf_d(va_arg(ap, int), f);
                    break;
                case 'u':
                    acum += printf_u(va_arg(ap, unsigned int), f);
                    break;
                case 'x':
                    acum += printf_x(va_arg(ap, unsigned int), f);
                    break;
                case 'X':
                    acum += printf_X(va_arg(ap, unsigned int), f);
                    break;
                case 'c':
                    putc(va_arg(ap, int), f);
                    acum++;
                    break;
                case '%':
                    putc('%', f);
                    acum++;
                    break;
                default:
                    return -1;
            }
        }
        else
        {
            putc(fmt[i], f);
            acum++;
        }
        i++;
    }
    return acum;
}

int
read(int fd, void *buf, int count)
{
    return sc_read(fd, buf, count);
}

int
write(int fd, void *buf, int count)
{
    return sc_write(fd, buf, count);
}