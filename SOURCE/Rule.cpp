/*
 * Rule.cpp
 *
 *  Created on: 2012/11/18
 *      Author: Hiroki Sudo
 */
#include "Rule.h"

Dictionary Rule::dictionary = Dictionary::copy();
/*
 Rule::~Rule() {
 }
 */

Rule::Rule()
{
}
Rule::~Rule()
{
}

Rule::Rule(char* cstr) {
  Rule(std::string(cstr));
}

Rule::Rule(std::string str) {
  internal.clear();
  external.clear();
  std::regex arrow(Prefices::ARW),
    slash(Prefices::DEL),
    pas(std::string("[") + Prefices::LPRN + Prefices::RPRN + Prefices::CNM + std::string("]")),
    num("[1-9][0-9]*"),
    nums(".([1-9][0-9]*)\\/.([1-9][0-9]*)");
  std::string l, r, cat_alp, mean, category;
  std::vector<std::string> meanings;

  std::sregex_token_iterator it = std::sregex_token_iterator();
  std::sregex_token_iterator it1(std::begin(str), std::end(str), arrow, -1);
  int i = 0;
  for(; it1 != it; it1++, i++){
    if(i == 2){
      std::cerr << "too many right arrow" << std::endl;
      exit(1);
    }else if(i == 0){
      l = *it1;
    }else{
      r = *it1;
    }
  }
  std::sregex_token_iterator it2(std::begin(l), std::end(l), slash, -1);
  for(i = 0; it2 != it; it2++, i++){
    if(i == 2){
      std::cerr << "too many slash" << std::endl;
      exit(1);
    }else if(i == 0){
      cat_alp = *it2;
    }else{
      mean = *it2;
    }
  }
  int cnum;
  std::sregex_token_iterator it3(std::begin(mean), std::end(mean), pas, -1);
  std::copy(it3, it, std::back_inserter(meanings));
  if(cat_alp.size() >= 2){
    std::sregex_token_iterator it4(std::begin(cat_alp), std::end(cat_alp), num, {-1,0});
    for(i = 0; it4 != it; it4++, i++){
      if(i == 2){
        std::cerr << "A category name can't include numbers." << std::endl;
        exit(1);
      }else if(i == 0){
        category = *it4;
      }else{
        cnum = std::stoi(*it4);
      }
    }
    if(category != Prefices::CAT){
      std::cerr << category << " is not category charactor." << std::endl;
      exit(1);
    }else{
      type = RULE_TYPE::NOUN;
      cat = cnum;
    }
  }else{
    category = cat_alp;
    cnum = 0;
    if(category == Prefices::SEN) {
      type = RULE_TYPE::SENTENCE;
      cat = 0;
    }else{
      std::cerr << category << " is not start symbol." << std::endl;
      exit(1);
    }
  }
  std::map<int,int> var_cat;
  std::vector<std::variant<int,Element>> in_list;
  for(auto meaning : meanings){
    std::sregex_token_iterator it5(std::begin(meaning), std::end(meaning), num, {-1,0});
    for(i = 0; it5 != it; it5++, i++){
      if(i == 2){
        std::cerr << "A category name can't include numbers." << std::endl;
        exit(1);
      }else if(i == 0){
        if(std::string(*it5) != Prefices::VAR){
          Element el;
          std::map<std::string, int>::iterator dic_it;
          dic_it = dictionary.conv_individual.find(std::string(*it5));
          if (dic_it != dictionary.conv_individual.end()) {
            el = Mean((*dic_it).second);
          }else{
	    std::cout << "meaning \"" << *it5 << "\"" << std::endl;
            std::cerr << "no candidate in dictionary" << std::endl;
            exit(1);
          }
          in_list.push_back(el);
        }
      }else{
        int ivar = std::stoi(*it5);
        in_list.push_back(ivar);
        var_cat[ivar] = int();
      }
    }
  }
  auto r_it = std::begin(r);
  for(; r_it != std::end(r); r_it++){
    if(std::string{*r_it} == Prefices::CAT){
      int var_num, cat_num;
      std::sregex_token_iterator it6(r_it, std::end(r), nums, {1,2});
      for(i = 0; it6 != it; it6++, i++){
        if(i == 2){
          break;
        }else if(i == 1){
          var_num = std::stoi(*it6);
          r_it += std::string(*it6).size();
        }else{
          cat_num = std::stoi(*it6);
          r_it += std::string(*it6).size();
        }
      }
      var_cat[var_num] = cat_num;
      Element ex_cat;
      ex_cat=Nonterminal(cat_num, var_num);
      external.push_back(ex_cat);
      r_it ++;// for slash
      r_it ++;// for variable symbol
    }else{
      Element sym;
      std::map<std::string, int>::iterator dic_it;
      dic_it = dictionary.conv_symbol.find(std::string{*r_it});
      if (dic_it != dictionary.conv_symbol.end()) {
        sym = Symbol((*dic_it).second);
      }else{
        std::cerr << "no candidate in dictionary" << std::endl;
        exit(1);
      }
      external.push_back(sym);
    }
  }
  for(auto il : in_list){
    if(std::holds_alternative<int>(il)){
      int var_num = std::get<int>(il), cat_num = var_cat[var_num];
      Element mean;
      mean=Variable(cat_num, var_num);
      internal.push_back(mean);
    }else if(std::holds_alternative<Element>(il)){
      internal.push_back(std::get<Element>(il));
    }
  }
}

