 /*
 * File:   MSILMParameters.h
 * Author: hiroki
 *
 * Created on October 26, 2013, 5:28 PM
 */

#ifndef MSILMPARAMETERS_H_
#define MSILMPARAMETERS_H_

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <boost/program_options.hpp>

#include "Parameters.h"
#include "LogBox.h"



/*!
 * 実行時引数を解釈して、保持するクラス
 */
class MSILMParameters{
public:
	//type definition
    enum FORMAT {
      BIN, XML
    };

    //experiment parameters
    int MAX_GENERATIONS;
    double PER_UTTERANCES; //
    boost::uint32_t RANDOM_SEED; //
    bool UNIQUE_UTTERANCE; //

    uint32_t CONTROLS;
    int buzz_length;

    //Execution Values
    int UTTERANCES;
    uint32_t Generation_Counter; //

    //system parameters
    bool LOGGING;
    bool PROGRESS;
    bool RESUME;
    bool SAVE_LAST_STATE;
    bool SAVE_ALL_STATE;
    bool ANALYZE;
	bool OMISSION;
    FORMAT SAVE_FORMAT;

    std::string DICTIONARY_FILE;

    //file parameters
    //file prefix
    std::string FILE_PREFIX;
    std::string DATE_STR;

    //file extentions
    std::string STATE_EXT;
    std::string RESULT_EXT;
    std::string LOG_EXT;

    //path
    std::string BASE_PATH;

    //file
    std::string LOG_FILE;
    std::string RESUME_FILE;
    std::string SAVE_FILE;
    std::string RESULT_FILE;

    boost::program_options::variables_map svm;
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
private:
	std::string string_join(const std::vector<std::string> & str_v, const std::string & delim);
};

#endif /* MSILMPARAMETERS_H_ */
