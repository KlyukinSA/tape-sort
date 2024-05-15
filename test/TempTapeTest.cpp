#include "../TempTape.hpp"

#include "lib/microtest.h"

int pos = 0;
int size = 0;
const int cap = 8;
int array[cap];
bool finished = false;

FileTape::FileTape(std::fstream&& file, const FileTapeConfig& config)
    : file(std::move(file))
    , config(config)
{}

bool FileTape::read(int& value) {
    if (pos == size) {
        finished = true;
        value = 0;
    } else {
        value = array[pos];
    }
    return true;
}

bool FileTape::write(int value) {
    array[pos] = value;
    if (pos == size) {
        size++;
    }
    return true;
}

bool FileTape::rewind() {
    pos = 0;
    return true;
}

bool FileTape::shift(int n) {
    if (n == 0) {
        return true;
    } else if (n > 0) {
        if (pos + n > size) {
            pos = size;
            finished = true;
            return false;
        }
    } else {
        finished = false;
        if (pos - n <= 0) {
            pos = 0;
            return false;
        }
    }
    pos += n;
    return true;
}

bool FileTape::isFinished() {
    return finished;
}

TEST(ExampleSucceedingTest) {
    TempTape tape{FileTape{std::fstream{}, FileTapeConfig{}}};
    // as usual tape
    tape.write(1112);
    tape.shift(1);
    tape.write(1113);   
    tape.shift(1);
    ASSERT(!tape.isFinished());
    int val;
    tape.read(val);  
    ASSERT(tape.isFinished());

    tape.rewind();
    ASSERT(!tape.isFinished());
    tape.read(val);
    ASSERT_EQ(val, 1112);
    
    tape.shift(1);
    tape.shift(1);
    tape.read(val); 
    ASSERT(tape.isFinished());
    tape.write(1114);
    ASSERT(!tape.isFinished());
    tape.read(val);
    tape.shift(1);
    ASSERT(!tape.isFinished());
    ASSERT_EQ(val, 1114);

    // with clean()
    tape.clean();
    ASSERT(!tape.isFinished());
    tape.read(val);
    ASSERT_EQ(val, 0);
    ASSERT(tape.isFinished());
    tape.shift(1);
    ASSERT(tape.isFinished());
    tape.write(511);
    ASSERT(!tape.isFinished());
    tape.read(val);
    ASSERT(!tape.isFinished());
    ASSERT_EQ(val, 511);
    tape.shift(1);
    ASSERT(!tape.isFinished());
    tape.read(val);
    ASSERT(tape.isFinished());

    tape.write(6);
    ASSERT(!tape.isFinished());
}

TEST_MAIN();
