/*
 * File:   MSILMAgent.cpp
 * Author: hiroki
 *
 * Created on October 25, 2013, 4:08 PM
 */

#include "MSILMAgent.h"

bool MSILMAgent::SYM_FLAG = false;
bool MSILMAgent::UCSYM = false;
bool MSILMAgent::OMISSION_FLAG = false;

MSILMAgent::MSILMAgent()
{
	generation_index = 0;
}

MSILMAgent::~MSILMAgent()
{
}

MSILMAgent &
MSILMAgent::operator=(const MSILMAgent &dst)
{
	kb = dst.kb;
	generation_index = dst.generation_index;
	LOGGING_FLAG = dst.LOGGING_FLAG;
	SYM_FLAG = dst.SYM_FLAG;
	OMISSION_FLAG = dst.OMISSION_FLAG;
	last_selected_meaning = dst.last_selected_meaning;
	return *this;
}

MSILMAgent
MSILMAgent::make_child(void)
{
	MSILMAgent child;
	child.generation_index = generation_index + 1;
	return child;
}

MSILMAgent &
MSILMAgent::grow(void)
{

	kb.DIC_BLD = false;
	kb.word_dic.clear();
	kb.build_word_index();

	last_selected_meaning = std::vector<Rule>();

	return *this;
}

Rule MSILMAgent::cognition_say(std::vector<Rule> &internals)
{
	try
	{
		if (LOGGING_FLAG)
		{
			LogBox::push_log("START TO GUESS\n");
		}
		Rule return_internal = random_think_meaning(internals);
		last_selected_meaning.push_back(return_internal);
		if (LOGGING_FLAG)
		{
			LogBox::push_log("SELECTED MEANING : " + return_internal.to_s());
		}

		return kb.fabricate(return_internal);
	}
	catch (...)
	{
		LogBox::refresh_log();
		exit(1);
	}
}

std::vector<Rule>
MSILMAgent::cognition_say(std::vector<std::vector<Rule>> &internals)
{
	std::vector<Rule> rules;
	last_selected_meaning.clear();

	for (auto &el : internals)
	{
		rules.push_back(cognition_say(el));
	}

	return rules;
}

void MSILMAgent::sym_on(void)
{
	SYM_FLAG = true;
}

void MSILMAgent::sym_off(void)
{
	SYM_FLAG = false;
}

void MSILMAgent::ucsym_on(void)
{
	SYM_FLAG = true;
	UCSYM = true;
}

void MSILMAgent::omission_on(void)
{
	OMISSION_FLAG = true;
	KnowledgeBase::omission_on();
}

void MSILMAgent::omission_off(void)
{
	OMISSION_FLAG = false;
	KnowledgeBase::omission_off();
}

Rule MSILMAgent::random_think_meaning(std::vector<Rule> &internals)
{
	Rule result;

	if (LOGGING_FLAG)
	{
		LogBox::push_log("USING RAMDOM THINK");
		LogBox::push_log("PROSPECTIVE MEANINGS:\n" + tr_vector_Rule_to_string(internals));
	}
	if (internals.size() > 0)
	{
		int use_index = MT19937::irand(0, internals.size() - 1);
		result = internals[use_index];
		if (LOGGING_FLAG)
		{
			LogBox::push_log("RANDOM THINK\nRETURN MEANINGS:\n" + result.to_s());
		}
		return result;
	}
	else
	{
		std::cerr << "invalid input" << std::endl;
		std::cerr << "invalid input" << std::endl;
		exit(1);
	}
}

