/*
 * Rule.h
 *
 *  Created on: 2012/11/18
 *      Author: Hiroki Sudo
 */

#ifndef RULE_H_
#define RULE_H_

#include <vector>
#include <string>
#include <regex>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <variant>

//#include <boost/range/algorithm.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/serialization/serialization.hpp>
//#include <boost/serialization/nvp.hpp>
//#include <boost/serialization/vector.hpp>

#include "Distance.hpp"
#include "Element.h"
#include "IndexFactory.h"

namespace RULE_TYPE
{
const unsigned int SENTENCE = 0;
const unsigned int NOUN = 1;
}

class RuleTypeDef
{
public:
  typedef std::vector<Element> InType;
  typedef std::vector<Element> ExType;
};

/*!
 * hoge(foo,bar)->xhkdjeoek
 * のようなルールを表現するクラス
 */
class Rule : public RuleTypeDef
{
public:
  //member
  std::vector<Element> internal; //内部言語列
  std::vector<Element> external; //外部言語列

  int type; //文規則か、単語規則かのデータ
  int cat;  //単語規則の場合のカテゴリナンバー

  /*!
     * 文字列表現に変換する際に使用する辞書データ
     */
  static Dictionary dictionary;

  //constructor
  Rule();
  Rule(char* cstr);

  /*!
     * デバッグ用ではあるが、文字列表現のRuleを使ってインスタンスを生成する
     */
  Rule(std::string str);

  //destructor
  virtual ~Rule();

  //!operator
  bool
  operator==(const Rule &dst) const;
  bool
  operator!=(Rule &dst) const;
  Rule &
  operator=(const Rule &dst);

  //method
  bool
  is_sentence(void) const;
  bool
  is_noun(void) const;
  int composition(void) const;

  std::string
  to_s(void);
  void
  set_noun(int dcat, Element &dind, ExType &dex);
  void
  set_sentence(InType &din, ExType &dex);

private:
  std::string
  string_join(const std::vector<std::string> &str_v, const std::string &delim);
};

#endif /* RULE_H_ */
