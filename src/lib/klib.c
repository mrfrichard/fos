#include "proto.h"
#include "const.h"
#include "string.h"

PUBLIC char* itoa(char *str, int num) {
    char* p = str;
    char ch;
    BOOL flag = FALSE;

    *p++ = '0';
    *p++ = 'x';
    if (num == 0) {
        *p++ = '0';
    } else {
        for (int i = 28; i >= 0; i -= 4) {
            ch = (num >> i) & 0xf;
            if (flag || (ch > 0)) {
                flag = TRUE;
                ch += '0';
                if (ch > '9') {
                    ch += 7;
                }
                *p++ = ch;
            }
        }
    }
    *p = 0;
    return str;
}

PUBLIC char* ltoa(char *str, T64 num) {
    char* p = str;
    char ch;
    BOOL flag = FALSE;

    *p++ = '0';
    *p++ = 'x';
    if (num == 0) {
        *p++ = '0';
    } else {
        for (int i = 60; i >= 0; i -= 4) {
            ch = (num >> i) & 0xf;
            if (flag || (ch > 0)) {
                flag = TRUE;
                ch += '0';
                if (ch > '9') {
                    ch += 7;
                }
                *p++ = ch;
            }
        }
    }
    *p = 0;
    return str;
}



PUBLIC void disp_int(int v) {
    char str[16];
    memset(str, 0, 16);
    itoa(str, v);
    disp_str(str);
}

PUBLIC void disp_T64(T64 v) {
    char str[32];
    memset(str, 0, 32);
    ltoa(str, v);
    disp_str(str);
}

PUBLIC void disp_int_pos(int v, unsigned pos) {
    char str[16];
    memset(str, 0, 16);
    itoa(str, v);
    disp_str_pos(str, pos);
}

PUBLIC void disp_T64_pos(T64 v, unsigned pos) {
    char str[32];
    memset(str, 0, 32);
    ltoa(str, v);
    disp_str_pos(str, pos);
}

/*PUBLIC void delay(int time) {
    int i, j, k;
    for (k = 0; k < time; ++k) {
        for (i = 0; i < 10000; ++i) {
             for (j = 0; j < 10000; ++j) { }
        }
    }
}

PUBLIC void milli_delay(int milli_sec) {
    int t = get_ticks();
    while (((get_ticks() - t) * 1000 / HZ) < milli_sec){}
}


PUBLIC int vsprintf(char *buf, const char *fmt, va_list args) {
    char* p;
    char tmp[64];

    va_list p_next_arg =args;

    for (p=buf; *fmt; fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;
            continue;
        }

        fmt++;
        switch (*fmt) {
            case 'x':
                itoa(tmp, *((int*)p_next_arg));
                int len = strcpy(p, tmp);
                p_next_arg += 4;
                p += len;
                break;
            default:
                break;
        }
    }
    return (p - buf);
}

PUBLIC int printf(const char *fmt, ...) {
    int i;
    char buf[64];
    va_list args= (va_list)((char*)(&fmt) + 4);

    i = vsprintf(buf, fmt, args);
    write(buf, i);
    return i;
}*/

