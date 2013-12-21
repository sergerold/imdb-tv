CC=g++
in=main.cpp
out=imdb.a
series=Series.o
episode=Episode.o
curlandclean=src/htmlfunctions/curlandclean.cpp
pugilib=src/pugixml/src/pugixml.o
curllib=src/libcurl/lib/.libs/libcurl.a
tidylib=src/tidy/tidy-html5-master/lib/libtidy.a
CFLAGS=-c 
C11FLAG=-std=c++11
MTFLAG=-fopenmp -lpthread
tempdir=src/temp

ARC=ar rcs

all: $(out)

$(out):$(series) $(episode) curlandclean.o $(pugilib) $(tempdir)
	ar rcs $(out) $(series) $(episode) curlandclean.o $(pugilib) $(tempdir)/*.o
	

$(tempdir):$(curllib) $(tidylib)
	mkdir $(tempdir)
	cd $(tempdir) && ar -x ../libcurl/lib/.libs/libcurl.a
	cd $(tempdir) && ar -x ../tidy/tidy-html5-master/lib/libtidy.a


	

curlandclean.o:$(curlandclean)
	$(CC) $(CFLAGS) $(curlandclean)

$(series):src/Series.cpp
	$(CC) $(CFLAGS) $(C11FLAG) $(MTFLAG) src/Series.cpp

$(episode):src/Episode.cpp
	$(CC) $(CFLAGS) src/Episode.cpp

$(curllib):
	cd src/libcurl && ./configure --disable-shared
	cd src/libcurl && $(MAKE)

$(pugilib):
	cd src/pugixml/src && $(CC) $(CFLAGS) pugixml.cpp

$(tidylib):
	cd src/tidy/tidy-html5-master && $(MAKE)

clean: 
	rm *.o

