#include <iostream>
#include "../FileTape.hpp"

#include <cassert>

int main(int argc, char* argv[]) {
    FileTape tape("file_tape.txt", std::ios::trunc);
    tape.write(2);
    tape.shift(1);
    tape.write(3);
    tape.shift(1);
    assert(!tape.isFinished());
    int val;
    tape.read(val); 
    assert(tape.isFinished());

    tape.rewind();
    assert(!tape.isFinished());
    tape.read(val);
    assert(val == 2);
    
    tape.shift(1);
    tape.shift(1);
    tape.read(val); 
    assert(tape.isFinished());
    tape.write(4);
    assert(!tape.isFinished());
    tape.read(val);
    tape.shift(1);
    assert(!tape.isFinished());
    assert(val == 4);
    
    
    return 0;
}
