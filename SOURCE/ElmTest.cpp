#include "Element.h"
#include <vector>
#include <iostream>
int main(int arg, char **argv)
{
	Element::dictionary.load((char *)"data.dic");

	std::vector<Element> elements, elements2;
	int type = 0;

	for (int type =0; type < 4; type++)
	{
		Element elm;
		switch (type)
		{
		case ELEM_TYPE::CAT_TYPE:
			for (unsigned int i = 0; i < 3; i++)
			{
				elm.set_cat(i, i);
				elements.push_back(elm);
			}
			break;
		case ELEM_TYPE::MEAN_TYPE:
			for (unsigned int i = 0; i < Element::dictionary.individual.size(); i++)
			{
				elm.set_ind(i);
				elements.push_back(elm);
			}
			break;
		case ELEM_TYPE::SYM_TYPE:
			for (unsigned int i = 0; i < Element::dictionary.symbol.size(); i++)
			{
				elm.set_sym(i);
				elements.push_back(elm);
			}
			break;
		case ELEM_TYPE::VAR_TYPE:
			for (unsigned int i = 0; i < 3; i++)
			{
				elm.set_var(i, 0);
				elements.push_back(elm);
			}
			break;
		default:
			std::cerr << "unknown type of Element " << type << std::endl;
			exit(1);
		}
	}

	// std::vector<Element>::iterator it;
	// it = elements.begin();
	// int l = 0;
	// while (it != elements.end())
	// {
	// 	std::cout << l++ << ": " << (*it).to_s() << std::endl;
	// 	it++;
	// }

	std::vector<Element>::iterator it1, it2;
	it1 = elements.begin();
	int k = 0;
	int j = 0;
	for (int j = 0; j < elements.size(); j++)
	{
		while (int k = 0; k < elements.size(); k++)
		{
			if (elements[j] == elements[k])
			{
				if(j != k){
					exit(1);
				}
			}else{
				if(j == k){
					exit(1);
				}
			}
		}
	}

	elements2 = elements;
	for (int i = 0; i < elements.size(); i++)
	{
		if(elements[i] != elements2[i]){
			exit(1);
		}
	}
	return 0;
}