void MSILMAgent::cognition_hear(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meaning_lists, std::vector<Rule> &all_meanings)
{
	std::vector<Rule> internals = hear_think_meaning(utterances, meaning_lists, all_meanings);

	//Rule internal;
	if (internals.size() != 0)
	{
		//internal = internals[0];
	}
	else
	{
		//Exception
		std::cerr << "No meaning for utterance in Func:cognittion_say" << std::endl;
		std::cerr << "Exception" << std::endl;
		exit(1);
	}

	if (internals.size() == utterances.size())
	{
		for (int i = 0; i < utterances.size(); i++)
		{
			utterances[i].internal = internals[i].internal;
		}
	}
	else
	{
		std::cerr << "thinking error" << std::endl;
		exit(1);
	}

	last_selected_meaning = internals;

	if (LOGGING_FLAG)
	{
		LogBox::push_log("CHILD AQUIRED : " + tr_vector_Rule_to_string(utterances));
	}

	kb.send_box(utterances);
}

std::vector<Rule>
MSILMAgent::hear_think_meaning(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meaning_lists, std::vector<Rule> &all_meanings)
{
	//utterancesはexternalだけを使う
	std::vector<std::vector<Rule>> internal_lists;
	std::vector<Rule> internals;

	std::vector<Rule>::iterator mean_it;
	std::vector<Rule> kb1_pat, kb1_all;
	std::vector<std::vector<Rule>>::iterator meanings_it;

	if (LOGGING_FLAG)
	{
		LogBox::push_log("START TO GUESS\n");
		LogBox::push_log("PROSPECTIVE MEANINGS:\n" + tr_vector_Rule_to_string(meaning_lists) + "\n");
	}

	internal_lists = meaning_lists;

	//枝刈り
	int max_meanings = 1;
	meanings_it = meaning_lists.begin();
	for (; meanings_it != meaning_lists.end(); meanings_it++)
	{
		if (max_meanings < (*meanings_it).size())
		{
			max_meanings = (*meanings_it).size();
			break;
		}
	}

	//バッチ処理対応の処理だからちょっと複雑
	//バイアスによって意味の候補を減らす
	//trueなら完全一致、falseなら非完全一致
	if (!UCSYM)
	{
		if (SYM_FLAG && max_meanings > 1)
		{
			kb.DIC_BLD = false;
			kb.word_dic.clear();
			kb.build_word_index();

			mean_it = all_meanings.begin();
			for (; mean_it != all_meanings.end(); mean_it++)
			{
				kb1_pat = kb.grounded_rules((*mean_it));
				for (int i = 0; i < kb1_pat.size(); i++)
				{
					kb1_all.push_back(kb1_pat[i]);
				}
			}

			if (SYM_FLAG)
			{
				internal_lists = symmetry_bias_think(utterances, meaning_lists, kb1_all);
			}
		}
	}
	else
	{
		if (SYM_FLAG && max_meanings > 1)
		{
			kb.DIC_BLD = false;
			kb.word_dic.clear();
			kb.build_word_index();

			mean_it = all_meanings.begin();
			for (; mean_it != all_meanings.end(); mean_it++)
			{
				kb1_pat = kb.grounded_rules((*mean_it));
				for (int i = 0; i < kb1_pat.size(); i++)
				{
					kb1_all.push_back(kb1_pat[i]);
				}
			}
			if (SYM_FLAG)
			{
				//意味に選択順序を与える（using hamming and levenshtein distance）
				//選択順序が最も高い意味をinternal_listsにいれる
				internal_lists = ucsymmetry_bias_think(utterances, meaning_lists, kb1_all);
			}
		}
	}

	//候補の中からランダムに選択してinternalsに格納
	for (int i = 0; i < internal_lists.size(); i++)
	{
		if (internal_lists[i].size() > 1)
			internals.push_back(random_think_meaning(internal_lists[i]));
		else
			internals.push_back(internal_lists[i].front());
	}

	if (LOGGING_FLAG)
	{
		LogBox::push_log("SELECTED MEANING : " + tr_vector_Rule_to_string(internals));
	}

	return internals;
}

