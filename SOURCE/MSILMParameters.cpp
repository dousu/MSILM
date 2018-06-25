/*
* File:   MSILMParameters.cpp
* Author: hiroki
*
* Created on October 26, 2013, 5:12 PM
*/

#include "MSILMParameters.h"

MSILMParameters::MSILMParameters() {
	// spo = ProgramOption();
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
	// SAVE_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
	RESULT_FILE = (FILE_PREFIX + DATE_STR + RESULT_EXT);
	// RESUME_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
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
  	// SAVE_FORMAT = BIN;
  	ANALYZE = false;
  	DICTIONARY_FILE = "data.dic";
  	buzz_length = 3;

  	LOGGING = false;
  	// PROGRESS = false;
  	// RESUME = false;
  	// SAVE_LAST_STATE = false;
  	// SAVE_ALL_STATE = false;
  	OMISSION = false;
	DATE_STR = date_str;
  	STATE_EXT = ".st";
  	RESULT_EXT = ".rst";
  	LOG_EXT = ".log";

  	BASE_PATH = "../RESULT/";
  	// SAVE_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
  	RESULT_FILE = (FILE_PREFIX + DATE_STR + RESULT_EXT);
  	// RESUME_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
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
  // if (po.count("format")) {
  //   if (po.get<std::string>("format") == "xml")
  //     SAVE_FORMAT = XML;
  // }

  if (po.count("prefix")) {
    FILE_PREFIX = po.get<std::string>("prefix");

    SAVE_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
    RESULT_FILE = (FILE_PREFIX + DATE_STR + RESULT_EXT);
    // RESUME_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
    LOG_FILE = (FILE_PREFIX + DATE_STR + LOG_EXT);
  }

  if (po.count("path")) {
    BASE_PATH = (po.get<std::string>("path"));
  }

  //Set option values
  if (po.count("random-seed")) {
    RANDOM_SEED = po.get<int>("random-seed");
  }

  if (po.count("generations")) {
    MAX_GENERATIONS = po.get<int>("generations");
  }

  if (po.count("utterances")) {
    PER_UTTERANCES = po.get<double>("utterances");
  }

  if (po.count("omission")) {
	  OMISSION = po.get<bool>("omission");
  }

  if (po.count("analyze")) {
    RESULT_FILE = (FILE_PREFIX + RESULT_EXT);
    ANALYZE = po.get<bool>("analyze");
  }

  if (po.count("unique-utterance")) {
    UNIQUE_UTTERANCE = po.get<bool>("unique-utterance");
  }

  if (po.count("dictionary")) {
    DICTIONARY_FILE = po.get<std::string>("dictionary");
  }

  if (po.count("word-length")) {
    buzz_length = po.get<int>("word-length");
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
    LOGGING = po.get<bool>("logging");
  }

  // if (po.count("resume")) {
  //   std::vector<std::string> args;
  //   args = po["resume"].as<std::vector<std::string> >();

  //   if (args.size() > 0) {
  //     RESUME_FILE = args.front();
  //   }

  //   RESUME = true;
  // }

  // if (po.count("last-save")) {
  //   SAVE_LAST_STATE = true;
  // }

  // if (po.count("all-save")) {
  //   SAVE_ALL_STATE = true;
  // }

  // if (po.count("progress")) {
  //   PROGRESS = po.get<bool>("progress");
  // }

	if (po.count("analyze")) {
		ACC_MEA = po.get<bool>("analyze");
	}
	if (po.count("interspace-analysis")) {
		ANALYZE = true;
		INTER_ANALYSIS = true;
		SPACE_ANALYSIS = po.get<int>("interspace-analysis");
		ACC_MEA = true;
	}
	if (po.count("interspace-logging")) {
		LOGGING = true;
		INTER_LOG = true;
		SPACE_LOG = po.get<bool>("interspace-logging");
		ACC_MEA = true;
	}

	if (po.count("multiple-meanings")) {
		MULTIPLE_MEANINGS = po.get<int>("multiple-meanings");
	}

	if (po.count("term")) {
		PER_TERM = po.get<double>("term");
	}

	if (po.count("window")) {
		WINDOW = po.get<int>("window");
	}

	if (po.count("symmetry")) {
		SYMMETRY = po.get<bool>("symmetry");
	}

	if (po.count("ucsymmetry")) {
		UC_SYMMETRY = po.get<bool>("ucsymmetry");
	}

	if (po.count("omission")) {
		OMISSION = po.get<bool>("omission");
	}

	if (po.count("accuracy-meaning")) {
		ACC_MEA = po.get<bool>("accuracy-meaning");
	}
	//必ずprefixの変更後に行うこと
	// SAVE_FILE = (FILE_PREFIX + DATE_STR + "_" + std::to_string(RANDOM_SEED) + STATE_EXT);
	RESULT_FILE = (FILE_PREFIX + DATE_STR + "_" + std::to_string(RANDOM_SEED) + RESULT_EXT);
	// RESUME_FILE = (FILE_PREFIX + DATE_STR + "_" + std::to_string(RANDOM_SEED) + STATE_EXT);
	LOG_FILE = (FILE_PREFIX + DATE_STR + "_" + std::to_string(RANDOM_SEED) + LOG_EXT);
}

