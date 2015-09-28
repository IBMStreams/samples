/**
 * Copyright (C) 2015, International Business Machines Corporation.
 * All Rights Reserved.
 */

#ifndef STREAMSX_SAMPLE_SGFILTER_COEFFICIENTS_H
#define STREAMSX_SAMPLE_SGFILTER_COEFFICIENTS_H

#include <cassert>
#include <cstddef>

namespace streamsx { namespace sample { namespace sgfilter {

/**
 * Encapsulates initialization and access to the coefficients of a
 * Savitzky-Golay filter.
 */
class Coefficients
{
public:
    Coefficients(size_t n) {
        float* p = filter5;
        *p++ = -3.F/35; *p++ = 12.F/35; *p++ = 17.F/35; *p++ = 12.F/35; *p++ = -3.F/35;
        p = filter7;
        *p++ = -2.F/14; *p++ = 3.F/14; *p++ = 6.F/14; *p++ = 7.F/14; *p++ = 6.F/14; *p++ = 4.F/14; *p++ = -2.F/14;
        p = filter9;
        *p++ = -21.F/231; *p++ = 14.F/231; *p++ = 39.F/231; *p++ = 54.F/231;
        *p++ = 59.F/231; *p++ = 54.F/231; *p++ = 39.F/231; *p++ = 14.F/231; *p++ = -21.F/231;
        p = filter11;
        *p++ = -36.F/429; *p++ = 9.F/429; *p++ = 44.F/429; *p++ = 69.F/429; *p++ = 84.F/429; *p++ = 89.F/429;
        *p++ = 84.F/429; *p++ = 69.F/429; *p++ = 44.F/429; *p++ = 9.F/429; *p++ = -36.F/429;

        switch (n) {
        case 5:
            coefficients_ = filter5;
            break;
        case 7:
            coefficients_ = filter7;
            break;
        case 9:
            coefficients_ = filter9;
            break;
        case 11:
            coefficients_ = filter11;
            break;
        default:
            assert("Invalid filter length");
        }
    }

   /**
    * @brief  Subscript access to coefficients data.
    * @param n Coefficient index.
    * @return Read-only reference to a coefficient.
    */
   float& operator[](size_t n)
   { return *(this->coefficients_ + n); }

   /**
    * @brief  Subscript access to coefficients data.
    * @param n Coefficient index.
    * @return Read-only reference to a coefficient.
    */
   const float& operator[](size_t n) const
   { return *(this->coefficients_ + n); }

private:
    float* coefficients_;
    float filter5[5]; // {-3.F/35, 12.F/35, 17.F/35, 12.F/35, -3.F/35};
    float filter7[7]; // {-2.F/14, 3.F/14, 6.F/14, 7.F/14, 6.F/14, 4.F/14, -2.F/14};
    float filter9[9]; // {-21.F/231, 14.F/231, 39.F/231, 54.F/231, 59.F/231, 54.F/231, 39.F/231, 14.F/231, -21.F/231};
    float filter11[11]; // {-36.F/429, 9.F/429, 44.F/429, 69.F/429, 84.F/429, 89.F/429, 84.F/429, 69.F/429, 44.F/429, 9.F/429, -36.F/429};
};
}}}
#endif // STREAMSX_SAMPLE_SGFILTER_COEFFICIENTS_H
