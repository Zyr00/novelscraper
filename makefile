CC = gcc

LIBXML_CFLAGS := $(shell pkg-config --cflags libxml-2.0)

CURL_LDFALGS := $(shell pkg-config --libs libcurl)
LIBXML_LDFLAGS := $(shell pkg-config --libs libxml-2.0)

CFLAGS := -Wall -g $(LIBXML_CFLAGS)
LDFLAGS := $(CURL_LDFALGS) $(LIBXML_LDFLAGS)

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
ifneq ("$(wildcard ./$(BIN))","")
	rm $(BIN)
endif


