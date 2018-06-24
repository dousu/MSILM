/*
* File:   MSILMParameters.cpp
* Author: hiroki
*
* Created on October 26, 2013, 5:12 PM
*/

#include "MSILMParameters.h"

MSILMParameters::MSILMParameters() {
	INTER_ANALYSIS = false;
	SPACE_ANALYSIS = 0;
	INTER_LOG = false;
	SPACE_LOG = 0;
	FILE_PREFIX = "MSILM_";
	MULTIPLE_MEANINGS = 2;
	PER_TERM = 0.0;
	TERMS = 0;
	WINDOW = 1;
	SYMMETRY = false;
	UC_SYMMETRY = false;
	OMISSION = false;
	ACC_MEA = false;
	SAVE_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
	RESULT_FILE = (FILE_PREFIX + DATE_STR + RESULT_EXT);
	RESUME_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
	LOG_FILE = (FILE_PREFIX + DATE_STR + LOG_EXT);
	DICTIONARY_FILE = "./data.dic";
	PER_UTTERANCES = 0.5; //意味空間の数の半分
	BASE_PATH = "../RESULT/"; //result用フォルダを初期値に設定
	time_t now;
  	struct tm *stm = localtime(&now);
  	char s[100];
  	strftime(s,100,"%Yy%mm%dd%Hh%Mm%Ss",stm);
  	std::string date_str = std::string(s);

 	//initialization with default value
  	MAX_GENERATIONS = 100;
  	PER_UTTERANCES = 0.5;
  	RANDOM_SEED = 101010;
  	UNIQUE_UTTERANCE = false;
  	SAVE_FORMAT = BIN;
  	ANALYZE = false;
  	DICTIONARY_FILE = "data.dic";
  	buzz_length = 3;

  	LOGGING = false;
  	PROGRESS = false;
  	RESUME = false;
  	SAVE_LAST_STATE = false;
  	SAVE_ALL_STATE = false;
  	OMISSION = false;
	DATE_STR = date_str;
  	STATE_EXT = ".st";
  	RESULT_EXT = ".rst";
  	LOG_EXT = ".log";

  	BASE_PATH = "../RESULT/";
  	SAVE_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
  	RESULT_FILE = (FILE_PREFIX + DATE_STR + RESULT_EXT);
  	RESUME_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
  	LOG_FILE = (FILE_PREFIX + DATE_STR + LOG_EXT);

  	CONTROLS = 0x0;

  	UTTERANCES = 0;
  	Generation_Counter = 0;
}

MSILMParameters::~MSILMParameters() {
	// TODO Auto-generated destructor stub
}

void
MSILMParameters::set_option(boost::program_options::variables_map& vm) {
	//Files
  if (vm.count("format")) {
    if (vm["format"].as<std::string>() == "xml")
      SAVE_FORMAT = Parameters::XML;
  }

  if (vm.count("prefix")) {
    FILE_PREFIX = vm["prefix"].as<std::string>();

    SAVE_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
    RESULT_FILE = (FILE_PREFIX + DATE_STR + RESULT_EXT);
    RESUME_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
    LOG_FILE = (FILE_PREFIX + DATE_STR + LOG_EXT);
  }

  if (vm.count("path")) {
    BASE_PATH = (vm["path"].as<std::string>());
  }

  //Set option values
  if (vm.count("random-seed")) {
    RANDOM_SEED = vm["random-seed"].as<uint32_t>();
  }

  if (vm.count("generations")) {
    MAX_GENERATIONS = vm["generations"].as<int>();
  }

  if (vm.count("utterances")) {
    PER_UTTERANCES = vm["utterances"].as<double>();
  }

  if (vm.count("omission")) {
	  OMISSION = true;
  }

  if (vm.count("analyze")) {
    RESULT_FILE = (FILE_PREFIX + RESULT_EXT);
    ANALYZE = true;
  }

  if (vm.count("unique-utterance")) {
    UNIQUE_UTTERANCE = true;
  }

  if (vm.count("dictionary")) {
    DICTIONARY_FILE = vm["dictionary"].as<std::string>();
  }

  if (vm.count("word-length")) {
    buzz_length = vm["word-length"].as<int>();
  }

  if (vm.count("keep-random-rule")) {
    CONTROLS |= KnowledgeBase::USE_ADDITION_OF_RANDOM_WORD;
  }

  if (vm.count("delete-redundant-rules")) {
    CONTROLS |= KnowledgeBase::USE_OBLITERATION;
  }

  if (vm.count("invention")) {
    CONTROLS |= KnowledgeBase::USE_SEMICOMPLETE_FABRICATION;
  }

  if (vm.count("logging")) {
    LOGGING = true;
  }

  if (vm.count("resume")) {
    std::vector<std::string> args;
    args = vm["resume"].as<std::vector<std::string> >();

    if (args.size() > 0) {
      RESUME_FILE = args.front();
    }

    RESUME = true;
  }

  if (vm.count("last-save")) {
    SAVE_LAST_STATE = true;
  }

  if (vm.count("all-save")) {
    SAVE_ALL_STATE = true;
  }

  if (vm.count("progress")) {
    PROGRESS = true;
  }

	if (vm.count("analyze")) {
		ACC_MEA = true;
	}
	if (vm.count("interspace-analysis")) {
		ANALYZE = true;
		INTER_ANALYSIS = true;
		SPACE_ANALYSIS = vm["interspace-analysis"].as<int>();
		ACC_MEA = true;
	}
	if (vm.count("interspace-logging")) {
		LOGGING = true;
		INTER_LOG = true;
		SPACE_LOG = vm["interspace-logging"].as<int>();
		ACC_MEA = true;
	}

	if (vm.count("multiple-meanings")) {
		MULTIPLE_MEANINGS = vm["multiple-meanings"].as<int>();
	}

	if (vm.count("term")) {
		PER_TERM = vm["term"].as<double>();
	}

	if (vm.count("window")) {
		WINDOW = vm["window"].as<int>();
	}

	if (vm.count("symmetry")) {
		SYMMETRY = true;
	}

	if (vm.count("ucsymmetry")) {
		UC_SYMMETRY = true;
	}

	if (vm.count("omission")) {
		OMISSION = true;
	}

	if (vm.count("accuracy-meaning")) {
		ACC_MEA = true;
	}
	//必ずprefixの変更後に行うこと
	SAVE_FILE = (FILE_PREFIX + DATE_STR + "_" + std::to_string(RANDOM_SEED) + STATE_EXT);
	RESULT_FILE = (FILE_PREFIX + DATE_STR + "_" + std::to_string(RANDOM_SEED) + RESULT_EXT);
	RESUME_FILE = (FILE_PREFIX + DATE_STR + "_" + std::to_string(RANDOM_SEED) + STATE_EXT);
	LOG_FILE = (FILE_PREFIX + DATE_STR + "_" + std::to_string(RANDOM_SEED) + LOG_EXT);
}

