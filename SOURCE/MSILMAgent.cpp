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
MSILMAgent::grow(std::vector<Rule> meanings)
{

	kb.DIC_BLD = false;
	kb.word_dic.clear();
	kb.build_word_index();

	std::vector<Rule> pseudo_meaning;
	last_selected_meaning = pseudo_meaning;

	return *this;
}

Rule MSILMAgent::cognittion_say(std::vector<Rule> &internals)
{
	try
	{
		Rule return_internal = say_think_meaning(internals);
		last_selected_meaning.push_back(return_internal);

		return kb.fabricate(return_internal);
	}
	catch (...)
	{
		LogBox::refresh_log();
		throw;
	}
}

std::vector<Rule>
MSILMAgent::cognittion_say(std::vector<std::vector<Rule>> &internals)
{
	std::vector<Rule> rules;
	last_selected_meaning.clear();

	for (auto &el : internals)
	{
		rules.push_back(cognittion_say(el));
	}

	return rules;
}

Rule MSILMAgent::say_think_meaning(std::vector<Rule> &internals)
{
	if (LOGGING_FLAG)
	{
		LogBox::push_log("START TO GUESS\n");
	}

	Rule return_rule = random_think_meaning(internals);

	if (LOGGING_FLAG)
	{
		LogBox::push_log("SELECTED MEANING : " + return_rule.to_s());
	}

	return return_rule;
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
		throw "invalid input";
	}
}

void MSILMAgent::cognittion_hear(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meaning_lists, std::vector<Rule> &all_meanings)
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
		throw "Exception";
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
		throw "thinking error";
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
				symmetry_bias_think(utterances, meaning_lists, kb1_all, internal_lists);
			}
		}
	}
	else
	{
		if (SYM_FLAG && max_meanings > 1)
		{
			//Window:mC2:2
			std::vector<std::vector<std::vector<Rule>>> meaning_pair_lists;
			std::vector<std::vector<double>> meaning_distancess;
			//Window:2
			std::vector<std::vector<Rule>> utterance_pairs;

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
				ucsymmetry_bias_inference(utterances, meaning_lists, kb1_all, utterance_pairs, meaning_pair_lists, meaning_distancess);
				//選択順序が最も高い意味をinternal_listsにいれる
				internal_lists = decide_likelihood(utterances, utterance_pairs, meaning_pair_lists);
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

void MSILMAgent::symmetry_bias_think(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meaning_lists, std::vector<Rule> &reference, std::vector<std::vector<Rule>> &return_rules)
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
	refer_it = reference.begin();
	for (; refer_it != reference.end(); refer_it++)
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
						meaning_lists[i].front() = reference[0]; //文ならなんでもいい初期化
						meaning_lists[i].front().internal = (*refer_it).internal;
						meaning_lists[i].front().external.clear();
						exist_f = true;
					}
				}
			}
		}
		ref_count++;
	}

	return_rules = meaning_lists;

	if (LOGGING_FLAG)
	{
		LogBox::push_log("SYMMETRY BIAS THINK\nRETURN MEANINGS:\n" + tr_vector_Rule_to_string(return_rules));
	}
}