std::vector<std::vector<Rule>> &MSILMAgent::symmetry_bias_think(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meaning_lists, std::vector<Rule> &krules)
{
	if (LOGGING_FLAG)
	{
		LogBox::push_log("USING SYMMETRY BIAS THINK");
		LogBox::push_log("PROSPECTIVE MEANINGS:\n" + tr_vector_Rule_to_string(meaning_lists));
	}
	std::vector<Rule>::iterator meanings_it1, meanings_it2, refer_it;
	std::vector<std::vector<Rule>>::iterator meaningss_it;
	std::vector<Rule> meaning_stack;

	//window内のbias
	for (int i = 0; i < utterances.size(); i++)
	{
		for (int j = i + 1; j < utterances.size(); j++)
		{
			if (utterances[i].external == utterances[j].external)
			{
				meaning_stack.clear();
				//meaning_lists[i],meaning_lists[j]
				meanings_it1 = meaning_lists[i].begin();
				for (; meanings_it1 != meaning_lists[i].end(); meanings_it1++)
				{
					meanings_it2 = meaning_lists[j].begin();
					for (; meanings_it2 != meaning_lists[j].end(); meanings_it2++)
					{
						if ((*meanings_it1).internal == (*meanings_it2).internal)
							meaning_stack.push_back(*meanings_it1);
					}
				}

				if (meaning_stack.size() != 0)
				{
					meaning_lists[i] = meaning_stack;
					meaning_lists[j] = meaning_stack;
				}
			}
		}
	}

	//言語知識からのbias
	int ref_count = 0;
	refer_it = krules.begin();
	for (; refer_it != krules.end(); refer_it++)
	{
		for (int i = 0; i < utterances.size(); i++)
		{
			bool exist_f = false;
			if ((*refer_it).external == utterances[i].external && meaning_lists[i].size() != 1)
			{
				bool find = false;
				if (!exist_f)
				{
					meanings_it1 = meaning_lists[i].begin();
					for (; meanings_it1 != meaning_lists[i].end(); meanings_it1++)
					{
						if ((*meanings_it1).internal == (*refer_it).internal)
							find = true;
					}
					if (find)
					{
						meaning_lists[i].clear();
						meaning_lists[i].resize(1);
						meaning_lists[i].front() = krules[0]; //文ならなんでもいい初期化
						meaning_lists[i].front().internal = (*refer_it).internal;
						meaning_lists[i].front().external.clear();
						exist_f = true;
					}
				}
			}
		}
		ref_count++;
	}

	if (LOGGING_FLAG)
	{
		LogBox::push_log("SYMMETRY BIAS THINK\nRETURN MEANINGS:\n" + tr_vector_Rule_to_string(meaning_lists));
	}
	return meaning_lists;
}

