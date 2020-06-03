g++ ./joker.cpp -o run-joker
echo "***Сompilation completed***"

FILES=./test/*
for test in $FILES
do
	echo "INPUT:"
	cat $test
	echo "OUTPUT:"
	./run-joker < $test
	echo "========================"
done
    pause
    cmd