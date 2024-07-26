APPNAME=nbiot

LIBPATH=./lib/
GPIODLIB=./install/lib
INCPATH=./inc/
GPIODINC=./install/include


CFLAGS+=-I${INCPATH}
CFLAGS+=-I${GPIODINC}


LDFLAGS+=-L${LIBPATH} 
LDFLAGS+=-L${GPIODLIB} 



CC=gcc

all:
		${CC} ${CFLAGS} -g main.c -o ${APPNAME} ${LDFLAGS} -lnbiot -lgpiod -pthread

clean:
		rm -f ${APPNAME}

run:
		sudo LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LIBPATH}  ./nbiot
