#ifndef TAPE_INTERFACE_H
#define TAPE_INTERFACE_H

class TapeInterface {
public:
    virtual bool read(int& value) = 0;
    virtual bool write(int value) = 0;
    virtual bool rewind() = 0;
    virtual bool shift(int n) = 0;
    virtual bool isFinished() = 0;
    virtual ~TapeInterface() {}
};

#endif
