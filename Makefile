COPT=-g
LOPT=-g -lsndfile
OBJS=main.o readWAV.o
CC=g++

default: markov-music

clean:
	rm -f *.o markov-music

markov-music: ${OBJS}
	${CC} ${LOPT} ${OBJS} -o $@

%.o: %.cpp
	${CC} ${COPT} -c $<

