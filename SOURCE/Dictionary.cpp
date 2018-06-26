/*
 * Dictionary.cpp
 *
 *  Created on: 2012/11/23
 *      Author: Hiroki Sudo
 */

#include "Dictionary.h"

std::map<int, std::string> Dictionary::individual;
std::map<int, std::string> Dictionary::symbol;
std::map<std::string, int> Dictionary::conv_individual;
std::map<std::string, int> Dictionary::conv_symbol;

void Dictionary::load(std::string &file_path)
{
  std::string line;
  std::ifstream source(file_path.c_str());

  //read test
  if (!source.good())
  {
    std::cerr << "not found dictionary file" << std::endl;
    exit(1);
  }

  //read items
  std::vector<std::string> individual_buffer;
  std::vector<std::string> symbol_buffer;
  while (std::getline(source, line))
  {
    const std::regex re("[,=]");
    std::sregex_token_iterator it(std::begin(line), std::end(line), re, -1), it_end;
    if (*it == "IND")
    {
      it++;
      std::copy(it, it_end, std::back_inserter(individual_buffer));
    }
    else if (*it == "SYM")
    {
      it++;
      std::copy(it, it_end, std::back_inserter(symbol_buffer));
    }
    else
    {
      std::cerr << "undefined key\"" << (*it) << "\"" << std::endl;
      exit(1);
    }
  }
  if (symbol_buffer.size() == 0 || individual_buffer.size() == 0)
  {
    std::cerr << "no dictionary data" << std::endl;
    exit(1);
  }

  //store items
  int index;
  std::vector<std::string>::iterator it;
  it = individual_buffer.begin();
  index = 0;
  while (it != individual_buffer.end())
  {
    if (individual.find(index) == individual.end() && conv_individual.find(*it) == conv_individual.end())
    {
      individual.insert(std::map<int, std::string>::value_type(index, *it));
      conv_individual.insert(
          std::map<std::string, int>::value_type(*it, index));
    }
    if (index + 1 <= index)
    {
      std::cerr << "range over" << std::endl;
      exit(1);
    }
    index++;
    it++;
  }

  it = symbol_buffer.begin();
  index = 0;
  while (it != symbol_buffer.end())
  {
    if (symbol.find(index) == symbol.end() && conv_symbol.find(*it) == conv_symbol.end())
    {
      symbol.insert(std::map<int, std::string>::value_type(index, *it));
      conv_symbol.insert(std::map<std::string, int>::value_type(*it, index));
    }
    if (index + 1 <= index)
    {
      std::cerr << "range over" << std::endl;
      exit(1);
    }

    index++;
    it++;
  }
}

Dictionary
Dictionary::copy(void)
{
  return Dictionary();
}
