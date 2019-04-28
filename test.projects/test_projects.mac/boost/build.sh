exe=echo $1|awk -F'.' '{print $1}'
g++ $1 -o exe --std=c++11
