srcs	=$(wildcard *.c)
outs=$(srcs:%.c=%)

.PHONY: all
all: ${outs}

.PHONY: clean
clean:
	    rm -rf $(srcs:%.c=%.o) $(outs)
