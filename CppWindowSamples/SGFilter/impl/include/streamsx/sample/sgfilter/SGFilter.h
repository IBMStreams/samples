/**
 * Copyright (C) 2015, International Business Machines Corporation.
 * All Rights Reserved.
 */

#ifndef STREAMSX_SAMPLE_SGFILTER_SGFILTER_H
#define STREAMSX_SAMPLE_SGFILTER_SGFILTER_H

#include "Coefficients.h"

#include <SPL/Runtime/Operator/Operator.h>
#include <SPL/Runtime/Operator/OperatorContext.h>
#include <SPL/Runtime/Window/Window.h>

#include <sstream>
#include <stdexcept>

namespace streamsx { namespace sample { namespace sgfilter {

/**
 * Generic implementation for a tuple transformation implementing the
 * Savitzky-Golay algorithm for data filtering.
 *
 * Template parameters:
 *  - OperatorType the actual SPL operator wrapping this instance
 *  - TupleInType type of tuples received by the operator on input port 0
 *  - TupleOutType type of tuples submitted on the operator output port 0
 */
template <typename OperatorType, typename TupleInType, typename TupleOutType>
class SGFilter : public ::SPL::WindowEvent<TupleInType>
{
public:
    // Type of window used by this filter
    typedef typename ::SPL::SlidingWindow<TupleInType> SlidingWindowType;

    // Types related to the WindowEvent we inherit from
    typedef typename ::SPL::WindowEvent<TupleInType> WindowEventType;
    typedef typename WindowEventType::WindowType WindowType;
    typedef typename WindowEventType::PartitionType PartitionType;

    /**
     * @brief Creates a @c SGFilter with the specified length.
     *
     * @param op Operator embedding this filter instance
     * @param filterLength length of this filter
     * @param traceCategories trace source categories associated with the
     *      trace statements emitted by this object (comma-delimited list
     *      of strings)
     */
    SGFilter(OperatorType& op, size_t filterLength, const char* traceCategories) :
        op_(op),
        coefficients_(filterLength),
        traceCats_(traceCategories),
        window_(op,
                // Window associated with port 0 of operator 'op'
                0,
                // Window size count eviction policy set to coincide with
                // the number of filter coefficients
                ::SPL::CountWindowPolicy(filterLength),
                // Trigger on every inserted tuple, after window is full
                ::SPL::CountWindowPolicy(1)
                ),
        windowFull_(false)
    {
        window_.registerOnWindowTriggerHandler(this);
        window_.registerOnWindowInitialFullHandler(this);
        SPLAPPTRC(L_INFO, "Constructed SGFilter " << (void*)this <<
                " with window with eviction policy CountWindowPolicy(" <<
                filterLength << ") and trigger policy CountWindowPolicy(1)", traceCats_);
    }

    virtual ~SGFilter() {}

    /**
     * @brief Process input tuple.
     *
     * Simply insert tuple into the window and let the WindowEvent handler
     * do the work.
     */
    void process(TupleInType const & tuple, uint32_t port)
    {
        SPLAPPTRC(L_TRACE, "process(" << tuple << ", " << port << ")", traceCats_);
        TupleInType const & t = static_cast<TupleInType const&>(tuple);
        window_.insert(t);
        SPLAPPTRC(L_TRACE, "Inserted tuple: " << tuple << " window is: " << window_.toString(), traceCats_);
    }

    /**
     * @brief Invoked when the window becomes full for the first time.
     * @param window the window of tuples
     * @param partition indicates which window partition is full
     */
    void onWindowInitialFullEvent(WindowType & window, PartitionType const & partition)
    {
        windowFull_ = true;
        SPLAPPTRC(L_DEBUG, "Received onWindowInitialFullEvent", traceCats_);
    }

    /**
     * @brief Invoked when the window triggers.
     * @param window the window of tuples
     * @param partition indicates which window partition is full
     *
     * Calculates the filtered value and submits it on the output port of the
     * associated operator.
     */
    void onWindowTriggerEvent(WindowType & window, PartitionType const & partition)
    {
        SPLAPPTRC(L_DEBUG, "Received onWindowTriggerEvent", traceCats_);

        // Don't output tuples from an incomplete window
        if (!windowFull_)
            return;

        // Submit a WindowMarker after every window flush
        typename WindowType::DataType & data = window.getWindowData(partition);
        if (data.size() == 0) {
            op_.submit(::SPL::Punctuation::WindowMarker, 0);
            SPLAPPTRC(L_DEBUG, "Data size is 0, window marker submitted", traceCats_);
            return;
        }

        // Calculate output value
        typename WindowType::DataType::const_iterator it;
        size_t i = 0;
        float result = 0.F;
        for (it = data.begin(); it != data.end(); it++, i++) {
            TupleInType const & tuple = *it;
            double val = (double)tuple.getAttributeValue(0);
            result += val * coefficients_[i];
        }

        // Submit the result
        TupleOutType otuple(result);
        op_.submit(otuple, 0);
        op_.submit(::SPL::Punctuation::WindowMarker, 0);
        SPLAPPTRC(L_DEBUG, "Submitted tuple: " << otuple << " and window marker", traceCats_);
    }

    /**
     * @brief Validate the specified filterLength value.
     *
     * This function throws an exception if value is not an odd
     * integer between 5 and 11.
     */
    static int validateFilterLength(int value)
    {
        if ((value < 5 || value > 11) || (value % 2 == 0)) {
            std::stringstream ss;
            ss << "FilterLength value " << value << " even or outside [5..11]";
            std::domain_error ex(ss.str());
            throw ex;
        }
        return value;
    }

private:
    OperatorType& op_;              // Reference to the wrapping operator instance
    Coefficients coefficients_;     // Filter coefficients
    const std::string traceCats_;   // Operator trace categories
    SlidingWindowType window_;      // Window which holds incoming tuples
    bool windowFull_;               // Set to true when the window fills for the first time
};
}}}
#endif // STREAMSX_SAMPLE_SGFILTER_SGFILTER_H
