// -----------------------------------------------------------------------------
//
// GAMCS -- Generalized Intelligence Model and Computer Simulation
//
// Copyright (C) 2013-2014, Andy Huang  <andyspider@126.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// -----------------------------------------------------------------------------
//
// Created on: Oct 19, 2013
//
// -----------------------------------------------------------------------------

#ifndef GIOM_H
#define GIOM_H
#include <stddef.h>     // NULL
#define __STDC_LIMIT_MACROS     // UINT64_MAX
#include <stdint.h>
#define __STDC_FORMAT_MACROS    // PRIu64
#include <inttypes.h>
#include "gamcs/config.h"
#include "gamcs/debug.h"

namespace gamcs
{

#if INT_BITS == 8
typedef uint8_t gamcs_uint;
typedef int8_t gamcs_int;
#define GAMCS_UINT_FMT PRIu8
#define GAMCS_INT_FMT PRId8
#define GAMCS_UINT_MAX UINT8_MAX
#define GAMCS_INT_MAX INT8_MAX

#elif INT_BITS == 16
typedef uint16_t gamcs_uint;
typedef int16_t gamcs_int;
#define GAMCS_UINT_FMT PRIu16
#define GAMCS_INT_FMT PRId16
#define GAMCS_UINT_MAX UINT16_MAX
#define GAMCS_INT_MAX INT16_MAX

#elif INT_BITS == 32
typedef uint32_t gamcs_uint;
typedef int32_t gamcs_int;
#define GAMCS_UINT_FMT PRIu32
#define GAMCS_INT_FMT PRId32
#define GAMCS_UINT_MAX UINT32_MAX
#define GAMCS_INT_MAX INT32_MAX

#else       // 64bit by default
typedef uint64_t gamcs_uint;
typedef int64_t gamcs_int;
#define GAMCS_UINT_FMT PRIu64
#define GAMCS_INT_FMT PRId64
#define GAMCS_UINT_MAX UINT64_MAX
#define GAMCS_INT_MAX INT64_MAX

#endif

#define IN_FMT GAMCS_INT_FMT       // usage: printf("%" IN_FMT "\n", intput);
#define OUT_FMT GAMCS_INT_FMT       // usage: printf("%" OUT_FMT "\n", output);

class OSpace;

/**
 * Generalized Input Output Model
 */
class GIOM
{
    public:
        typedef gamcs_int Input; /**< input type, signed integer */
        typedef gamcs_int Output; /**< output type, signed integer */

        /** Default constructor */
        GIOM();
        /** Default destructor */
        virtual ~GIOM();
        Output process(Input input, OSpace &available_outputs); /**< choose an output value in an output space under a specified input value */
        float singleOutputEntropy(Input input, OSpace &available_outputs) const; /**< calculate entropy of a specified input */
        virtual void update(); /**< update inner states of GIOM, derived classes may have their own inner states to update */

        static const Input INVALID_INPUT = GAMCS_INT_MAX; /**< the maximun value is used to indicate invalid input */
        static const Output INVALID_OUTPUT = GAMCS_INT_MAX; /**< the maximun value is used to indicate an invalid output */

    protected:
        virtual OSpace constrain(Input input, OSpace &available_outputs) const; /**< constrain the output space to a subspace */
        Input cur_in; /**< input value */
        Output cur_out; /**< output value corresponding to cur_in */
        unsigned long process_count; /**< count of processing */

    private:
        gamcs_uint randomGenerator(gamcs_uint size) const; /**< generate a random number in range 0 to LONG_MAX. It's where all possibilities and miracles come from! */
};

/**
 * Fragment to store a single output or a range of outputs.
 * For a singe output, the end is equal to start.
 */
struct OFragment
{
        GIOM::Output start; /**< the starting output value */
        GIOM::Output end; /**< the final output value */
        GIOM::Output step; /**< the increasing or decreasing step */
};

/**
 *  Output space.
 */
class OSpace
{
    public:
        enum
        {
            SPARE_CAPACITY = 16
        };

        typedef gamcs_uint olsize_t;    // actions have the maximun number of 2^64

        explicit OSpace(olsize_t initfn = 0) :
                frag_num(initfn), the_capacity(initfn + SPARE_CAPACITY), current_index(
                        0), outputs(NULL)
        {
            outputs = new OFragment[the_capacity];
        }

        OSpace(const OSpace &other) :
                frag_num(0), the_capacity(SPARE_CAPACITY), current_index(0), outputs(
                        NULL)
        {
            operator=(other);
        }

        ~OSpace()
        {
            delete[] outputs;
        }

