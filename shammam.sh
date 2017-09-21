
mkdir -p tmp
cat shammam.cpp > tmp/code.cpp
cat $1 >> tmp/code.cpp
g++ tmp/code.cpp -Iinc -o tmp/shammam

./tmp/shammam

rm -rf tmp/*
