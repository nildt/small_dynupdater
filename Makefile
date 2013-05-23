CC=	gcc
CFLAGS=	-Wall -Wstrict-prototypes -DNO_LINENOS -DNDEBUG
CFLAGSDEBUG= -Wall -Wstrict-prototypes -Wextra -g -pg -pg
LDLIBS = -lcurl
LDFLAGS = -L/usr/lib/x86_64-linux-gnu
SOURCES = src/small_dynupdater.c
OUT = bin/dynupdater

all: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)  -o $(OUT) $(LDFLAGS) $(LDLIBS)
debug: $(SOURCES)
	$(CC) $(CFLAGSDEBUG) $(SOURCES)  -o $(OUT).debug $(LDFLAGS) $(LDLIBS)


#	Some examples:
#	gcc -Wall -W -Wstrict-prototypes -Wextra -DNO_LINENOS -DNDEBUG small_dynupdater.c -o dynupdater -L/usr/lib/x86_64-linux-gnu -lcurl
#	gcc -Wall -W -Wstrict-prototypes -Wextra -g -DNO_LINENOS small_dynupdater.c -L/usr/lib/x86_64-linux-gnu -lcurl
#	gcc -Wall -W -Wstrict-prototypes -Wextra -DNO_LINENOS -DNDEBUG small_dynupdater.c -o dynupdater -L/usr/lib/x86_64-linux-gnu -lcurl
