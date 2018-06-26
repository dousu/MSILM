/*
 * File:   MSILM_main.h
 * Author: hiroki
 *
 * Created on October 28, 2013, 6:14 PM
 */

#ifndef MSILM_MAIN_H
#define MSILM_MAIN_H

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cfloat>

#include <stdio.h>

//#include <boost/program_options.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/algorithm/string/trim.hpp>
// #include <boost/progress.hpp>
/*#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/export.hpp>*/
//#include <boost/lexical_cast.hpp>

#include "MSILMAgent.h"
#include "Rule.h"
#include "Element.h"
#include "MT19937.h"
#include "Dictionary.h"
#include "LogBox.h"
#include "MSILMParameters.h"
#include "Distance.hpp"

//construct function
void construct_meanings(std::vector<Rule> &meanings);
void construct_individuals(std::vector<Element> &inds, Dictionary &dic);
void cognition_task_init(std::vector<int> &source, MSILMParameters &param);

//analyze function
void unit_analyze(std::vector<double> &result_vector,
				  std::vector<Rule> &meanings, MSILMAgent &agent);

double
expression(std::vector<Rule> &meanings, MSILMAgent &agent);

void calculate_language_distance(
	std::vector<double> &lev_dist_vector,
	std::vector<Rule> &meanings, MSILMAgent &agent1,
	MSILMAgent &agent2);

void analyze_and_output(MSILMParameters &param, std::vector<Rule> meaning_space,
						MSILMAgent &agent1, MSILMAgent &agent2, int index);

double
calculate_distance(std::vector<Rule> &meanings,
				   KnowledgeBase &kb1, KnowledgeBase &kb2, double &word_length);

//utility
void calculate_average_word_length(std::vector<Rule> &meanings, KnowledgeBase &kb1, double &word_length);

std::string
tr_vector_double_to_string(std::vector<double> vector);

double limit_time;

void logging_off(MSILMParameters &param);

void logging_on(MSILMParameters &param);

/*template<typename _IFS>
void resume_agent(
	_IFS&,
	MSILMParameters&,
	unsigned long long int&,
	unsigned long long int&,
	Dictionary&,
	std::vector<Rule>&,
	MSILMAgent&
);

template<typename _OFS>
void save_agent(
	_OFS&,
	MSILMParameters&,
	unsigned long long int&,
	unsigned long long int&,
	Dictionary&,
	std::vector<Rule>&,
	MSILMAgent&
);*/

#endif /* MSILM_MAIN_H */
