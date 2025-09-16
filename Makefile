GXX = cc 
FLAGS = -shared -fPIC
OUT = liblibv.so

$(OUT)	:	library.c 
	$(GXX)	library.c	$(FLAGS)	-o	$(OUT)

.PHONY	:	clean
	rm	$(OUT)
