#include <bits/stdc++.h>  // dev stage only

/*
 * ==========================
 * BEGIN BIGNUM LIB INTERFACE
 * ==========================
 */



/*
 * bignum type
 *
 * A big number is composed of a signal (positive or negative), a string
 * of characters representing the digits (0-9), the length of such string
 * (limited by size_t) and a decimal point position (as the index of '.'
 * in '<integral part>.<fractional part>', for example).
 *
 * Of all the possible implementations of big numbers of arbitrary length and
 * precision, this ranks among the worst, probably. Do not replicate this.
 */
struct bignum {

        bool            signal;         // positive or negative
        std::string     digits;         // actual number digits
        size_t          dpoint;         // decimal point position
        size_t          length;         // length of bignumber

        explicit bignum(bool signal = true, std::string digits = "", size_t dpoint = 0) :
                signal{signal},
                digits{digits},
                dpoint{dpoint},
                length{digits.size()}
        {
        }
};

// arithmetics
void bn_add(bignum &dest, const bignum &a, bignum &b);          // add bignums a and b, store result in dest
void bn_sub(bignum &dest, const bignum &a, bignum &b);          // subtract bignums a and b, store result in dest
void bn_mult(bignum &dest, const bignum &a, const bignum &b);   // multiply bignums a andm b, store result in dest
void bn_half(bignum &dest);                                     // divide bignum  dest by 2 in-place
void bn_inverse(bignum &dest);                                  // find mult-inverse of bignum dest in-place

// utilities
void bn_pad(bignum &a, bignum &b);                              // pad bignums a and b with zeroes so they're aligned (and same sized)
void bn_trim(bignum &number);                                   // trim trailing and leading zeroes from bignum number
int  bn_abscompare(const bignum &a, const bignum &b);           // compare absolute values of bignums a and b (as in a - b)
void bn_pc_to_bignum(bignum &dest, const size_t pc);            // convert positive constant pc into a bignum, store result in dest

// I/O
void bn_print(bignum &number);                                  // print an arbitrary length and precision bignum number



/*
 * =============
 * BEGIN PROGRAM
 * =============
 */



