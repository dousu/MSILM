/*
 * Element.cpp
 *
 *  Created on: 2012/11/19
 *      Author: Hiroki Sudo
 */

#include "Element.h"

bool Variable::operator==(const Nonterminal & dst) const {
	return cat == dst.cat && obj == dst.obj;
}
bool Variable::operator!=(const Nonterminal & dst) const {
	return !(*this == dst);
}

bool Nonterminal::operator==(const Variable & dst) const {
	return cat == dst.cat && obj == dst.obj;
}
bool Nonterminal::operator!=(const Variable & dst) const {
	return !(*this == dst);
}
