#include <iostream>
#include "../TempTape.hpp"

#include <cassert>

int main(int argc, char* argv[]) {
    TempTape tape("temp_tape.txt", std::ios::trunc);
    // as usual tape
    tape.write(1112);
    tape.shift(1);
    tape.write(1113);   
    tape.shift(1);
    assert(!tape.isFinished());
    int val;
    tape.read(val);  
    assert(tape.isFinished());

    tape.rewind();
    assert(!tape.isFinished());
    tape.read(val);
    assert(val == 1112);
    
    tape.shift(1);
    tape.shift(1);
    tape.read(val); 
    assert(tape.isFinished());
    tape.write(1114);
    assert(!tape.isFinished());
    tape.read(val);
    tape.shift(1);
    assert(!tape.isFinished());
    assert(val == 1114);

    // with clean()
    tape.clean();
    assert(!tape.isFinished());
    tape.read(val);
    assert(val == 0);
    assert(tape.isFinished());
    tape.shift(1);
    assert(tape.isFinished());
    tape.write(511);
    assert(!tape.isFinished());
    tape.read(val);
    assert(!tape.isFinished());
    assert(val == 511);
    tape.shift(1);
    assert(!tape.isFinished());
    tape.read(val);
    assert(tape.isFinished());

    tape.write(6);
    assert(!tape.isFinished());

}