int main(int argc, char *argv[])
{
        #ifndef NDEBUG
        bignum number1{1, {2, 8, 4, 7, 9, 8, 4, 2, 6, 5, 3, 2, 0, 2}, 10};
        bignum number2{1, {8, 4, 9, 6, 7, 0, 4, 2, 1, 4, 5, 5, 2, 3}, 9};

        std::cout << std::endl;
        std::cout << "================" << std::endl;
        std::cout << "Test: Pad & Trim" << std::endl;
        std::cout << "================" << std::endl;
        {
                std::cout << "+ original" << std::endl;
                bn_print(number1);
                bn_print(number2);

                bn_pad(number1, number2);

                std::cout << "+ padded" << std::endl;
                bn_print(number1);
                bn_print(number2);

                bn_trim(number1);
                bn_trim(number2);

                std::cout << "+ trimmed" << std::endl;
                bn_print(number1);
                bn_print(number2);

                bignum testa{true, {0, 0, 0, 0, 0}, 2};
                bignum testb{true, {8, 0, 0, 0, 0}, 3};

                std::cout << "+ ultrim: original" << std::endl;
                bn_print(testa);
                bn_print(testb);

                bn_trim(testa);
                bn_trim(testb);

                std::cout << "+ ultrim: trimmed" << std::endl;
                bn_print(testa);
                bn_print(testb);

                bn_pad(testa, testb);

                std::cout << "+ ultrim: padded" << std::endl;
                bn_print(testa);
                bn_print(testb);
        }

        std::cout << std::endl;
        std::cout << "=========" << std::endl;
        std::cout << "Test: Add" << std::endl;
        std::cout << "=========" << std::endl;
        {
                std::cout << "+ original" << std::endl;
                bn_print(number1);
                bn_print(number2);

                bignum result;
                bn_pad(number1, number2);
                bn_add(result, number1, number2);
                bn_trim(result);

                std::cout << "+ result" << std::endl;
                bn_print(result);
        }

        std::cout << std::endl;
        std::cout << "=========" << std::endl;
        std::cout << "Test: Sub" << std::endl;
        std::cout << "=========" << std::endl;
        {
                std::cout << "+ original" << std::endl;
                bn_print(number2);
                bn_print(number1);

                bignum result;
                bn_pad(number1, number2);
                bn_sub(result, number2, number1);
                bn_trim(result);

                std::cout << "+ result" << std::endl;
                bn_print(result);

                std::cout << "+ original" << std::endl;
                bn_print(number1);
                bn_print(number2);

                bn_sub(result, number1, number2);
                bn_trim(result);

                std::cout << "+ result" << std::endl;
                bn_print(result);
        }

        std::cout << std::endl;
        std::cout << "==========" << std::endl;
        std::cout << "Test: Mult" << std::endl;
        std::cout << "==========" << std::endl;
        {
                std::cout << "+ original" << std::endl;
                bn_print(number1);
                bn_print(number2);

                bignum result;
                bn_mult(result, number1, number2);
                bn_trim(result);

                std::cout << "+ result" << std::endl;
                bn_print(result);
        }

        std::cout << std::endl;
        std::cout << "==========" << std::endl;
        std::cout << "Test: Half" << std::endl;
        std::cout << "==========" << std::endl;
        {
                bignum number{true, {1}, 1};
                std::cout << "+ original" << std::endl;
                bn_print(number);

                bn_half(number);
                std::cout << "+ result" << std::endl;
                bn_print(number);

                bn_half(number);
                std::cout << "+ result" << std::endl;
                bn_print(number);

                bn_half(number);
                std::cout << "+ result" << std::endl;
                bn_print(number);
        }

        std::cout << std::endl;
        std::cout << "=============" << std::endl;
        std::cout << "Test: Compare" << std::endl;
        std::cout << "=============" << std::endl;
        {
                bn_print(number1);
                bn_print(number2);
                bn_pad(number1, number2);
                int rc = bn_abscompare(number1, number2);
                std::cout << "+ comparison returns " << rc << std::endl;

                bn_print(number2);
                bn_print(number1);
                rc = bn_abscompare(number2, number1);
                std::cout << "+ comparison returns " << rc << std::endl;

                bn_print(number1);
                bn_print(number1);
                rc = bn_abscompare(number1, number1);
                std::cout << "+ comparison returns " << rc << std::endl;
        }

        std::cout << std::endl;
        std::cout << "==================" << std::endl;
        std::cout << "Test: Mult-Inverse" << std::endl;
        std::cout << "==================" << std::endl;
        {
                std::cout << "+ original" << std::endl;
                bn_print(number2);

                bignum result = number2;
                bn_inverse(result);
                bn_trim(result);

                std::cout << "+ result" << std::endl;
                bn_print(result);

                //

                bignum testa{true, {3}, 1};
                std::cout << "+ original" << std::endl;
                bn_print(testa);

                result = testa;
                bn_inverse(result);
                bn_trim(result);

                std::cout << "+ result" << std::endl;
                bn_print(result);

                //

                bignum testb{true, {4}, 1};
                std::cout << "+ original" << std::endl;
                bn_print(testb);

                result = testb;
                bn_inverse(result);
                bn_trim(result);

                std::cout << "+ result" << std::endl;
                bn_print(result);
        }
        #endif

        return EXIT_SUCCESS;
}



/*
 * =====================
 * BEGIN BIGNUM LIB IMPL
 * =====================
 */



/*
 * add bignum b to bignum b and store in bignum dest
 *
 * Bignums a and b _must_ be aligned in length and decimal point (see bn_pad)
 * and the result is not trimmed (see bn_trim).
 */
void bn_add(bignum &dest, const bignum &a, bignum &b)
{
        /*
         * (a) + (-b) = (a - b)
         * (-a) + (b) = -(a - b)
         */
        if (a.signal != b.signal) {
                // try to alter as little state as possible
                bignum c{!b.signal, b.digits, b.dpoint};
                bn_sub(dest, a, c);
                return;
        }

        dest.signal = a.signal;
        dest.digits = a.digits;
        dest.length = a.length + 1;
        dest.dpoint = a.dpoint + 1;

        // at most 1 digit longer
        dest.digits.insert(0, 1, 0);

        // now add elements in 'b' to current
        for (size_t i = b.length; i > 0; i--) {
                dest.digits[i] += b.digits[i - 1];
                dest.digits[i - 1] += dest.digits[i] / 10;
                dest.digits[i] = dest.digits[i] % 10;
        }
}


/*
 * subtract bignum b from bignum a and store in bignum dest
 *
 * Bignums a and b _must_ be aligned in length and decimal point (see bn_pad)
 * and the result is not trimmed (see bn_trim).
 */