std::string
MSILMParameters::to_s(void) {
	std::string param1, param2;
	std::vector<std::string> bag;

	//Files
  	// if (spo.count("format")) {
   //  	bag.push_back("--format ");
   //  	if (spo.get<std::string>("format") == "xml")
   //    		bag.push_back("xml");
   //  	else if (spo.get<std::string>("format") == "bin")
   //    		bag.push_back("bin");
  	// }

  	if (spo.count("prefix")) {
    	bag.push_back("--prefix");
    	bag.push_back(spo.get<std::string>("prefix"));
  	}

  	if (spo.count("path")) {
    	bag.push_back("--path");
    	bag.push_back(spo.get<std::string>("path"));
  	}

  	//Set option values
  	if (spo.count("random-seed")) {
    	bag.push_back("--random-seed");
    	bag.push_back(
        	std::to_string(spo.get<int>("random-seed")));
  	}

  	if (spo.count("generations")) {
    	bag.push_back("--generations");
    	bag.push_back(
        	std::to_string(spo.get<int>("generations")));
  	}

  	if (spo.count("utterances")) {
    	bag.push_back("--utterances");
    	bag.push_back(
        	std::to_string(spo.get<double>("utteranes")));
  	}

  	if (spo.count("analyze")) {
    	bag.push_back("--analyze");
    	bag.push_back(
    		std::to_string(spo.get<bool>("analyze")));
  	}

  	if (spo.count("unique-utterance")) {
    	bag.push_back("--unique-utterance");
    	bag.push_back(
    		std::to_string(spo.get<bool>("unique-utterance")));
  	}

  	if (spo.count("dictionary")) {
    	bag.push_back("--dictionary");
    	bag.push_back(spo.get<std::string>("dictionary"));
  	}

  	if (spo.count("word-length")) {
    	bag.push_back("--word-length");
   		bag.push_back(
        	std::to_string(spo.get<int>("word-length")));
  	}

  	if (spo.count("keep-random-rule")) {
    	bag.push_back("--keep-random-rule");
    	bag.push_back(
    		std::to_string(spo.get<bool>("keep-random-rule")));
  	}

  	if (spo.count("delete-redundant-rules")) {
    	bag.push_back("--delete-redundant-rules");
    	bag.push_back(
    		std::to_string(spo.get<bool>("delete-redundant-rules")));
  	}

  	if (spo.count("invention")) {
    	bag.push_back("--invention");
    	bag.push_back(
    		std::to_string(spo.get<bool>("invention")));
  	}

  	if (spo.count("logging")) {
    	bag.push_back("--logging");
    	bag.push_back(
    		std::to_string(spo.get<bool>("logging")));
  	}

  	// if (spo.count("last-save")) {
   //  	bag.push_back("--last-save");
  	// }

  	// if (spo.count("all-save")) {
   //  	bag.push_back("--all-save");
  	// }

  	// if (spo.count("progress")) {
   //  	bag.push_back("--progress");
   //  	bag.push_back(
   //  		std::to_string(spo.get<bool>("progress")));
  	// }

	if (spo.count("interspace-analysis")) {
		bag.push_back("--interspace-analysis");
		bag.push_back(
			std::to_string(spo.get<int>("interspace-analysis")));
	}
	if (spo.count("interspace-logging")) {
		bag.push_back("--interspace-logging");
		bag.push_back(
			std::to_string(spo.get<int>("interspace-logging")));
	}

	if (spo.count("multiple-meanings")) {
		bag.push_back("--multiple-meanings");
		bag.push_back(
			std::to_string(spo.get<int>("multiple-meanings")));
	}

	if (spo.count("term")) {
		bag.push_back("--term");
		bag.push_back(
			std::to_string(spo.get<int>("term")));
	}

	if (spo.count("window")) {
		bag.push_back("--window");
		bag.push_back(
			std::to_string(spo.get<int>("window")));
	}

	if (spo.count("symmetry")) {
		bag.push_back("--symmetry ");
		bag.push_back(
			std::to_string(spo.get<bool>("symmetry")));
	}

	if (spo.count("ucsymmetry")) {
		bag.push_back("--ucsymmetry ");
		bag.push_back(
			std::to_string(spo.get<bool>("ucsymmetry")));
	}

	if (spo.count("omission")) {
		bag.push_back("--omission ");
		bag.push_back(
			std::to_string(spo.get<bool>("omission")));
	}

	if (spo.count("accuracy-meaning")) {
		bag.push_back("--accuracy-meaning ");
		bag.push_back(
			std::to_string(spo.get<bool>("accuracy-meaning")));
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