// Test harness for solution.cpp — you should not need to edit this file.
//
//   ./test                 run every suite
//   ./test edge|random|stress|big   run one suite
//   ./test random 1234     fix the RNG seed (default: random, printed on start)
//
// Suites
//   edge   : hand-picked corner cases (zeros, ones, carries, powers of ten)
//   random : many small/medium random cases vs. a slow O(n*m) reference
//   stress : bigger cases (up to ~20k digits) vs. the reference
//   big    : 10^5 .. 10^6 digit cases, timed; checked with modular hashing
//            because the reference would be far too slow
#define LOCAL_TEST_HARNESS
#include "solution.cpp"

namespace harness {

// ----------------------------------------------------------------- reference
// Schoolbook multiplication in base 1e9. Slow but obviously correct.
string reference(const string& a, const string& b) {
    auto to_limbs = [](const string& s) {
        vector<uint64_t> v;
        for (int i = (int)s.size(); i > 0; i -= 9) {
            int st = max(0, i - 9);
            v.push_back(stoull(s.substr(st, i - st)));
        }
        return v;
    };
    const uint64_t BASE = 1000000000ULL;
    auto x = to_limbs(a), y = to_limbs(b);
    vector<uint64_t> r(x.size() + y.size() + 1, 0);
    for (size_t i = 0; i < x.size(); ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < y.size() || carry; ++j) {
            uint64_t cur = r[i + j] + carry + (j < y.size() ? x[i] * y[j] : 0);
            r[i + j] = cur % BASE;
            carry = cur / BASE;
        }
    }
    while (r.size() > 1 && r.back() == 0) r.pop_back();
    string out = to_string(r.back());
    char buf[16];
    for (int i = (int)r.size() - 2; i >= 0; --i) {
        snprintf(buf, sizeof buf, "%09llu", (unsigned long long)r[i]);
        out += buf;
    }
    return out;
}

// ------------------------------------------------------- modular verification
// Checks (a mod p)(b mod p) == (res mod p) for a few large primes.
// A wrong answer slips through with negligible probability.
uint64_t mod_of(const string& s, uint64_t p) {
    unsigned __int128 r = 0;
    for (char c : s) r = (r * 10 + (c - '0')) % p;
    return (uint64_t)r;
}
bool well_formed(const string& s) {
    if (s.empty()) return false;
    for (char c : s) if (c < '0' || c > '9') return false;
    return s == "0" || s[0] != '0';
}
bool modular_check(const string& a, const string& b, const string& res) {
    static const uint64_t P[] = {2305843009213693951ULL /*2^61-1*/,
                                 1000000000000000003ULL, 999999999999999989ULL};
    for (uint64_t p : P) {
        unsigned __int128 lhs = (unsigned __int128)mod_of(a, p) * mod_of(b, p) % p;
        if ((uint64_t)lhs != mod_of(res, p)) return false;
    }
    // length sanity: |a|+|b|-1 <= |res| <= |a|+|b| (unless result is 0)
    if (res != "0" && (res.size() + 1 < a.size() + b.size() || res.size() > a.size() + b.size()))
        return false;
    return true;
}

// ---------------------------------------------------------------- generators
mt19937_64 rng;
string rand_num(size_t len) {
    if (len == 0) return "0";
    string s(len, '0');
    s[0] = char('1' + rng() % 9);
    for (size_t i = 1; i < len; ++i) s[i] = char('0' + rng() % 10);
    return s;
}
string all_nines(size_t len) { return string(len, '9'); }
string pow10(size_t k) { return "1" + string(k, '0'); }

// ------------------------------------------------------------------ reporting
string shorten(const string& s, size_t lim = 60) {
    if (s.size() <= lim) return "\"" + s + "\"";
    return "\"" + s.substr(0, lim / 2) + "..." + s.substr(s.size() - lim / 2) +
           "\" (" + to_string(s.size()) + " digits)";
}
int passed = 0, failed = 0;
const int MAX_PRINTED_FAILS = 5;