void bn_sub(bignum &dest, const bignum &a, bignum &b)
{
        /*
         * (a) - (-b) = (a + b)
         * (-a) - (b) = -(a + b)
         */
        if (a.signal != b.signal) {
                // try to alter as little state as possible
                bignum c{!b.signal, b.digits, b.dpoint};
                bn_add(dest, a, c);
                return;
        }

        dest.signal = a.signal;  // if a is bigger, the signal maintains
        dest.digits = a.digits;
        dest.length = a.length + 1;
        dest.dpoint = a.dpoint + 1;

        // might be needed for borrowing, if b > a
        dest.digits.insert(0, 1, 0);

        // now sum elements in 'b' from current
        for (size_t i = b.length; i > 0; i--) {
                dest.digits[i] -= b.digits[i - 1];

                // borrow if needed
                if (dest.digits[i] < 0) {
                        dest.digits[i] += 10;
                        dest.digits[i - 1] -= 1;
                }
        }

        // result may be negative
        if (dest.digits[0] < 0) {
                // minor (guaranteed) positive comp
                std::string pcomp_digits{dest.digits, 1, dest.length - 1};
                pcomp_digits.insert(0, 1, 0); // adjust length
                bignum pcomp{dest.signal, pcomp_digits, dest.dpoint};

                // major (guaranteed) negative comp
                dest.digits[0] = -dest.digits[0];
                dest.digits.replace(1, dest.length - 1, dest.length - 1, '\0');

                // readjust arithmetic
                bn_sub(dest, dest, pcomp);
                dest.signal = !dest.signal;  // ...now the signal is proper
        }
}


void bn_mult(bignum &dest, const bignum &a, const bignum &b)
{
        /*
         * length of x ~~ log10(x) + 1 (or, at most one-off from it)
         *
         * The length of the resulting multiplication a*b would be
         * log10(a) + log10(b) + 1. If I already know the lenghts of a and b
         * (which I do), computing the log10 of each can be skipped. Length of a
         * + length of b + 1 guarantees at least 1 leading unused digit.
         */
        dest.signal = (a.signal == b.signal);
        dest.length = a.length + b.length + 1;
        dest.digits = std::string(dest.length, '\0');
        dest.dpoint = dest.length
                - (a.length - a.dpoint)
                - (b.length - b.dpoint);

        // now do simple simple multiply
        //#pragma omp parallel for collapse(2)
        for (size_t i = b.length; i > 0; i--) {
                for (size_t j = a.length; j > 0; j--) {
                        dest.digits[i + j] += b.digits[i - 1] * a.digits[j - 1];
                        dest.digits[i + j - 1] += dest.digits[i + j] / 10;
                        dest.digits[i + j] = dest.digits[i + j] % 10;
                }
        }
}


void bn_half(bignum &dest)
{
        // ...for leisure
        bignum source = dest;

        // clear the target string
        dest.digits = std::string(dest.length, '\0');

        // jump straight to the first non-zero element
        size_t nonzero = source.digits.find_first_not_of('\0');

        // remainder of each step
        size_t remains = 0;

        // do long division
        for (size_t i = nonzero; i < source.length; i++) {
                size_t dividend = remains * 10 + source.digits[i];
                dest.digits[i] = dividend / 2;
                remains = dividend % 2;
        }

        // process remains
        while (remains > 0) {
                remains *= 10;
                dest.digits.insert(dest.digits.end(), 1, remains / 2);
                remains = remains % 2;
                dest.length++;
        }
}


void bn_inverse(bignum &dest)
{
        // ...for leisure
        bignum source = dest;

        // elevate bignum to negative exponent
        auto bn_elevate_neg = [](bignum &dest, size_t n) {

                dest.digits.insert(0, n, '\0');
                dest.length += n;
        };

        // absolute value of bignum
        auto bn_abs = [](bignum &dest) {

                dest.signal = true;
        };

        // a = 1/b; if a*b == 1, then I found the inverse of b
        bignum one;
        bn_pc_to_bignum(one, 1);

        // lower boundary for the binary search
        bignum lower;
        bn_pc_to_bignum(lower, 0);

        // upper boundary for the binary search
        bignum upper;
        bn_pc_to_bignum(upper, 1);

        /*
         * how to set an appropriate value for the error margin?
         *
         * The current setting is absolute overkill. Perhaps this should be set
         * manually, according to the precision desired from the PI number.
         */
        bignum error;
        bn_pc_to_bignum(error, 1);
        bn_elevate_neg(error, source.length * 2 + 1);

        // if tester * source = 1 or tester - one < error, stop checking
        bignum tester;

        do {
                // adjust search ranges
                bn_pad(lower, upper);
                bn_add(dest, lower, upper);
                bn_half(dest);

                // stop condition: value (get tester value)
                bn_trim(dest);
                bn_pad(dest, source);
                bn_mult(tester, dest, source);

                // stop condition: value (actually do test)
                bn_trim(tester);
                bn_pad(tester, one);
                int comp = bn_abscompare(tester, one);

                if (comp == 0) {        // value found
                        break;
                } else if (comp > 0) {  // adjust range
                        upper = dest;
                } else if (comp < 0) {  // adjust range
                        lower = dest;
                }

                // stop condition: error
                bn_sub(tester, tester, one);
                bn_pad(tester, error);

        } while (bn_abscompare(tester, error) > 0);
}


