/*
 * MT19937E.cpp
 *
 *  Created on: 2012/12/04
 *      Author: Hiroki Sudo
 */

#include "MT19937.h"

MT19937::MT19937() {
	// TODO Auto-generated constructor stub

}

MT19937::~MT19937() {
	// TODO Auto-generated destructor stub
}

unsigned long long int MT19937::icount=0LL;
unsigned long long int MT19937::rcount=0LL;


std::mt19937 MT19937::igen(111);
std::uniform_int_distribution<> MT19937::idist(0, INT_MAX);
// boost::variate_generator<boost::mt19937, boost::uniform_int<int> > MT19937::_irand(MT19937::igen, MT19937::idist);

std::mt19937 MT19937::rgen(111);
std::uniform_real_distribution<> MT19937::rdist(0, 1);
// boost::variate_generator<boost::mt19937, boost::uniform_real<double> > MT19937::_rrand(MT19937::rgen, MT19937::rdist);


int MT19937::irand(void){
	if(icount < ULLONG_MAX -1)
		icount++;
	else
		throw "cannot use random any more";

	return idist(igen);
}

int MT19937::irand(int a, int b){
	if(icount < ULLONG_MAX -1)
		icount++;
	else
		throw "cannot use random any more";
	std::uniform_int_distribution<> dist(a,b);
	return dist(igen);
}

double MT19937::rrand(void){
	if(rcount < ULLONG_MAX -1)
		rcount++;
	else
		throw "cannot use random any more";
	return rdist(rgen);
}

double MT19937::rrand(double a, double b){
	if(rcount < ULLONG_MAX -1)
		rcount++;
	else
		throw "cannot use random any more";
	std::uniform_real_distribution<> dist(a,b);
	return dist(rgen);
}

void MT19937::waste(void){
	igen.discard(icount);
	rgen.discard(rcount);
}

void MT19937::set_seed(uint32_t seed_value){
	igen.seed(static_cast<unsigned long>(seed_value));
	rgen.engine().seed(static_cast<unsigned long>(seed_value));
}