/*
 *
 * Operators
 *
 */

bool Rule::operator!=(Rule &dst) const
{
  return !(*this == dst);
}

bool Rule::operator==(const Rule &dst) const
{
  if (type == dst.type && internal.size() == dst.internal.size() && external.size() == dst.external.size())
  {
    switch (type)
    {
    case RULE_TYPE::SENTENCE:
      if (internal == dst.internal && external == dst.external)
        return true;
      break;

    case RULE_TYPE::NOUN:
      if (cat == dst.cat && internal == dst.internal && external == dst.external)
        return true;
      break;

    default:
      std::cout << "error type" << std::endl;
    }
  }

  return false;
}

/*
 bool Rule::operator<(Rule& dst) const{
 return ;
 }
 */

Rule &
Rule::operator=(const Rule &dst)
{
  type = dst.type;
  cat = dst.cat;
  internal = dst.internal;
  external = dst.external;

  return *this;
}

bool Rule::is_sentence(void) const
{
  if (type == RULE_TYPE::SENTENCE)
    return true;
  return false;
}

bool Rule::is_noun(void) const
{
  if (type == RULE_TYPE::NOUN)
    return true;
  return false;
}

int Rule::composition(void) const
{
  InType::const_iterator it;
  int comp;
  comp = 0;

  for (it = internal.begin(); it != internal.end(); it++)
  {
    if ((*it).type() == ELEM_TYPE::VAR_TYPE)
      comp++;
  }
  return comp;
}

std::string
Rule::to_s(void)
{
  //rule category
  std::string rule_type = "";
  std::string internal_str = "";
  std::string external_str = "";

  switch (type)
  {
  case RULE_TYPE::SENTENCE:
    rule_type = Prefices::SEN;
    break;

  case RULE_TYPE::NOUN:
    rule_type = Prefices::CAT + Prefices::CLN + std::to_string(cat);

    break;
  default:
    std::cerr << "RULE TYPE : " << type << std::endl;
    throw "unknown rule type";
  }

  std::vector<std::string> buffer;

  if (internal.size() > 0)
  {
    InType::iterator it;
    //internal
    buffer.clear();
    it = internal.begin();
    while (it != internal.end())
    {
      buffer.push_back((*it).to_s());
      it++;
    }
    internal_str += string_join(buffer, " ");
  }

  if (external.size() > 0)
  {
    ExType::iterator it;
    //external
    it = external.begin();
    buffer.clear();
    while (it != external.end())
    {
      buffer.push_back((*it).to_s());
      it++;
    }
    external_str = string_join(buffer, " ");
  }

  return rule_type + " " + internal_str + " " + Prefices::ARW + " " + external_str;
}

void Rule::set_noun(Element &dcat, Element &dind, std::vector<Element> &dex)
{
  set_noun(Mean(dcat.get<Mean>()).get_cat_id(), dind, dex);
}

void Rule::set_noun(int dcat, Element &dind, std::vector<Element> &dex)
{
  type = RULE_TYPE::NOUN;
  cat = dcat;
  internal.clear();
  internal.push_back(dind);
  external = dex;
}

void Rule::set_sentence(std::vector<Element> &din, std::vector<Element> &dex)
{
  type = RULE_TYPE::SENTENCE;
  cat = 0; //feature
  internal = din;
  external = dex;
}

std::vector<std::vector<Element>>
Rule::moph(void)
{
  std::vector<Element>::iterator it;
  std::vector<std::vector<Element>> moph;
  std::vector<Element> buf;

  it = external.begin();
  for (; it != external.end(); it++)
  {
    switch ((*it).type())
    {
    case ELEM_TYPE::SYM_TYPE:
      buf.push_back(*it);
      break;
    case ELEM_TYPE::VAR_TYPE:
      if (buf.size() != 0)
        moph.push_back(buf);
      std::vector<Element> new_buf;
      buf = new_buf;
      break;
    }
  }
  if (buf.size() != 0)
    moph.push_back(buf);

  return moph;
}

std::string
Rule::string_join(const std::vector<std::string> &str_v, const std::string &delim)
{
  std::ostringstream os;
  std::copy(str_v.begin(), str_v.end(), std::ostream_iterator<std::string>(os, delim.c_str()));
  std::string str = os.str();
  str.erase(str.end() - delim.size(), str.end());
  return str;
}
