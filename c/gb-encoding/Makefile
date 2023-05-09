UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	test_options := -I/opt/homebrew/include -L/opt/homebrew/lib -liconv
  perf_options := -liconv
endif
test: test.c
	clang -o test test.c gb18030.c naive.c -Wall -lcriterion $(test_options)
perf: perf.c
	clang -o perf perf.c gb18030.c naive.c util.c -Wall -O3 $(perf_options)
