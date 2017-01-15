#compiler
CC = gcc

#compiler flags
CFLAGS = -g

#build executable
TARGET = lab0

#project files for tarball
proj_files = lab0.c Makefile README *.png

default:
	$(CC) $(CFLAGS) -o $(TARGET) lab0.c

clean:
	rm -f $(TARGET) *.o *.tar.gz *.txt

check: default tests
	if [-s err.txt]
	then
	else echo "tests unsuccesful, check err.txt for errors."
	fi
	rm -f *.txt

dist: default
	tar -czf lab0--704635527.tar.gz $(proj_files)

tests: NoWriteTest NoReadTest IOtest catchTest diffTest


NoWriteTest:
	echo "1234567890abcdefghijklmnopqrstuvwxyz" > test.txt
	touch out.txt
	chmod 0000 out.txt
	./lab0 --output=out.txt --input=test.txt
	if [[ $$? -ne 2 ]] then echo "File is unwriteable." >>err.txt
	fi
	rm -f test.txt
NoReadTest:
	echo "1234567890abcdefghijklmnopqrstuvwxyz" > test.txt 
	chmod 0000 test.txt
	./lab0 --input=test.txt
	if [[ $$? -ne 1 ]] then echo "File is unreadable." >>err.txt
	fi
	rm -f test.txt
IOtest:
	echo "1234567890abcdefghijklmnopqrstuvwxyz" > test.txt
	./lab0 --input=test.txt --output=err.txt 
	if [[ $$? -ne 0 ]]then echo "IO incomplete." >>err.txt 
	fi
catchTest:
	./lab0 --segfault --catch
	if [[$$? -ne 3]] then echo "Could not catch seg fault" >>err.txt
	fi
diffTest:
	echo "1234567890abcdefghijklmnopqrstuvwxyz" > test.txt
	./lab0 --input=test.txt --output=out.txt
	cmp test.txt out.txt
	if [[$$? -ne 0]] then echo "IO not the same" >>err.txt
	fi
	rm -f test.txt out.txt