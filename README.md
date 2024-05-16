`test/TempTapeTest.cpp` содержит unit test для TempTape.

```sh
mkdir tmp
echo "readDelay=1 // microseconds" > test/config.txt

make && ./prog test/input2.txt test/output.txt
make && rm test/output.txt && ./prog test/input1.txt test/output.txt && cat test/output.txt
make clean && make && rm test/output.txt && ./prog test/input1.txt test/output.txt && cat test/output.txt

g++ TempTapeTest.cpp && ./a.out
g++ FileTapeTest.cpp ../FileTape.cpp && ./a.out
g++ config.cpp ../FileTapeConfig.cpp && ./a.out
```

По умолчанию N = 42 (числа), M = 28 (байт). Если указать 3-им параметром N и 4-ым M то они зададутся.

`./prog test/input2.txt test/output.txt 42 28`