/*
 * align (pad with zeroes) bignums a and b in length and decimal point
 */
void bn_pad(bignum &a, bignum &b)
{
        // INTEGRAL PART
        size_t intpart_size = std::max(a.dpoint, b.dpoint);
        size_t a_fwpad = intpart_size - a.dpoint;
        size_t b_fwpad = intpart_size - b.dpoint;

        a.digits.insert(0, a_fwpad, 0);
        b.digits.insert(0, b_fwpad, 0);

        // FRACTIONAL PART
        size_t decpart_size = std::max(a.length - a.dpoint,
                        b.length - b.dpoint);
        size_t a_backpad = decpart_size - (a.length - a.dpoint);
        size_t b_backpad = decpart_size - (b.length - b.dpoint);

        a.digits.insert(a.digits.end(), a_backpad, 0);
        b.digits.insert(b.digits.end(), b_backpad, 0);

        // adjust lengths...
        a.length += a_fwpad + a_backpad;
        b.length += b_fwpad + b_backpad;

        // ...and point positions
        a.dpoint += a_fwpad;
        b.dpoint += b_fwpad;
}


/*
 * trim leading and trailing zeroes from a bignum
 */
void bn_trim(bignum &number)
{
        /*
         * trim left
         *
         * If the search fails, the return is std::npos = -1 = max_size_t,
         * which the decimal point is guaranteed to be smaller than (hopefully).
         */
        size_t ltrim = std::min(number.digits.find_first_not_of('\0'), number.dpoint);
        number.digits.erase(number.digits.begin(), number.digits.begin() + ltrim);

        // next step depends on dpoint, so...
        number.dpoint -= ltrim;

        // ENDING IS EXCLUSIVE, BEGINING IS INCLUSIVE
        // ENDING IS EXCLUSIVE, BEGINING IS INCLUSIVE

        /*
         * trim right
         *
         * If the search fails, the return is std::npos = -1 = max_size_t;
         * max_size_t + 1 = 0, which in turn will be smaller than the dpoint.
         */
        size_t rtrim = std::max(number.digits.find_last_not_of('\0') + 1, number.dpoint);
        number.digits.erase(number.digits.begin() + rtrim, number.digits.end());

        // ltrim was already removed, so...
        number.length = rtrim;
}


int bn_abscompare(const bignum &a, const bignum &b)
{
        int tendency = 0;

        for (size_t i = 0; i < a.length; i++) {
                /*
                 * Numbers are aligned in length and decimal point, so the first
                 * difference between digits already tells me which is greater.
                 */
                if (a.digits[i] > b.digits[i])
                        return  1;
                if (a.digits[i] < b.digits[i])
                        return -1;
        }

        return tendency;
}


void bn_pc_to_bignum(bignum &dest, size_t pc) {

        size_t len_minus_one = (pc == 0 ? 0 : size_t(log10(pc)));

        dest.signal = true;
        dest.digits = std::string(len_minus_one + 1, '\0');
        dest.length = dest.digits.size();
        dest.dpoint = dest.digits.size();

        for (size_t i = dest.digits.size(); i > 0; i--) {
                dest.digits[i - 1] = pc % 10;
                pc = pc / 10;
        }
};


void bn_print(bignum &number)
{
        // signal
        if (!number.signal)
                std::cout << "-";

        // integral part
        for (size_t i = 0; i < number.dpoint; i++)
                std::cout << char(number.digits[i] + 48);

        // decimal point
        std::cout << '.';

        // decimal part
        for (size_t i = number.dpoint; i < number.length; i++)
                std::cout << char(number.digits[i] + 48);

        std::cout << std::endl;
}
