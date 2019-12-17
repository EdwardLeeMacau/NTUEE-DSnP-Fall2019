# echo make clean
# make clean

# echo make
make

echo do1 
./taskMgr -F ./dofiles/do1 &> ./output/do1
diff ./output/do1 ./output/do1-ref

# echo do2
# ./taskMgr -F ./dofiles/do2 &> ./output/do2
# diff ./output/do2 ./output/do2-ref

echo do3
./taskMgr -F ./dofiles/do3 &> ./output/do3
diff ./output/do3 ./output/do3-ref

echo do4
./taskMgr -F ./dofiles/do4 &> ./output/do4
diff ./output/do4 ./output/do4-ref

echo do5
./taskMgr -F ./dofiles/do5 &> ./output/do5
diff ./output/do5 ./output/do5-ref

echo do6
./taskMgr -F ./dofiles/do6 &> ./output/do6 
diff ./output/do6 ./output/do6-ref

# echo do7
# ./taskMgr -F ./dofiles/do7 &> ./output/do7
# diff ./output/do7 ./output/do7-ref
