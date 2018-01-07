 /*
 * File:   MSILMParameters.h
 * Author: hiroki
 *
 * Created on October 26, 2013, 5:28 PM
 */

#ifndef MSILMPARAMETERS_H_
#define MSILMARAMETERS_H_

#include <vector>
#include <string>
#include <fstream>
#include <boost/program_options.hpp>

#include "Parameters.h"
#include "LogBox.h"



/*!
 * 実行時引数を解釈して、保持するクラス
 */
class MSILMParameters: public Parameters {
public:
	bool INTER_ANALYSIS;
	int SPACE_ANALYSIS;

	bool INTER_LOG;
	int SPACE_LOG;
        
    int MULTIPLE_MEANINGS;
        
    double PER_TERM;
    int TERMS;
    int WINDOW;
    bool SYMMETRY;
	bool UC_SYMMETRY;
    bool OMISSION;
    bool ACC_MEA;

	MSILMParameters();
	virtual ~MSILMParameters();

	void set_option(boost::program_options::variables_map& vm);
	std::string to_s(void);
};

#endif /* MSILMPARAMETERS_H_ */
