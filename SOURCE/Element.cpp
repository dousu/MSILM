/*
 * Element.cpp
 *
 *  Created on: 2012/11/19
 *      Author: Hiroki Sudo
 */

#include "Element.h"

//static initialize
Dictionary Element::dictionary = Dictionary::copy();

Element::Element()
{
}

/*
 * Operators
 */
bool Element::operator!=(const Element &dst) const
{
	return !(*this == dst);
}

bool Element::operator==(const Element &dst) const
{
	//タイプ検査
	if (type != dst.type)
		return false;

	//同タイプでの比較
	switch (type)
	{
	case ELEM_TYPE::CAT_TYPE:
		if (obj == dst.obj && cat == dst.cat)
			return true;
		break;
	case ELEM_TYPE::VAR_TYPE:
		if (obj == dst.obj)
			return true;
		break;
	case ELEM_TYPE::MEAN_TYPE:
		if (obj == dst.obj)
			return true;
		break;
	case ELEM_TYPE::SYM_TYPE:
		if (obj == dst.obj)
			return true;
		break;
	default:
		std::cout << type << ":"
				  << "unknown type" << std::endl;
		throw "unknown type";
		break;
	}

	return false;
}

bool Element::operator<(const Element &dst) const
{
	if (type < dst.type)
		return true;
	else if (type > dst.type)
		return false;

	//同タイプでの比較
	switch (type)
	{
	case ELEM_TYPE::CAT_TYPE:
		if (cat < dst.cat)
			return true;
		else if (cat > dst.cat)
			return false;

		if (obj < dst.obj)
			return true;
		else if (obj > dst.obj)
			return false;

		break;
	case ELEM_TYPE::VAR_TYPE:
		if (obj < dst.obj)
			return true;
		else if (obj > dst.obj)
			return false;

		break;
	case ELEM_TYPE::MEAN_TYPE:
		if (obj < dst.obj)
			return true;
		else if (obj > dst.obj)
			return false;

		break;
	case ELEM_TYPE::SYM_TYPE:
		if (obj < dst.obj)
			return true;
		else if (obj > dst.obj)
			return false;

		break;
	default:
		std::cout << type << ":"
				  << "unknown type" << std::endl;
		throw "unknown type";
		break;
	}

	return false;
}

Element &Element::operator=(const Element &dst)
{
	type = dst.type;
	obj = dst.obj;
	cat = dst.cat;

	return *this;
}

std::string Element::to_s(void)
{
	switch (type)
	{

	case ELEM_TYPE::CAT_TYPE:
		return Prefices::CAT + Prefices::CLN +
			   std::to_string(cat) +
			   Prefices::DEL +
			   Prefices::VAR + Prefices::CLN +
			   std::to_string(obj);
		break;

	case ELEM_TYPE::MEAN_TYPE:
		if (dictionary.individual.find(obj) == dictionary.individual.end())
		{
			return "-";
		}
		return dictionary.individual[obj];
		break;

	case ELEM_TYPE::SYM_TYPE:
		if (dictionary.symbol.find(obj) == dictionary.symbol.end())
		{
			return "*";
		}
		return dictionary.symbol[obj];
		break;

	case ELEM_TYPE::VAR_TYPE:
		return Prefices::CAT + Prefices::CLN +
			   std::to_string(cat) +
			   Prefices::DEL +
			   Prefices::VAR + Prefices::CLN +
			   std::to_string(obj);
		break;

	default:
		std::cerr << "unknown type: Element::to_s()" << std::endl;
		throw "unknown type of Element";
	}
}

Element &Element::set_cat(int var, int cat)
{
	return set(ELEM_TYPE::CAT_TYPE, var, cat);
}

Element &Element::set_var(int var, int cat)
{
	return set(ELEM_TYPE::VAR_TYPE, var, cat);
}

Element &Element::set_ind(int id)
{
	if (dictionary.individual.find(id) == dictionary.individual.end())
		throw "range over for individual";
	return set(ELEM_TYPE::MEAN_TYPE, id, 0);
}

Element &Element::set_sym(int id)
{
	if (dictionary.symbol.find(id) == dictionary.symbol.end())
		throw "range over for symbol";
	return set(ELEM_TYPE::SYM_TYPE, id, 0);
}

Element &Element::set(int dtype, int dobj, int dcat)
{
	type = dtype;
	obj = dobj;
	cat = dcat;
	return *this;
}

bool Element::is_var(void) const
{
	if (type == ELEM_TYPE::VAR_TYPE)
		return true;
	else
		return false;
}
bool Element::is_cat(void) const
{
	if (type == ELEM_TYPE::CAT_TYPE)
		return true;
	else
		return false;
}
bool Element::is_ind(void) const
{
	if (type == ELEM_TYPE::MEAN_TYPE)
		return true;
	else
		return false;
}
bool Element::is_sym(void) const
{
	if (type == ELEM_TYPE::SYM_TYPE)
		return true;
	else
		return false;
}
