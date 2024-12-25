g++ -c lab6_scanner.cpp -o scan
g++ -c lab6_elements.cpp -o elems
g++ elems scan -o main
rm -rf scan elems
./main 123.txt 234.txt