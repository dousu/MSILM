/*
 * IndexFactory.cpp
 *
 *  Created on: 2012/11/20
 *      Author: Hiroki Sudo
 */

#include "IndexFactory.h"

IndexFactory &IndexFactory::operator=(const IndexFactory &dst)
{
	index_counter = dst.index_counter;
	return *this;
}
int IndexFactory::generate(void)
{
	if (index_counter + 1 < index_counter){
		std::cerr << "range over error" << std::endl;
		exit(1);
	}
	index_counter++;
	return index_counter;
}

IndexFactory::IndexFactory(IndexFactory &other)
{
	index_counter = other.index_counter;
}