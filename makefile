CC = gcc

LIBXML_CFLAGS := $(shell pkg-config --cflags libxml-2.0)
PTHREAD_CFLAGS := $(shell pkg-config --cflags libevent_pthreads)

CURL_LDFALGS := $(shell pkg-config --libs libcurl)
LIBXML_LDFLAGS := $(shell pkg-config --libs libxml-2.0)
PTHREAD_LDFLAGS := $(shell pkg-config --libs libevent_pthreads)

CFLAGS := -Wall -g $(LIBXML_CFLAGS) $(PTHREAD_CFLAGS)
LDFLAGS := $(CURL_LDFALGS) $(LIBXML_LDFLAGS) $(PTHREAD_LDFLAGS)

SDIR = src
ODIR = obj
HDIR = includes

SDIRS = $(wildcard $(SDIR)/*.c)
ODIRS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SDIRS))
HDRS = $(wildcard $(HDIR)/*.h)

BIN = main

all: $(BIN)

$(BIN): $(ODIRS) $(ODIR) $(HDRS)
	$(CC) $(CFLAGS) $(ODIRS) -o $@ $(LDFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR):
	mkdir -p $@

docs:
ifneq ("$(wildcard ./docs)", "")
	rm -rf docs
endif
	doxygen .doxygen

clean:
	rm -fr $(ODIR)
	rm -fr *.tex
ifneq ("$(wildcard ./$(BIN))","")
	rm $(BIN)
endif