std::string
MSILMParameters::to_s(void) {
	std::string param1, param2;
	std::vector<std::string> bag;

	//Files
  	if (svm.count("format")) {
    	bag.push_back("--format ");
    	if (svm["format"].as<std::string>() == "xml")
      	bag.push_back("xml");
    	else if (svm["format"].as<std::string>() == "bin")
      	bag.push_back("bin");
  	}

  	if (svm.count("prefix")) {
    	bag.push_back("--prefix");
    	bag.push_back(svm["prefix"].as<std::string>());
  	}

  	if (svm.count("path")) {
    	bag.push_back("--path");
    	bag.push_back(svm["path"].as<std::string>());
  	}

  	//Set option values
  	if (svm.count("random-seed")) {
    	bag.push_back("--random-seed");
    	bag.push_back(
        	std::to_string(svm["random-seed"].as<uint32_t>()));
  	}

  	if (svm.count("generations")) {
    	bag.push_back("--generations");
    	bag.push_back(
        	std::to_string(svm["generations"].as<int>()));
  	}

  	if (svm.count("utterances")) {
    	bag.push_back("--utterances");
    	bag.push_back(
        	std::to_string(svm["utterances"].as<double>()));
  	}

  	if (svm.count("analyze")) {
    	bag.push_back("--analyze");
  	}

  	if (svm.count("unique-utterance")) {
    	bag.push_back("--unique-utterance");
  	}

  	if (svm.count("dictionary")) {
    	bag.push_back("--dictionary");
    	bag.push_back(svm["dictionary"].as<std::string>());
  	}

  	if (svm.count("word-length")) {
    	bag.push_back("--word-length");
   		bag.push_back(
        std::to_string(svm["word-length"].as<int>()));
  	}

  	if (svm.count("keep-random-rule")) {
    	bag.push_back("--keep-random-rule");
  	}

  	if (svm.count("delete-redundant-rules")) {
    	bag.push_back("--delete-redundant-rules");
  	}

  	if (svm.count("invention")) {
    	bag.push_back("--invention");
  	}

  	if (svm.count("logging")) {
    	bag.push_back("--logging");
  	}

  	if (svm.count("resume")) {
    	bag.push_back("--resume");
    	std::vector<std::string> args;
    	args = svm["resume"].as<std::vector<std::string> >();

    	if (args.size() > 0) {
      		bag.push_back(args.front());
    	}
  	}

  	if (svm.count("last-save")) {
    	bag.push_back("--last-save");
  	}

  	if (svm.count("all-save")) {
    	bag.push_back("--all-save");
  	}

  	if (svm.count("progress")) {
    	bag.push_back("--progress");
  	}

	if (svm.count("interspace-analysis")) {
		bag.push_back("--interspace-analysis " + std::to_string(svm["interspace-analysis"].as<int>()));
	}
	if (svm.count("interspace-logging")) {
		bag.push_back("--interspace-logging " + std::to_string(svm["interspace-logging"].as<int>()));
	}

	if (svm.count("multiple-meanings")) {
		bag.push_back("--multiple-meanings " + std::to_string(svm["multiple-meanings"].as<int>()));
	}

	if (svm.count("term")) {
		bag.push_back("--term " + std::to_string(svm["term"].as<double>()));
	}

	if (svm.count("window")) {
		bag.push_back("--window " + std::to_string(svm["window"].as<int>()));
	}

	if (svm.count("symmetry")) {
		bag.push_back("--symmetry ");
	}

	if (svm.count("ucsymmetry")) {
		bag.push_back("--ucsymmetry ");
	}

	if (svm.count("omission")) {
		bag.push_back("--omission ");
	}

	if (svm.count("accuracy-meaning")) {
		bag.push_back("--accuracy-meaning ");
	}

	return string_join(bag, " ");
}

std::string
MSILMParameters::string_join(const std::vector<std::string> & str_v, const std::string & delim){
    std::ostringstream os;
    std::copy(str_v.begin(), str_v.end(), std::ostream_iterator<std::string>(os, delim.c_str()));
    std::string str = os.str();
    str.erase(str.end()-delim.size(),str.end());
    return str;
}