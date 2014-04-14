// -----------------------------------------------------------------------------
//
// GAMCS -- Generalized Agent Model and Computer Simulation
//
// Copyright (C) 2013-2014, Andy Huang  <andyspider@126.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <cmath>
#include "gamcs/debug.h"
#include "gamcs/GIOM.h"

namespace gamcs
{

GIOM::GIOM() :
		cur_in(INVALID_INPUT), cur_out(INVALID_OUTPUT), process_count(0), rand_device(
				NULL), max_rand_value(0)
{
	rand_device = new std::random_device(); // to get true random on linux, use rand("/dev/random");
	max_rand_value = rand_device->max();    // save maximum value
}

GIOM::~GIOM()
{
	delete rand_device;
}

/** \brief Constraint capacity of GIOM.
 * Minimum constrain by default, which means NO constrain at all here.
 * \param in input value
 * \param outputs all possible outputs for current input
 * \return the output space after constraint
 *
 */
OSpace GIOM::constrain(Input in, OSpace &outputs) const
{
	UNUSED(in);
	return outputs;    // return all outputs
}

/** \brief Process function of GIOM.
 * Return a random item from the constrained outputs by default.
 * \param in input identity
 * \return the generated output
 *
 */
GIOM::Output GIOM::process(Input in, OSpace &alpos_outputs)
{
	OSpace restricited_outputs = constrain(in, alpos_outputs); // get restricted output values first
	if (restricited_outputs.empty()) // no output generated, return an invalid GIOM::Output
		return INVALID_OUTPUT;

	gamcs_uint sz = restricited_outputs.size();    // number of alpos_outputs
	gamcs_uint index = randomGenerator(sz) % (sz); // choose an output value randomly
	GIOM::Output out = restricited_outputs[index];

	// record input and output
	cur_in = in;
	cur_out = out;
	++process_count;    // increase count
	return out;
}

/** \brief Calculate the entropy of a state under constraint
 *
 * \return the entropy value
 *
 */
float GIOM::singleOutputEntropy(Input in, OSpace &alpos_outputs) const
{
	OSpace restricted_outputs = constrain(in, alpos_outputs);
	if (restricted_outputs.empty())
		return 0.0;

	gamcs_uint sz = restricted_outputs.size();
	return pi_log2((double) sz); // all the alpos_outputs have the same probability of occurrence
}

/** \brief Update inner states and prepare for the next processing.
 *
 */
void GIOM::update()
{
	// clear state and prepare for the next process
	cur_in = INVALID_INPUT;
	cur_out = INVALID_OUTPUT;
	return;
}

/** \brief Generate a random number in the range from 0 to sz.
 *
 * \return the generated random number
 *
 */
gamcs_uint GIOM::randomGenerator(gamcs_uint sz) const
{
	std::uniform_int_distribution<gamcs_uint> dist(0, sz - 1); // random number range: 0 ~ sz-1

	// check upper bound
	if (max_rand_value < sz - 1)
		WARNNING(
				"size exceeds maximun random value potentially generated by the random-number engine\n");
	return dist(*rand_device);
}

}    // namespace gamcs
