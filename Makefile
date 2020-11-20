# ---------------------------------------------------------------
C_SRCS = tz_test_ls.c ls_utils.c

OBJS = tz_test_ls.o ls_utils.o

EXECUTABLE = test_ls

.PHONY: all clean build test all

all: build test

build: $(OBJS) 
	@echo 'Building target: $@'
	gcc -o $(EXECUTABLE) $(OBJS) 

%.o: %.c
	@echo 'Building file: $<'
	gcc -c -o "$@" "$<"

clean:
	rm -rf $(OBJS) $(EXECUTABLE)

test:
	./$(EXECUTABLE)

