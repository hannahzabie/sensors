#include "scanner.h"
#include "fmt.h"

void Scanner::Init() {
    lookahead = -1;
    tokpos = -1;
}

// TODO Decide if we should filter nonprintable characters out.
char Scanner::Next() {
    lookahead = -1;

    while (lookahead < 0) {
        delay(10);
        lookahead = SerialUSB.read();
    }

    return lookahead;
}

char Scanner::Peek() {
    if (lookahead < 0) {
        Next();
    }

    return lookahead;
}

char Scanner::Scan() {
    int c = Peek();

    tokpos = -1;

    while (isspace(c) || c == '\0') {
        if (c == '\n' || c == '\r' || c == '\0') {
            lookahead = -1;
            return '\n';
        }

        c = Next();
    }

    if (isgraph(c)) {
        tokpos = 0;

        while (isgraph(c)) {
            tok[tokpos++] = c;
            c = Next();
        }

        tok[tokpos] = '\0';
    }

    return 0;
}

const char *Scanner::TokenText() const {
    if (tokpos < 0) {
        return "";
    } else {
        return tok;
    }
}
