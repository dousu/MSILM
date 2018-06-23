/*
 * Parameters.cpp
 *
 *  Created on: 2012/12/24
 *      Author: Hiroki Sudo
 */

#include "Parameters.h"

Parameters::Parameters() {
  time_t now;
  time(&now);
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

  FILE_PREFIX = "KILM_";
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

Parameters::~Parameters() {
//	// TODO Auto-generated destructor stub
}

void
Parameters::set_option(boost::program_options::variables_map& vm) {
  svm = vm;

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
}

std::string
Parameters::to_s(void) {
  std::vector<std::string> bag;
  std::string str = "";

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

  return string_join(bag, " ");
}

std::string
Parameters::string_join(const std::vector<std::string> & str_v, const std::string & delim){
    std::ostringstream os;
    std::copy(str_v.begin(), str_v.end(), std::ostream_iterator<std::string>(os, delim.c_str()));
    std::string str = os.str();
    str.erase(str.end()-delim.size(),str.end());
    return str;
}

/*
#include <iostream>
#include <type_traits>
#include <variant>
#include <map>
#include <iterator>
#include <string>
#include <sstream>

#include "IndexFactory.h"

struct E{
  using F=std::variant<bool,int,double,std::string>;
private:
  F obj;
public:
  E(){};

  template<typename T>
  E(T val){
    obj = val;
  };
  E & operator=(const E & other){
    if(other.check_type<int>()){
      obj = other.var_get<int>();
    }else if(other.check_type<double>()){
      obj = other.var_get<double>();
    }else if(other.check_type<std::string>()){
      obj = other.var_get<std::string>();
    }else if(other.check_type<bool>(other.obj)){
      obj = other.var_get<bool>();
    }else{
      std::cerr << "irregular option value" << std::endl;
      exit(1);
    }
    return *this;
  };

  template<typename T>
  E & operator=(const T val){
    //型チェック
    if(check_type<int>()){
      if(!std::is_same<int,T>::value){
        std::cerr << "conflict type data" << std::endl;
      }
    }else if(check_type<double>()){
      if(!std::is_same<double,T>::value){
        std::cerr << "conflict type data" << std::endl;
      }
    }else if(check_type<std::string>()){
      if(!std::is_same<std::string,T>::value){
        std::cerr << "conflict type data" << std::endl;
      }
    }else if(check_type<bool>()){
      if(!std::is_same<bool,T>::value){
        std::cerr << "conflict type data" << std::endl;
      }
    }else{
      std::cerr << "irregular option value" << std::endl;
      exit(1);
    }
    C value = val;
    obj = value;
    return *this;
  };

  template<typename T>
  T var_get() const{
    return std::get<T>(obj);
  }

  template<typename T>
  bool check_type(){
    return std::holds_alternative<T>(obj);
  }
};
struct G{
  IndexFactory idx;
  std::map<std::string, int> id;
  std::map<int, E> val_list;
  std::map<int, std::string> desc_list;
  enum type_id {itype, dtype, stype, btype};
  std::map<int, type_id> type_inf;

  template<typename T>
  static T value(){return T();}

  template<typename T>
  static T value(T obj){return T(obj);}

  G & add_option(){return *this;}

  G & operator()(std::string key, E val, std::string str){
    int index = idx.generate();
    id[key] = index;
    val_list[index] = val;
    desc_list[index] = str;
    if(val.check_type<int>()){
      type_inf[index] = itype;
    }else if(val.check_type<double>()){
      type_inf[index] = dtype;
    }else if(val.check_type<std::string>()){
      type_inf[index] = stype;
    }else if(val.check_type<bool>()){
      type_inf[index] = btype;
    }else{
      std::cerr << "unknown type" << std::endl;
    }
    return *this;
  }
  G & operator()(std::string key, std::string str){
    int index = idx.generate();
    id[key] = index;
    val_list[index] = bool(false);
    desc_list[index] = str;
    type_inf[index] = btype;
    return *this;
  }

  template<typename T>
  T get(std::string str){
    return val_list[id[str]].var_get<T>();
  }
  void help(){
    std::string help_comment = "HELP:\n";
    for(auto index : id){
      help_comment += "  " + id.first + "\t" + desc_list[id.second].second + "\n";
    }
    std::cout << help_comment << std::endl;
    exit(0);
  }
  void parse(int argc, char* argv[]){
    int i = 1;
    std::string key = "";
    std::stringstream ss;
    while(i < argc){
      std::string str(argv[i]);
      std::string option;
      if(str.find("-") != 0 ||
        (str.find("-") == 0 && str.size() < 2) ||
        (str.find("--") == 0 && str.size() < 3)){
        if(id.count(key) == 0){
          std::cerr << "incorrect number of options" << std::endl;
          exit(1);
        }else{
          type_id ti = type_inf[key];
          bool b;
          switch(ti){
          case itype:
            val_list[id[key]] = std::stoi(str);
            break;
          case dtype:
            val_list[id[key]] = std::stod(str);
            break;
          case stype:
            val_list[id[key]] = str;
            break;
          case btype:
            ss << std::boolalpha << str;
            ss >> b;
            ss << std::noboolalpha;
            val_list[id[key]] = b;
            break;
          default:
            std::cerr << "unknown type_id" << std::endl;
            exit(1);
          }
        }
        key = "";
        i++;
        continue;
      }
      if(key != ""){
        val_list[id[key]] = true;
      }
      if(str.find("--") == 0){
        std::copy(std::begin(str)+2, std::end(str), std::back_inserter(option));
      }else{
        std::copy(std::begin(str)+1, std::end(str), std::back_inserter(option));
      }
      if(option == "h" || option == "help"){
        help();
      }
      if(id.count(option)){
        key = option
        i++;
      }else{
        std::cerr << "unknown option" << std::endl;
        exit(1);
      }
    }
    if(key != ""){
      val_list[id[key]] = true;
    }
  }
}
*/