        /**
         * Check if the space is empty
         * @return true or false
         */
        bool empty() const
        {
            return (size() == 0);
        }

        /**
         * Get the total number of outputs in space
         * @return  the number
         */
        olsize_t size() const
        {
            olsize_t total_size = 0;
            // traverse each frament and collect the total size
            for (olsize_t i = 0; i < frag_num; i++)
            {
                OFragment *frag = outputs + i;
                total_size += (frag->end - frag->start) / frag->step + 1;
            }

            return total_size;
        }

        /**
         * Current capacity of the space
         * @return  capacity
         */
        olsize_t capacity() const
        {
            return the_capacity;
        }

        /**
         * Override operator [].
         * For index out of bound returns INVALID_OUTPUT
         * @param index output index
         * @return output value
         */
        GIOM::Output operator[](olsize_t index) const
        {
            olsize_t total_num = 0;
            olsize_t out_wanted;

            if (index < total_num)    // subscript out of bound
            {
                return GIOM::INVALID_OUTPUT;
            }

            olsize_t i;
            // traverse each fragment to find which fragment the index locates in
            for (i = 0; i < frag_num; i++)
            {
                OFragment *ptr = outputs + i;
                olsize_t out_num_in_this_frag = (ptr->end - ptr->start)
                        / ptr->step + 1;
                if (total_num + out_num_in_this_frag > index)    // index is in this fragment
                {
                    olsize_t index_in_frag = index - total_num;
                    out_wanted = ptr->start + ptr->step * index_in_frag;
                    break;
                }

                total_num += out_num_in_this_frag;
            }

            if (i >= frag_num)    // superscript out of bound
            {
                return GIOM::INVALID_OUTPUT;
            }

            return out_wanted;
        }

        /**
         * Override operator =
         * @param other another OutList object
         * @return  reassigned object
         */
        const OSpace &operator=(const OSpace &other)
        {
            if (this != &other)
            {
                delete[] outputs;
                // copy data
                frag_num = other.frag_num;
                the_capacity = other.the_capacity;

                outputs = new OFragment[the_capacity];
                // copy each fragment
                for (olsize_t i = 0; i < frag_num; i++)
                {
                    outputs[i] = other.outputs[i];
                }
            }

            return *this;
        }

        /**
         * add a single output to space
         * @param output output
         */
        void add(GIOM::Output output)
        {
            // check if exceeds the capacity
            if (frag_num == the_capacity) expand(2 * the_capacity + 1);

            OFragment new_frag;
            new_frag.start = output;
            new_frag.end = output;
            new_frag.step = 1;
            outputs[frag_num++] = new_frag;    // copy fragment and increase num
        }

        /**
         * add a output range to space
         * @param start start output
         * @param end   end output
         * @param step increasing or decreasing step
         */
        void add(GIOM::Output start, GIOM::Output end, GIOM::Output step)
        {
            // check range
            if ((end - start) / step < 0)
                ERROR(
                        "Invalid range! %" OUT_FMT " --> %" OUT_FMT " (step: %" OUT_FMT ") \n",
                        start, end, step);

            if (frag_num == the_capacity) expand(2 * the_capacity);

            OFragment new_frag;
            new_frag.start = start;
            new_frag.end = end;
            new_frag.step = step;
            outputs[frag_num++] = new_frag;
        }

        /**
         * Expend space capacity.
         * @param ncap new capacity
         */
        void expand(olsize_t ncap)
        {
            if (ncap < frag_num) return;    // the new capacity should at least include all current fragments

            OFragment *old_list = outputs;
            // add up the spare
            ncap += SPARE_CAPACITY;
            outputs = new OFragment[ncap];
            // copy all fragments to new place
            for (olsize_t i = 0; i < frag_num; i++)
            {
                outputs[i] = old_list[i];
            }

            the_capacity = ncap;
            delete[] old_list;
        }

        /**
         * clear the space.
         */
        void clear()
        {
            frag_num = 0;
        }

        /**
         * Get the first output in space.
         * @return the first output
         */
        GIOM::Output first() const
        {
            current_index = 0;
            return operator[](current_index);
        }

        /**
         * Get the last output in space.
         * @return the last output
         */
        GIOM::Output last() const
        {
            return operator[](size());
        }

        /**
         * Iterator the space and get the next output.
         * @return the next output
         */
        GIOM::Output next() const
        {
            current_index++;
            return operator[](current_index);
        }

    private:
        olsize_t frag_num;    // number of fragments
        olsize_t the_capacity;    // space capacity
        mutable olsize_t current_index;    // used by iterator
        OFragment *outputs;
};

}    // namespace gamcs
#endif // GIOM_H
