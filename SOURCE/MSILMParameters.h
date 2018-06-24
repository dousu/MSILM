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
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <variant>
#include <type_traits>
#include <map>
#include <iterator>

#include "LogBox.h"
#include "IndexFactory.h"

struct OptionValue{
  using OptionTypes = std::variant<bool,int,double,std::string>;
private:
  OptionTypes obj;
public:
  OptionValue(){};

  template<typename T>
  OptionValue(T val){
    obj = val;
  };
  OptionValue & operator=(const OptionValue & other){
    if(other.check_type<int>()){
      obj = other.get<int>();
    }else if(other.check_type<double>()){
      obj = other.get<double>();
    }else if(other.check_type<std::string>()){
      obj = other.get<std::string>();
    }else if(other.check_type<bool>()){
      obj = other.get<bool>();
    }else{
      std::cerr << "irregular option value" << std::endl;
      exit(1);
    }
    return *this;
  };

  template<typename T>
  OptionValue & operator=(const T val){
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
    OptionTypes value = val;
    obj = value;
    return *this;
  };

  template<typename T>
  T get() const{
    return std::get<T>(obj);
  }

  template<typename T>
  bool check_type() const{
    return std::holds_alternative<T>(obj);
  }
};
struct ProgramOption{
private:
  IndexFactory idx;
  std::map<std::string, int> id;
  std::map<int, OptionValue> val_list;
  std::map<int, std::string> desc_list;
  enum type_id {itype, dtype, stype, btype};
  std::map<int, type_id> type_inf;
public:
  ProgramOption(){};
  ProgramOption(const ProgramOption & po){
  	idx = po.idx;
  	id = po.id;
  	val_list = po.val_list;
  	desc_list = po.desc_list;
  	type_inf = po.type_inf;
  }

  ProgramOption & operator=(const ProgramOption & po){
  	idx = po.idx;
  	id = po.id;
  	val_list = po.val_list;
  	desc_list = po.desc_list;
  	type_inf = po.type_inf;
  	return *this;
  }

  template<typename T>
  static T value(){return T();}

  template<typename T>
  static T value(T obj){return T(obj);}

  int count(std::string str){
  	return id.count(str);
  }

  ProgramOption & add_option(){return *this;}

  ProgramOption & operator()(std::string key, OptionValue val, std::string str){
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
  ProgramOption & operator()(std::string key, std::string str){
    int index = idx.generate();
    id[key] = index;
    val_list[index] = bool(false);
    desc_list[index] = str;
    type_inf[index] = btype;
    return *this;
  }

  template<typename T>
  T get(std::string str){
    return val_list[id[str]].get<T>();
  }
  void help(){
    std::string help_comment = "HELP:\n";
    for(auto index : id){
      help_comment += "  " + index.first + "\t" + desc_list[index.second] + "\n";
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
          type_id ti = type_inf[id[key]];
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
        key = option;
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
};

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

   	ProgramOption spo;
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

	void set_option(ProgramOption& po);
	std::string to_s(void);
private:
	std::string string_join(const std::vector<std::string> & str_v, const std::string & delim);
};
#endif /* MSILMPARAMETERS_H_ */
