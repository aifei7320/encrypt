
target=encrypt
CXXFLAGS+=-lcrypto
CC=g++
RM= rm -rf

obj=main.o encrypt.o

encrypt:${obj}
	${CC} -o encrypt ${obj} ${CXXFLAGS}

${obj}:%.o:%.cpp
	${CC} -c $< ${CXXFLAGS} 

.PONEY clean:
	${RM} *.o