std::vector<std::vector<Rule>> &MSILMAgent::ucsymmetry_bias_think(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meaning_lists, std::vector<Rule> &krules)
{
	if (LOGGING_FLAG)
	{
		LogBox::push_log("USING SYMMETRY BIAS THINK");
		LogBox::push_log("PROSPECTIVE MEANINGS:\n" + tr_vector_Rule_to_string(meaning_lists));
	}
	if (utterances.size() != meaning_lists.size())
	{
		std::cerr << "Size Error in Imcomplete Symmetry Bias Think" << std::endl;
		exit(1);
	}
	std::vector<std::size_t> index_list(utterances.size());
	std::iota(std::begin(index_list), std::end(index_list), 0);

	std::vector<std::vector<std::pair<Rule, Rule>>> all_ham_candidates(utterances.size());
	std::for_each(std::begin(index_list), std::end(index_list), [&all_ham_candidates, &index_list, &utterances, &meaning_lists, &krules](int pos_ut) {
		double min_lev(1.0);
		std::vector<int> min_index;
		std::vector<Rule> min_rules;
		std::for_each(std::begin(index_list), std::end(index_list), [&min_lev, &utterances, &pos_ut, &min_index](std::size_t idx_ut) {
			if (pos_ut != idx_ut)
			{
				double lev = Distance::levenstein2(utterances[pos_ut].external, utterances[idx_ut].external);
				if (lev < min_lev)
				{
					min_lev = lev;
					min_index.clear();
					min_index.push_back(idx_ut);
				}
				else if (lev == min_lev)
				{
					min_index.push_back(idx_ut);
				}
			}
		});
		std::for_each(std::begin(krules), std::end(krules), [&min_lev, &utterances, &pos_ut, &min_index, &min_rules](Rule &r) {
			double lev = Distance::levenstein2(utterances[pos_ut].external, r.external);
			if (lev < min_lev)
			{
				min_lev = lev;
				min_index.clear();
				min_rules.clear();
				min_rules.push_back(r);
			}
			else if (lev == min_lev)
			{
				min_rules.push_back(r);
			}
		});
		std::for_each(std::begin(meaning_lists[pos_ut]), std::end(meaning_lists[pos_ut]), [&min_index, &meaning_lists, &all_ham_candidates, &min_rules, &pos_ut](Rule &r1) {
			std::for_each(std::begin(min_index), std::end(min_index), [&meaning_lists, &all_ham_candidates, &pos_ut, &r1](int idx2) {
				std::for_each(std::begin(meaning_lists[idx2]), std::end(meaning_lists[idx2]), [&all_ham_candidates, &pos_ut, &r1](Rule &r2) {
					all_ham_candidates[pos_ut].push_back(std::pair<Rule, Rule>(r1, r2));
				});
			});
			std::for_each(std::begin(min_rules), std::end(min_rules), [&all_ham_candidates, &pos_ut, &r1](Rule &min_rule) {
				all_ham_candidates[pos_ut].push_back(std::pair<Rule, Rule>(r1, min_rule));
			});
		});
	});
	std::for_each(std::begin(index_list), std::end(index_list), [&meaning_lists, &all_ham_candidates](int idx) {
		std::vector<Rule> min_rules;
		double min_ham = 1.0;
		std::for_each(std::begin(all_ham_candidates[idx]), std::end(all_ham_candidates[idx]), [&min_rules, &min_ham](std::pair<Rule, Rule> &p) {
			double ham = Distance::hamming(p.first.internal, p.second.internal);
			if (ham < min_ham)
			{
				min_ham = ham;
				min_rules.clear();
				min_rules.push_back(p.first);
			}
			else if (ham == min_ham)
			{
				min_rules.push_back(p.first);
			}
		});
		if (min_rules.size() != 0)
			meaning_lists[idx] = min_rules;
	});
	return meaning_lists;
}

std::vector<Rule> MSILMAgent::return_last_selected_meaning()
{
	return last_selected_meaning;
}

//utility

std::string
MSILMAgent::tr_vector_Rule_to_string(std::vector<Rule> vector)
{
	if (vector.size() != 0)
	{
		std::string res = "(";
		std::vector<Rule>::iterator rule_it = vector.begin();

		res = res + (*rule_it).to_s();
		rule_it++;
		for (; rule_it != vector.end(); rule_it++)
		{
			res = res + "," + (*rule_it).to_s();
		}
		return (res + ")");
	}
	else
	{
		return "no rules";
	}
}

std::string
MSILMAgent::tr_vector_Rule_to_string(std::vector<std::vector<Rule>> vector)
{
	if (vector.size() != 0)
	{
		std::string res = "(";
		std::vector<std::vector<Rule>>::iterator rule_it = vector.begin();

		res = res + tr_vector_Rule_to_string(*rule_it);
		rule_it++;
		for (; rule_it != vector.end(); rule_it++)
		{
			res = res + "," + tr_vector_Rule_to_string(*rule_it);
		}
		return (res + ")");
	}
	else
	{
		return "no rules";
	}
}

std::vector<std::vector<Rule>>
MSILMAgent::tr_vector_Rule_to_double_vector(std::vector<Rule> v)
{
	std::vector<std::vector<Rule>> res;
	for (auto &el : v)
	{
		res.push_back(std::vector<Rule>());
		res.back().push_back(el);
	}
	return res;
}