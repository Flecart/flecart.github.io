// ============================================================================
//  Big-number multiplication with the Fast Fourier Transform
// ----------------------------------------------------------------------------
//  Problem
//    Given two non-negative integers A and B in decimal (up to ~10^6 digits
//    each, no leading zeros unless the number is exactly "0"), print A * B.
//
//  Input   : two lines, A and B
//  Output  : one line, A * B (no leading zeros; "0" if the product is zero)
//
//  The ONLY thing the test harness needs is:
//      std::string multiply(const std::string& a, const std::string& b);
//  Everything else below is a suggestion — rewrite it however you like
//  (iterative FFT, NTT, different base, splitting into 3-digit chunks, ...).
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

using cd = complex<double>;
const double PI = acos(-1.0);

// Suggested helper: in-place FFT on a power-of-two sized array.
// invert == false -> forward transform, invert == true -> inverse transform
// (remember to divide by n in the inverse).
void fft(vector<cd>& a, bool invert) {
    // TODO: implement (bit-reversal permutation + butterflies)
    (void)a; (void)invert;
}

// Suggested helper: multiply two polynomials (coefficients little-endian).
vector<long long> poly_multiply(const vector<int>& a, const vector<int>& b) {
    // TODO: pad to power of two >= a.size()+b.size(), fft both, pointwise
    //       multiply, inverse fft, round to nearest integer.
    (void)a; (void)b;
    return {};
}

// REQUIRED: the function under test.
string multiply(const string& a, const string& b) {
    // TODO: digits -> polynomial, poly_multiply, carry propagation,
    //       strip leading zeros, back to string.
    (void)a; (void)b;
    return "";
}

// ---------------------------------------------------------------------------
// CP-style entry point. Excluded when the test harness #includes this file.
// ---------------------------------------------------------------------------
#ifndef LOCAL_TEST_HARNESS
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string a, b;
    cin >> a >> b;
    cout << multiply(a, b) << '\n';
    return 0;
}
#endif
