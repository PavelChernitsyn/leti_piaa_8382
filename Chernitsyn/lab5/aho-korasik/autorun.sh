g++ ./aho-korasik.cpp -o run-aho-korasik
echo "***Сompilation completed***"

FILES=./test/*
for test in $FILES
do
	echo "INPUT:"
	cat $test
	echo "OUTPUT:"
	./run-aho-korasik < $test
	echo "========================"
done
    pause
    cmd