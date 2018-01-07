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
  MULTIPLE_MEANINGS=2;
  PER_TERM=1.0;
  TERMS=0;
  WINDOW=1;
  SYMMETRY=false;
  UC_SYMMETRY = false;
  OMISSION=false;
  ACC_MEA=false;
  SAVE_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
  RESULT_FILE = (FILE_PREFIX + DATE_STR + RESULT_EXT);
  RESUME_FILE = (FILE_PREFIX + DATE_STR + STATE_EXT);
  LOG_FILE = (FILE_PREFIX + DATE_STR + LOG_EXT);
  DICTIONARY_FILE = "./data.dic"; 
  PER_UTTERANCES = 0.5; //意味空間の数の半分
  BASE_PATH = "../RESULT/"; //result用フォルダを初期値に設定
}

MSILMParameters::~MSILMParameters() {
  // TODO Auto-generated destructor stub
}

void
MSILMParameters::set_option(boost::program_options::variables_map& vm) {
  Parameters::set_option(vm);

  if (vm.count("interspace-analysis")) {
    INTER_ANALYSIS = true;
    SPACE_ANALYSIS = vm["interspace-analysis"].as<int>();
  }
  if (vm.count("interspace-logging")) {
    INTER_LOG = true;
    SPACE_LOG = vm["interspace-logging"].as<int>();
  }
  
  if (vm.count("multiple-meanings")){
    MULTIPLE_MEANINGS=vm["multiple-meanings"].as<int>();
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
    SAVE_FILE = (FILE_PREFIX + DATE_STR + "_" + boost::lexical_cast<std::string>(RANDOM_SEED) + STATE_EXT);
    RESULT_FILE = (FILE_PREFIX + DATE_STR + "_" + boost::lexical_cast<std::string>(RANDOM_SEED) + RESULT_EXT);
    RESUME_FILE = (FILE_PREFIX + DATE_STR + "_" + boost::lexical_cast<std::string>(RANDOM_SEED) + STATE_EXT);
    LOG_FILE = (FILE_PREFIX + DATE_STR + "_" + boost::lexical_cast<std::string>(RANDOM_SEED) + LOG_EXT);
}

std::string
MSILMParameters::to_s(void) {
  std::string param1, param2;
  std::vector<std::string> bag;
  param1 = Parameters::to_s();
  bag.push_back(param1);

  if (svm.count("interspace-analysis")) {
    bag.push_back("--interspace-analysis " + boost::lexical_cast<std::string>(svm["interspace-analysis"].as<int>()));
  }
  if (svm.count("interspace-logging")) {
    bag.push_back("--interspace-logging " + boost::lexical_cast<std::string>(svm["interspace-logging"].as<int>()));
  }

  if (svm.count("multiple-meanings")){
      bag.push_back("--multiple-meanings " + boost::lexical_cast<std::string>(svm["multiple-meanings"].as<int>()));
  }
  
  if (svm.count("term")){
      bag.push_back("--term " + boost::lexical_cast<std::string>(svm["term"].as<double>()));
  }
  
  if (svm.count("window")){
      bag.push_back("--window " + boost::lexical_cast<std::string>(svm["window"].as<int>()));
  }
  
  if (svm.count("symmetry")) {
    bag.push_back("--symmetry ");
  }

  if (svm.count("ucsymmetry")) {
	  bag.push_back("--ucsymmetry ");
  }

  if (svm.count("omission")){
      bag.push_back("--omission ");
  }

  if (svm.count("accuracy-meaning")){
      bag.push_back("--accuracy-meaning ");
  }

  return boost::algorithm::join(bag, " ");
}
