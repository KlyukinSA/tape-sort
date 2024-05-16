`test/TempTapeTest.cpp` содержит unit test для TempTape.

```sh
make && ./prog test/input2.txt test/output.txt
make && rm test/output.txt && ./prog test/input1.txt test/output.txt && cat test/output.txt
make clean && make && rm test/output.txt && ./prog test/input1.txt test/output.txt && cat test/output.txt
cat tmp/0.txt
```
