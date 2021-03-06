# echo make clean
# make clean

# echo make
make

echo do1 Write File
./cirTest -F ./tests.script/do1 &> ./output/do1
diff ./output/do1 ./output/do1-ref

# echo do1a Write File
# ./cirTest -F ./tests.script/do1a &> ./output/do1a
# diff ./output/do1a ./output/do1a-ref

echo do2 Read all AIG
./cirTest -F ./tests.script/do2 &> ./output/do2
diff ./output/do2 ./output/do2-ref

echo do3 Print Netlist
./cirTest -F ./tests.script/do3 &> ./output/do3
diff ./output/do3 ./output/do3-ref

echo do4 Print Gate and Line Number
./cirTest -F ./tests.script/do4 &> ./output/do4
diff ./output/do4 ./output/do4-ref

echo do5 Print Gate and Line Number II
./cirTest -F ./tests.script/do5 &> ./output/do5
diff ./output/do5 ./output/do5-ref

echo do6 Print -fanin
./cirTest -F ./tests.script/do6 &> ./output/do6
diff ./output/do6 ./output/do6-ref

echo do7 Print -fanout
./cirTest -F ./tests.script/do7 &> ./output/do7
diff ./output/do7 ./output/do7-ref

echo do8 Error Prevention
./cirTest -F ./tests.script/do8 &> ./output/do8
diff ./output/do8 ./output/do8-ref

echo do9 Error Prevention
./cirTest -F ./tests.script/do9 &> ./output/do9
diff ./output/do9 ./output/do9-ref

echo do10 Error Prevention
./cirTest -F ./tests.script/do10 &> ./output/do10
diff ./output/do10 ./output/do10-ref

echo do11 Advanced Print -fanout
./cirTest -F ./tests.script/do11 &> ./output/do11
diff ./output/do11 ./output/do11-ref
