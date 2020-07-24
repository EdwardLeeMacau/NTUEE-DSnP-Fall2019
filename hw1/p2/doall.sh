echo make
make

echo do1 
./p2Run < ./dofiles/do1 &> ./output/do1.out
diff ./output/do1.out ./output/do1.ref

echo do2
./p2Run < ./dofiles/do2 &> ./output/do2.out
diff ./output/do2.out ./output/do2.ref

echo do3 
./p2Run < ./dofiles/do3 &> ./output/do3.out
diff ./output/do3.out ./output/do3.ref

echo do4 
./p2Run < ./dofiles/do4 &> ./output/do4.out
diff ./output/do4.out ./output/do4.ref

