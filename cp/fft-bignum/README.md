# FFT big-number multiplication — practice environment

Implement `std::string multiply(const std::string& a, const std::string& b)` in
`solution.cpp` (non-negative decimal integers, up to ~10^6 digits each).
The `fft` / `poly_multiply` stubs are only suggestions; change whatever you like,
as long as `multiply` keeps its signature.

| command | what it does |
|---|---|
| `make run` | build and run `solution` on stdin (CP style: A and B on two lines) |
| `make samples` | diff `solution` against `tests/*.in` / `tests/*.out` (add your own) |
| `make check` | run every harness suite |
| `make check SUITE=edge` | one suite: `edge`, `random`, `stress`, `big` |
| `make check SUITE=random SEED=42` | reproduce a failing random run |
| `make debug` | harness with AddressSanitizer + UBSan + `_GLIBCXX_DEBUG` (edge suite by default) |

Suites:
- **edge**: zeros, ones, carries, powers of ten, long runs of 9s.
- **random** / **stress**: random cases up to 60 / 20 000 digits, compared exactly with a slow schoolbook reference.
- **big**: 10^5 to 10^6 digits, timed, checked modulo three ~61-bit primes plus a length check.
  1e6 × 1e6 all-nines is the hardest case for floating-point precision.

The first 5 failures are printed with the first differing index. The seed is printed so you can re-run the same cases.