void report_fail(const string& suite, const string& a, const string& b,
                 const string& got, const string& want) {
    ++failed;
    if (failed > MAX_PRINTED_FAILS) return;
    cout << "  [FAIL] " << suite << "\n"
         << "    a    = " << shorten(a) << "\n"
         << "    b    = " << shorten(b) << "\n"
         << "    got  = " << shorten(got) << "\n";
    if (!want.empty()) {
        cout << "    want = " << shorten(want) << "\n";
        size_t i = 0;
        while (i < got.size() && i < want.size() && got[i] == want[i]) ++i;
        cout << "    first difference at index " << i << "\n";
    }
}

bool check_exact(const string& suite, const string& a, const string& b) {
    string want = reference(a, b), got;
    try { got = multiply(a, b); }
    catch (const exception& e) { got = string("<exception: ") + e.what() + ">"; }
    if (got == want) { ++passed; return true; }
    report_fail(suite, a, b, got, want);
    return false;
}

// --------------------------------------------------------------------- suites
void suite_edge() {
    cout << "[edge]\n";
    vector<pair<string, string>> cases = {
        {"0", "0"}, {"0", "1"}, {"1", "0"}, {"1", "1"}, {"0", "123456789"},
        {"2", "3"}, {"9", "9"}, {"12", "34"}, {"99", "99"}, {"10", "10"},
        {"123456789", "987654321"}, {"1000000", "1"},
        {all_nines(50), all_nines(50)}, {all_nines(1), all_nines(300)},
        {pow10(100), pow10(57)}, {pow10(0), all_nines(1000)},
        {all_nines(2000), all_nines(2000)},          // max carries everywhere
        {"31415926535897932384626433832795", "27182818284590452353602874713527"},
    };
    for (auto& [a, b] : cases) check_exact("edge", a, b);
}

void suite_random(int n, size_t maxlen, const char* name) {
    cout << "[" << name << "]\n";
    for (int t = 0; t < n; ++t) {
        size_t la = 1 + rng() % maxlen, lb = 1 + rng() % maxlen;
        // sometimes very unbalanced, sometimes zero
        if (rng() % 10 == 0) lb = 1 + rng() % 3;
        string a = rand_num(la), b = rand_num(lb);
        if (rng() % 50 == 0) a = "0";
        if (rng() % 20 == 0) b = all_nines(lb);
        check_exact(name, a, b);
    }
}

void suite_big() {
    cout << "[big]  (timed, verified mod 3 large primes)\n";
    vector<pair<size_t, size_t>> sizes = {
        {100000, 100000}, {300000, 250000}, {1000000, 1000000}, {1000000, 10}};
    for (auto [la, lb] : sizes) {
        string a = rand_num(la), b = rand_num(lb);
        string got;
        auto t0 = chrono::steady_clock::now();
        try { got = multiply(a, b); }
        catch (const exception& e) { got = string("<exception: ") + e.what() + ">"; }
        double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
        bool ok = well_formed(got) && modular_check(a, b, got);
        printf("  %7zu x %7zu digits : %9.1f ms  %s\n", la, lb, ms, ok ? "OK" : "WRONG");
        if (ok) ++passed; else report_fail("big", a, b, got, "");
    }
    // all-nines is the worst case for floating-point precision
    string a = all_nines(1000000);
    auto t0 = chrono::steady_clock::now();
    string got = multiply(a, a);
    double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
    bool ok = well_formed(got) && modular_check(a, a, got);
    printf("  %7d x %7d nines  : %9.1f ms  %s\n", 1000000, 1000000, ms, ok ? "OK" : "WRONG");
    if (ok) ++passed; else report_fail("big (all nines)", a, a, got, "");
}

}  // namespace harness

int main(int argc, char** argv) {
    using namespace harness;
    string which = argc > 1 ? argv[1] : "all";
    uint64_t seed = argc > 2 ? stoull(argv[2])
                             : (uint64_t)chrono::steady_clock::now().time_since_epoch().count();
    rng.seed(seed);
    cout << "seed = " << seed << "   (re-run with: ./test " << which << " " << seed << ")\n";

    bool all = which == "all";
    if (all || which == "edge")   suite_edge();
    if (all || which == "random") suite_random(500, 60, "random");
    if (all || which == "stress") suite_random(40, 20000, "stress");
    if (all || which == "big")    suite_big();

    if (failed > MAX_PRINTED_FAILS)
        cout << "  ... " << failed - MAX_PRINTED_FAILS << " more failures not shown\n";
    cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
