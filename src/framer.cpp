#include "framer.h"

const unsigned long TIMEOUT = 5000L;
const byte ESCAPE_MASK = 0x20;
const byte BBEGIN = 0xFE;
const byte BEND = 0xFC;
const byte BESCAPE = 0xFD;

bool ControlByte(byte b) {
    return (b == BBEGIN) || (b == BEND) || (b == BESCAPE);
}

Framer::Framer() {
    err = NULL;
}

const char *Framer::Err() const {
    return err;
}

void Framer::WriteFrame(byte *b, int n) {
    byte crc = 0;

    SerialUSB.write(BBEGIN);

    for (int i = 0; i < n; i++) {
        if (ControlByte(b[i])) {
            SerialUSB.write(BESCAPE);
            SerialUSB.write(b[i] ^ ESCAPE_MASK);
        } else {
            SerialUSB.write(b[i]);
        }
    }

    SerialUSB.write(crc);
    SerialUSB.write(BEND);
}

int Framer::ReadFrame(byte *data, int max) {
    int size = 0;

    // wait for frame start
    while (true) {
        byte b = ReadByte();

        if (err != NULL) {
            return 0;
        }

        if (b == BBEGIN) {
            break;
        }
    }

    // wait for data / end bytes.
    while (true) {
        if (size > max) {
            err = "frame too big";
            return 0;
        }

        byte b = ReadByte();

        if (err != NULL) {
            return 0;
        }

        if (b == BEND) {
            break;
        }

        if (b == BESCAPE) {
            data[size++] = ReadEscapedByte();
        } else {
            data[size++] = b;
        }
    }

    return size;
}

byte Framer::ReadByte() {
    unsigned long start = millis();

    while (SerialUSB.available() == 0) {
        if (millis() - start > TIMEOUT) {
            err = "read timeout";
            return 0;
        }
    }

    return SerialUSB.read();
}

byte Framer::ReadEscapedByte() {
    byte b = ReadByte();

    if (err != NULL) {
        return 0;
    }

    // control bytes should never follow escape byte.
    if (ControlByte(b)) {
        err = "invalid escaped byte";
        return 0;
    }

    return b ^ ESCAPE_MASK;
}
