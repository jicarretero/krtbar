source_dir=src
tmp_dir=tmp
build_dir=build
LIBS=-lX11 -lcjson

all: 
	gcc $(DEBUG) bar.c util.c kbarsignals.c klog.c modules/*.c -o krtbar $(LIBS)

clean:
	rm -rf krtbar 

debug: DEBUG = -g

debug: all

install: all
	cp -f krtbar /usr/local/bin
