COPT=-g -O3 
LOPT=-g -O3 -lsndfile
OBJS=main.o WAVio.o markov.o magicMap.o mapMarkov.o
CC=g++

default: markov-music

clean:
	rm -f *.o markov-music

markov-music: ${OBJS}
	${CC} ${LOPT} ${OBJS} -o $@

%.o: %.cpp
	${CC} ${COPT} -c $<

