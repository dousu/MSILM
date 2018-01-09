/* 
 * File:   MSILMAgent.h
 * Author: hiroki
 *
 * Created on October 25, 2013, 4:08 PM
 */

#ifndef MSILMAGENT_H
#define	MSILMAGENT_H
#include "KirbyAgent.h"
#include "MT19937.h"

class MSILMAgent : public KirbyAgent{
public:
    
    MSILMAgent make_child(void);
    
    MSILMAgent& grow(std::vector<Rule>);
    
    MSILMAgent& operator=(const MSILMAgent& dst);
    
    Rule cognittion_say(std::vector<Rule>& internals);
	std::vector<Rule> cognittion_say(std::vector<std::vector<Rule> >& internals);
//    void cognittion_hear(Rule& term,std::vector<Rule>& meanings, std::vector<Rule>& all_meanings);
    void cognittion_hear(std::vector<Rule>& utterances,std::vector<std::vector<Rule> >& meanings_list, std::vector<Rule>& all_meanings);
    
    Rule say_think_meaning(std::vector<Rule>& internals);
    std::vector<Rule> hear_think_meaning(std::vector<Rule>& utterances,std::vector<std::vector<Rule> >& meaning_lists,std::vector<Rule>& all_meanings);
    Rule random_think_meaning(std::vector<Rule>& internals);
    void symmetry_bias_think(std::vector<Rule>& terms,std::vector<std::vector<Rule> >& meaningss,std::vector<Rule>& reference, std::vector<std::vector<Rule> >& return_rules);
    void ucsymmetry_bias_inference(std::vector<Rule>& terms,std::vector<std::vector<Rule> >& meaningss,std::vector<Rule>& reference, std::vector<std::vector<Rule> >& term_pairs,std::vector<std::vector<std::vector<Rule> > >& meaning_pair_orders, std::vector<std::vector<double> >& meaning_distancess);
    std::vector<std::vector<Rule> > decide_likelihood(std::vector<Rule>& terms, std::vector<std::vector<Rule> >& term_pairs, std::vector<std::vector<std::vector<Rule> > >& meaning_pair_orders);
    static bool SYM_FLAG;
	static bool UCSYM;
    static bool OMISSION_FLAG;
    
	std::vector<Rule> return_last_selected_meaning();
    
    //utility
    std::string tr_vector_Rule_to_string(std::vector<Rule> vector);
    std::string tr_vector_Rule_to_string(std::vector<std::vector<Rule> > vector);
	std::vector<std::vector<Rule> > tr_vector_Rule_to_double_vector(std::vector<Rule>);
    
    MSILMAgent();
    virtual ~MSILMAgent();
    static void sym_on(void);
	static void sym_off(void);
	static void ucsym_on(void);
    static void omission_on(void);
    static void omission_off(void);
    
private:
	std::vector<Rule> last_selected_meaning;
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
		return static_cast<unsigned int>( MT19937::irand() % max );
	}
};

#endif	/* MSILMAGENT_H */

