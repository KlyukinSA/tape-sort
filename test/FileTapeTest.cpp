#include "../FileTape.hpp"

#include "lib/microtest.h"

TEST(ExampleSucceedingTest) {
    FileTape tape(std::fstream{"file_tape.txt", std::ios::in | std::ios::out | std::ios::trunc}, FileTapeConfig{0,0,0,0});
    tape.write(2);
    tape.shift(1);
    tape.write(3);
    tape.shift(1);
    ASSERT(!tape.isFinished());
    int val;
    tape.read(val); 
    ASSERT(tape.isFinished());

    tape.rewind();
    ASSERT(!tape.isFinished());
    tape.read(val);
    ASSERT_EQ(val, 2);
    
    tape.shift(1);
    tape.shift(1);
    tape.read(val); 
    ASSERT(tape.isFinished());
    tape.write(4);
    ASSERT(!tape.isFinished());
    tape.read(val);
    tape.shift(1);
    ASSERT(!tape.isFinished());
    ASSERT_EQ(val, 4);
}

TEST_MAIN();
