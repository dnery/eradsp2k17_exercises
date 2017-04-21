#include <bits/stdc++.h>  // dev stage only


/*
 * bignum type
 *
 * A big number is composed of a signal (positive or negative), a string
 * of characters representing the digits (0-9), the length of such string
 * (limited by size_t) and a decimal point position (as the index of '.'
 * in '<integral part>.<fractional part>', for example).
 */
struct bignum {

        bool            signal;         // positive or negative
        std::string     digits;         // actual number digits
        size_t          dpoint;         // decimal point position
        size_t          length;         // length of bignumber

        explicit bignum(bool signal = 1, std::string digits = "", size_t dpoint = 0) :
                signal{signal},
                digits{digits},
                dpoint{dpoint},
                length{digits.size()}
        {
        }
};



void bn_add(bignum &dest, const bignum &, bignum &);
void bn_sub(bignum &dest, const bignum &, bignum &);
void bn_pad(bignum &, bignum &);
void bn_trim(bignum &);
void bn_print(bignum &);  // PURE



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
        // to and fro (ignore this)
        auto increase = [](char a) { return a + 48; };
        auto decrease = [](char a) { return a - 48; };

        // convert to string... (ignore this)
        std::transform(number.digits.begin(), number.digits.end(),
                        number.digits.begin(), increase);

        // trim left
        size_t ltrim = number.digits.find_first_not_of("0");
        number.digits.erase(number.digits.begin(), number.digits.begin() + ltrim);

        // ENDING IS EXCLUSIVE, BEGINING IS INCLUSIVE
        // ENDING IS EXCLUSIVE, BEGINING IS INCLUSIVE

        // trim right
        size_t rtrim = number.digits.find_last_not_of("0") + 1;
        number.digits.erase(number.digits.begin() + rtrim, number.digits.end());

        // ...and back to numbers (ignore this)
        std::transform(number.digits.begin(), number.digits.end(),
                        number.digits.begin(), decrease);

        // adjust size, point
        number.dpoint -= ltrim;
        number.length = rtrim;  // ltrim was already removed, so...
}


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
                dest.digits.replace(1, dest.length - 1, dest.length - 1, 0);

                // readjust arithmetic
                bn_sub(dest, dest, pcomp);
                dest.signal = !dest.signal;  // ...now the signal is proper
        }
}


void bn_mult(bignum &dest, bignum &a, bignum &b)
{
        /*
         * length of x ~~ log10(x) + 1 (or, at most one-off from it)
         *
         * The length of the resulting multiplication a*b would
         * log10(a) + log10(b) + 1. Given, the previous, if I already know the
         * lenghts of a and b (which I do), computing the log10 can be skipped.
         */
        dest.signal = (a.signal == b.signal);
        dest.length = a.length + b.length + 1;
        dest.digits = std::string(dest.length, '\0');
        dest.dpoint = dest.length
                - (a.length - a.dpoint)
                - (b.length - b.dpoint);

        // now do simple simple multiply
        #pragma omp parallel for collapse(2)
        for (size_t i = b.length; i > 0; i--) {
                for (size_t j = a.length; j > 0; j--) {
                        dest.digits[i + j] += b.digits[i - 1] * a.digits[j - 1];
                        dest.digits[i + j - 1] += dest.digits[i + j] / 10;
                        dest.digits[i + j] = dest.digits[i + j] % 10;
                }
        }
}


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


int main(int argc, char *argv[])
{
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
        }

        std::cout << std::endl;
        std::cout << "=========" << std::endl;
        std::cout << "Test: Add" << std::endl;
        std::cout << "=========" << std::endl;
        {
                bignum result;
                bn_pad(number1, number2);
                bn_add(result, number1, number2);
                bn_trim(number1);
                bn_trim(number2);
                bn_trim(result);

                std::cout << "+ original" << std::endl;
                bn_print(number1);
                bn_print(number2);

                std::cout << "+ result" << std::endl;
                bn_print(result);
        }

        std::cout << std::endl;
        std::cout << "=========" << std::endl;
        std::cout << "Test: Sub" << std::endl;
        std::cout << "=========" << std::endl;
        {
                bignum result;
                bn_pad(number1, number2);
                bn_sub(result, number2, number1);
                bn_trim(number2);
                bn_trim(number1);
                bn_trim(result);

                std::cout << "+ original" << std::endl;
                bn_print(number2);
                bn_print(number1);

                std::cout << "+ result" << std::endl;
                bn_print(result);
        }

        std::cout << std::endl;
        std::cout << "==========" << std::endl;
        std::cout << "Test: Mult" << std::endl;
        std::cout << "==========" << std::endl;
        {
                bignum result;
                bn_mult(result, number1, number2);

                std::cout << "+ original" << std::endl;
                bn_print(number1);
                bn_print(number2);

                std::cout << "+ result" << std::endl;
                bn_print(result);
        }

        return EXIT_SUCCESS;
}
