test: test.c
	clang -o test test.c gb18030.c naive.c -Wall -lcriterion -liconv -I/opt/homebrew/include -L/opt/homebrew/lib
perf: perf.c
	clang -o perf perf.c gb18030.c naive.c util.c -Wall -O3 -liconv
