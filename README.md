`test/TempTapeTest.cpp` содержит mock класса FileTape. Лучше конечно сделать класс-наследник класса FileTape и подставить его в тестируемый объект TempTape, но это не проходит этап компиляции, так как TempTape владеет объектом FileTape ради оптимизации в ExternalSorter.

```sh
make && ./prog test/input2.txt test/output.txt
make && rm test/output.txt && ./prog test/input1.txt test/output.txt && cat test/output.txt
cat tmp/0.txt
```
