# Makefile principal 

all: libdico wc

# Compile la lib 
libdico:
	$(MAKE) -C libdico

# Compile wc qui dépend de notre libdico
wc: libdico
	$(MAKE) -C wc

clean:
	$(MAKE) -C libdico clean
	$(MAKE) -C wc clean

.PHONY: all libdico wc clean