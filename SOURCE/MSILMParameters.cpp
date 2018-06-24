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
MSILMParameters::set_option(ProgramOption & po) {
  spo = po;
  //Files
  if (po.count("format")) {
    if (po["format"].as<std::string>() == "xml")
      SAVE_FORMAT = XML;
  }

  if (po.count("prefix")) {
    FILE_PREFIX = po["prefix"].as<std::string>();

    SAVE_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
    RESULT_FILE = (FILE_PREFIX + DATE_STR + RESULT_EXT);
    RESUME_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
    LOG_FILE = (FILE_PREFIX + DATE_STR + LOG_EXT);
  }

  if (po.count("path")) {
    BASE_PATH = (po["path"].as<std::string>());
  }

  //Set option values
  if (po.count("random-seed")) {
    RANDOM_SEED = po["random-seed"].as<uint32_t>();
  }

  if (po.count("generations")) {
    MAX_GENERATIONS = po["generations"].as<int>();
  }

  if (po.count("utterances")) {
    PER_UTTERANCES = po["utterances"].as<double>();
  }

  if (po.count("omission")) {
	  OMISSION = true;
  }

  if (po.count("analyze")) {
    RESULT_FILE = (FILE_PREFIX + RESULT_EXT);
    ANALYZE = true;
  }

  if (po.count("unique-utterance")) {
    UNIQUE_UTTERANCE = true;
  }

  if (po.count("dictionary")) {
    DICTIONARY_FILE = po["dictionary"].as<std::string>();
  }

  if (po.count("word-length")) {
    buzz_length = po["word-length"].as<int>();
  }

  if (po.count("keep-random-rule")) {
    CONTROLS |= KnowledgeBase::USE_ADDITION_OF_RANDOM_WORD;
  }

  if (po.count("delete-redundant-rules")) {
    CONTROLS |= KnowledgeBase::USE_OBLITERATION;
  }

  if (po.count("invention")) {
    CONTROLS |= KnowledgeBase::USE_SEMICOMPLETE_FABRICATION;
  }

  if (po.count("logging")) {
    LOGGING = true;
  }

  if (po.count("resume")) {
    std::vector<std::string> args;
    args = po["resume"].as<std::vector<std::string> >();

    if (args.size() > 0) {
      RESUME_FILE = args.front();
    }

    RESUME = true;
  }

  if (po.count("last-save")) {
    SAVE_LAST_STATE = true;
  }

  if (po.count("all-save")) {
    SAVE_ALL_STATE = true;
  }

  if (po.count("progress")) {
    PROGRESS = true;
  }

	if (po.count("analyze")) {
		ACC_MEA = true;
	}
	if (po.count("interspace-analysis")) {
		ANALYZE = true;
		INTER_ANALYSIS = true;
		SPACE_ANALYSIS = po["interspace-analysis"].as<int>();
		ACC_MEA = true;
	}
	if (po.count("interspace-logging")) {
		LOGGING = true;
		INTER_LOG = true;
		SPACE_LOG = po["interspace-logging"].as<int>();
		ACC_MEA = true;
	}

	if (po.count("multiple-meanings")) {
		MULTIPLE_MEANINGS = po["multiple-meanings"].as<int>();
	}

	if (po.count("term")) {
		PER_TERM = po["term"].as<double>();
	}

	if (po.count("window")) {
		WINDOW = po["window"].as<int>();
	}

	if (po.count("symmetry")) {
		SYMMETRY = true;
	}

	if (po.count("ucsymmetry")) {
		UC_SYMMETRY = true;
	}

	if (po.count("omission")) {
		OMISSION = true;
	}

	if (po.count("accuracy-meaning")) {
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
  	if (spo.count("format")) {
    	bag.push_back("--format ");
    	if (spo["format"].as<std::string>() == "xml")
      	bag.push_back("xml");
    	else if (spo["format"].as<std::string>() == "bin")
      	bag.push_back("bin");
  	}

  	if (spo.count("prefix")) {
    	bag.push_back("--prefix");
    	bag.push_back(spo["prefix"].as<std::string>());
  	}

  	if (spo.count("path")) {
    	bag.push_back("--path");
    	bag.push_back(spo["path"].as<std::string>());
  	}

  	//Set option values
  	if (spo.count("random-seed")) {
    	bag.push_back("--random-seed");
    	bag.push_back(
        	std::to_string(spo["random-seed"].as<uint32_t>()));
  	}

  	if (spo.count("generations")) {
    	bag.push_back("--generations");
    	bag.push_back(
        	std::to_string(spo["generations"].as<int>()));
  	}

  	if (spo.count("utterances")) {
    	bag.push_back("--utterances");
    	bag.push_back(
        	std::to_string(spo["utterances"].as<double>()));
  	}

  	if (spo.count("analyze")) {
    	bag.push_back("--analyze");
  	}

  	if (spo.count("unique-utterance")) {
    	bag.push_back("--unique-utterance");
  	}

  	if (spo.count("dictionary")) {
    	bag.push_back("--dictionary");
    	bag.push_back(spo["dictionary"].as<std::string>());
  	}

  	if (spo.count("word-length")) {
    	bag.push_back("--word-length");
   		bag.push_back(
        std::to_string(spo["word-length"].as<int>()));
  	}

  	if (spo.count("keep-random-rule")) {
    	bag.push_back("--keep-random-rule");
  	}

  	if (spo.count("delete-redundant-rules")) {
    	bag.push_back("--delete-redundant-rules");
  	}

  	if (spo.count("invention")) {
    	bag.push_back("--invention");
  	}

  	if (spo.count("logging")) {
    	bag.push_back("--logging");
  	}

  	if (spo.count("resume")) {
    	bag.push_back("--resume");
    	std::vector<std::string> args;
    	args = spo["resume"].as<std::vector<std::string> >();

    	if (args.size() > 0) {
      		bag.push_back(args.front());
    	}
  	}

  	if (spo.count("last-save")) {
    	bag.push_back("--last-save");
  	}

  	if (spo.count("all-save")) {
    	bag.push_back("--all-save");
  	}

  	if (spo.count("progress")) {
    	bag.push_back("--progress");
  	}

	if (spo.count("interspace-analysis")) {
		bag.push_back("--interspace-analysis " + std::to_string(spo["interspace-analysis"].as<int>()));
	}
	if (spo.count("interspace-logging")) {
		bag.push_back("--interspace-logging " + std::to_string(spo["interspace-logging"].as<int>()));
	}

	if (spo.count("multiple-meanings")) {
		bag.push_back("--multiple-meanings " + std::to_string(spo["multiple-meanings"].as<int>()));
	}

	if (spo.count("term")) {
		bag.push_back("--term " + std::to_string(spo["term"].as<double>()));
	}

	if (spo.count("window")) {
		bag.push_back("--window " + std::to_string(spo["window"].as<int>()));
	}

	if (spo.count("symmetry")) {
		bag.push_back("--symmetry ");
	}

	if (spo.count("ucsymmetry")) {
		bag.push_back("--ucsymmetry ");
	}

	if (spo.count("omission")) {
		bag.push_back("--omission ");
	}

	if (spo.count("accuracy-meaning")) {
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