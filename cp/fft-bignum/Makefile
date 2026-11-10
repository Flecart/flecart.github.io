CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
DBGFLAGS  = -std=c++17 -O0 -g -Wall -Wextra -fsanitize=address,undefined -D_GLIBCXX_DEBUG

.PHONY: all run samples check debug clean
all: solution test

solution: solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: test.cpp solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ test.cpp

# make run            -> type A and B on stdin
run: solution
	./solution

# make samples        -> diff solution against tests/*.in / *.out
samples: solution
	@pass=0; fail=0; \
	for f in tests/*.in; do \
	  exp=$${f%.in}.out; \
	  if ./solution < $$f | diff -q - $$exp >/dev/null; then pass=$$((pass+1)); echo "  [OK]   $$f"; \
	  else fail=$$((fail+1)); echo "  [FAIL] $$f"; echo "    got:  $$(./solution < $$f)"; echo "    want: $$(cat $$exp)"; fi; \
	done; echo "$$pass passed, $$fail failed"; [ $$fail -eq 0 ]

# make check [SUITE=edge|random|stress|big] [SEED=123]
check: test
	./test $(or $(SUITE),all) $(SEED)

# same, built with sanitizers (catches out-of-bounds, UB) — skip 'big', it's slow here
debug: test.cpp solution.cpp
	$(CXX) $(DBGFLAGS) -o test_debug test.cpp
	./test_debug $(or $(SUITE),edge)

clean:
	rm -f solution test test_debug
