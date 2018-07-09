/* 
 * File:   MSILMAgent.h
 * Author: hiroki
 *
 * Created on October 25, 2013, 4:08 PM
 */

#ifndef MSILMAGENT_H
#define MSILMAGENT_H
#include "KirbyAgent.h"
#include "MT19937.h"

class MSILMAgent : public KirbyAgent
{
public:
  static bool SYM_FLAG;
  static bool UCSYM;
  static bool OMISSION_FLAG;
  MSILMAgent make_child(void);
  MSILMAgent &grow(void);
  MSILMAgent &operator=(const MSILMAgent &dst);
  Rule cognition_say(std::vector<Rule> &internals);
  std::vector<Rule> cognition_say(std::vector<std::vector<Rule>> &internals);
  void cognition_hear(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meanings_list, std::vector<Rule> &all_meanings);
  std::vector<Rule> return_last_selected_meaning();
  MSILMAgent();
  virtual ~MSILMAgent();
  static void sym_on(void);
  static void sym_off(void);
  static void ucsym_on(void);
  static void omission_on(void);
  static void omission_off(void);

private:
  std::vector<Rule> last_selected_meaning;
  std::vector<Rule> hear_think_meaning(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meaning_lists, std::vector<Rule> &all_meanings);
  Rule random_think_meaning(std::vector<Rule> &internals);
  std::vector<std::vector<Rule>> &symmetry_bias_think(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meaningss, std::vector<Rule> &krules);
  std::vector<std::vector<Rule>> &ucsymmetry_bias_think(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meaningss, std::vector<Rule> &krules);
  //utility
  std::string tr_vector_Rule_to_string(std::vector<Rule> vector);
  std::string tr_vector_Rule_to_string(std::vector<std::vector<Rule>> vector);
  std::vector<std::vector<Rule>> tr_vector_Rule_to_double_vector(std::vector<Rule>);
};

class Random
{
public:
  // コンストラクタ
  Random()
  {
  }

  // 関数オブジェクト
  unsigned int operator()(unsigned int max)
  {
    return static_cast<unsigned int>(MT19937::irand() % max);
  }
};

#endif /* MSILMAGENT_H */