void MSILMAgent::ucsymmetry_bias_inference(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &meaning_lists, std::vector<Rule> &reference, std::vector<std::vector<Rule>> &term_pairs, std::vector<std::vector<std::vector<Rule>>> &meaning_pair_lists, std::vector<std::vector<double>> &meaning_distance_lists)
{
	if (LOGGING_FLAG)
	{
		LogBox::push_log("USING SYMMETRY BIAS THINK");
		LogBox::push_log("PROSPECTIVE MEANINGS:\n" + tr_vector_Rule_to_string(meaning_lists));
	}

	bool double_check;
	int index, index2;
	std::vector<int> index_buf;
	std::vector<int>::iterator index_buf_it;
	std::vector<Rule> meanings, temp;
	std::vector<Rule>::iterator refer_it;
	double min_dist = 10, lev;
	Rule base_rule, min_dist_rule;
	term_pairs.clear();
	while (index_buf.size() != utterances.size())
	{
		//同時チェックに対応するための仕組み
		for (int i = 0; i < utterances.size(); i++)
		{
			index_buf_it = std::find(index_buf.begin(), index_buf.end(), i);
			if (index_buf_it == index_buf.end())
			{
				index = i;
				index_buf.push_back(index);
				break;
			}
		}
		//調べる終端記号列（発話）の設定
		base_rule = utterances[index];
		//調べる終端記号列（発話）に対応する候補の初期設定
		meanings = meaning_lists[index];

		//調べていない発話，既知の発話の中から調べる対象と一番似た発話を探す
		//調べる対象と一番近い発話を調べてないものから検索
		double_check = false;
		for (int i = 0; i < utterances.size(); i++)
		{
			index_buf_it = std::find(index_buf.begin(), index_buf.end(), i);
			if (index_buf_it == index_buf.end())
			{
				lev = Distance::levenstein(base_rule.external, utterances[i].external);
				if (min_dist > lev)
				{
					double_check = true;
					min_dist = lev;
					min_dist_rule = utterances[i];
					meanings = meaning_lists[i];
					index2 = i;
				}
			}
		}
		//調べる対象と一番近い発話を既知の発話から検索
		refer_it = reference.begin();
		for (; refer_it != reference.end(); refer_it++)
		{
			lev = Distance::levenstein(base_rule.external, (*refer_it).external);
			if (min_dist > lev)
			{
				double_check = false;
				min_dist = lev;
				min_dist_rule = (*refer_it);
				temp.clear();
				temp.push_back(*refer_it);
				meanings = temp;
			}
		}

		//発話のpair(vector)をつくる
		//受け取った発話内であれば，2つチェックしたこととする．
		temp = std::vector<Rule>();
		temp.push_back(base_rule);
		if (double_check)
		{
			temp.push_back(min_dist_rule);
			index_buf.push_back(index2);
		}
		std::vector<Rule> empty;
		term_pairs.push_back(empty);
		term_pairs.back() = temp;

		//meaning_pairを作る
		//meaning_lists[index]とmeaningsで行う．
		std::vector<std::vector<Rule>> meaning_pairs, meaning_pairs_buf;
		meaning_pair_lists.push_back(meaning_pairs);
		//meaning_pairs_bufに入っている2つの意味のハミング距離を計算
		std::vector<double> distances, distances_buf;
		meaning_distance_lists.push_back(distances);
		for (int i = 0; i < meaning_lists[index].size(); i++)
		{
			for (int j = 0; j < meanings.size(); j++)
			{
				std::vector<Rule> meaning_pair;
				meaning_pairs_buf.push_back(meaning_pair);
				//pair作成
				temp.clear();
				temp.push_back(meaning_lists[index][i]);
				temp.push_back(meanings[j]);
				meaning_pairs_buf.back() = temp;

				distances_buf.push_back(Distance::hamming(meaning_lists[index][i].internal, meanings[j].internal));
			}
		}
		//並び替え(仮でバブルソート)
		double ham_dist;			 //交換用
		std::vector<Rule> tmp_rules; //交換用
		for (int i = 0; i < (meaning_pairs_buf.size() - 1); i++)
		{
			for (int j = 0; j < (meaning_pairs_buf.size() - i - 1); j++)
			{
				ham_dist = distances_buf[j];
				if (ham_dist > distances_buf[j + 1])
				{
					//exchanging distance
					distances_buf[j] = distances_buf[j + 1];
					distances_buf[j + 1] = ham_dist;
					//exchanging meaning
					tmp_rules = meaning_pairs_buf[j];
					meaning_pairs_buf[j] = meaning_pairs_buf[j + 1];
					meaning_pairs_buf[j + 1] = tmp_rules;
				}
			}
		}

		meaning_distance_lists.back() = distances_buf;
		meaning_pair_lists.back() = meaning_pairs_buf;
	}
}

std::vector<std::vector<Rule>>
MSILMAgent::decide_likelihood(std::vector<Rule> &utterances, std::vector<std::vector<Rule>> &utterance_pairs, std::vector<std::vector<std::vector<Rule>>> &meaning_pair_list)
{
	std::vector<Rule> internals;
	std::vector<Rule>::iterator it;
	internals.resize(utterances.size());
	if (LOGGING_FLAG)
	{
		LogBox::push_log("DECIDE MAXIMUM LIKELIHOOD MEANNING\nUTTERANCES:\n" + tr_vector_Rule_to_string(utterance_pairs));
	}

	for (int index = 0; index < utterances.size(); index++)
	{
		int pos = 0;
		for (; pos < utterance_pairs.size(); pos++)
		{
			it = std::find(utterance_pairs[pos].begin(), utterance_pairs[pos].end(), utterances[index]);
			if (it != utterance_pairs[pos].end())
			{
				break;
			}
		}
		internals[index] = meaning_pair_list[pos].front()[it - utterance_pairs[pos].begin()];
		//future work
		//erase utterance_pair, meaning_pair
	}

	return tr_vector_Rule_to_double_vector(internals);
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