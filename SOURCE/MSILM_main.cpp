/*
 * File:   MSILM_main.cpp
 * Author: hiroki
 *
 * Created on October 28, 2013, 6:12 PM
 */

#include "MSILM_main.h"

/*template<typename _OFS>
void save_agent(
	_OFS& ofs,
	MSILMParameters& param,
	unsigned long long int& icount,
	unsigned long long int& rcount,
	Dictionary& dic,
	std::vector<Rule>& meanings,
	MSILMAgent& agent
) {
	ofs << boost::serialization::make_nvp("CTL", param.CONTROLS);
	ofs << boost::serialization::make_nvp("UUF", param.UNIQUE_UTTERANCE);
	ofs << boost::serialization::make_nvp("RS", param.RANDOM_SEED);
	ofs << boost::serialization::make_nvp("UC", param.UC_SYMMETRY);
	ofs << boost::serialization::make_nvp("IRC", icount);
	ofs << boost::serialization::make_nvp("RRC", rcount);
	ofs << boost::serialization::make_nvp("MS", meanings);
	ofs << boost::serialization::make_nvp("DC", dic);
	ofs << boost::serialization::make_nvp("MS", agent);
}

template<typename _IFS>
void resume_agent(
	_IFS& ifs,
	MSILMParameters& param,
	unsigned long long int& icount,
	unsigned long long int& rcount,
	Dictionary& dic,
	std::vector<Rule>& meanings,
	MSILMAgent& agent
) {
	ifs >> boost::serialization::make_nvp("CTL", param.CONTROLS);
	ifs >> boost::serialization::make_nvp("UUF", param.UNIQUE_UTTERANCE);
	ifs >> boost::serialization::make_nvp("RS", param.RANDOM_SEED);
	ifs >> boost::serialization::make_nvp("UC", param.UC_SYMMETRY);
	ifs >> boost::serialization::make_nvp("IRC", icount);
	ifs >> boost::serialization::make_nvp("RRC", rcount);
	ifs >> boost::serialization::make_nvp("MS", meanings);
	ifs >> boost::serialization::make_nvp("DC", dic);
	ifs >> boost::serialization::make_nvp("MS", agent);
}*/

void construct_meanings(std::vector<Rule> &meanings)
{
	int VERB_INDEX_BEGIN = 0;
	int VERB_INDEX_END = 4;
	int NOUN_INDEX_BEGIN = 5;
	int NOUN_INDEX_END = 9;

	//construct means
	for (int i = VERB_INDEX_BEGIN; i <= VERB_INDEX_END; i++)
	{
		Element verb;
		verb.set_ind(i);

		for (int j = NOUN_INDEX_BEGIN; j <= NOUN_INDEX_END; j++)
		{
			Element ind1;
			ind1.set_ind(j);
			for (int k = NOUN_INDEX_BEGIN; k <= NOUN_INDEX_END; k++)
			{
				if (j != k)
				{
					Element ind2;
					std::vector<Element> internal, external;
					Rule mean;

					ind2.set_ind(k);

					internal.push_back(verb);
					internal.push_back(ind1);
					internal.push_back(ind2);

					mean.set_sentence(internal, external);
					meanings.push_back(mean);
				}
			}
		}
	}
}

void construct_individuals(std::vector<Element> &inds, Dictionary &dic)
{
	Dictionary::DictionaryType::iterator dic_it;
	dic_it = dic.individual.begin();
	for (; dic_it != dic.individual.end(); dic_it++)
	{
		Element elem;
		elem.set_ind((*dic_it).first);
		inds.push_back(elem);
	}
}

std::vector<std::vector<double>> analyze(std::vector<Rule> &meanings,
										 MSILMAgent &agent1, MSILMAgent &agent2)
{

	std::vector<double> unit_result, dist_result;

	std::vector<std::vector<double>> result;
	unit_result.clear();
	dist_result.clear();

	//Agent単体の分析
	unit_analyze(unit_result, meanings, agent1);

	//Agent相互の分析
	calculate_language_distance(dist_result, meanings,
								agent1, agent2);

	//リターンバケットに結果をつめて返す
	result.push_back(unit_result);
	result.push_back(dist_result);

	return result;
}

void unit_analyze(std::vector<double> &result_matrix,
				  std::vector<Rule> &meanings, MSILMAgent &agent)
{
	int index = 0, max_index;
	int GEN = 0, EXP = 1, SRN = 2, WRN = 3;

	//Item1 :generation number
	result_matrix.push_back(agent.generation_index);

	//Item2 :expressiveness
	result_matrix.push_back(expression(meanings, agent));

	//Item3 :sentence rule number
	result_matrix.push_back(agent.kb.sentenceDB.size());

	//Item4 :Word rule number
	result_matrix.push_back(agent.kb.wordDB.size());
}

double expression(std::vector<Rule> &meanings, MSILMAgent &agent)
{
	std::vector<Rule>::iterator mean_it;
	int counter = 0;
	mean_it = meanings.begin();
	while (mean_it != meanings.end())
	{
		if (agent.utterable(*mean_it))
			counter++;
		mean_it++;
	}

	return (static_cast<double>(counter)) / (static_cast<double>(meanings.size())) * 100.0;
}

void calculate_language_distance(
	std::vector<double> &lev_matrix,
	std::vector<Rule> &meanings,
	MSILMAgent &agent1, MSILMAgent &agent2)
{

	double word_length;

	//言語間距離を計算
	lev_matrix.push_back(calculate_distance(meanings, agent1.kb, agent2.kb, word_length));
}

void analyze_and_output(MSILMParameters &param, std::vector<Rule> meaning_space,
						MSILMAgent &agent1, MSILMAgent &agent2, int index)
{
	//	std::vector<NetWorld>::iterator n_it;
	std::vector<std::vector<double>> res;
	std::string index_str, file, file_postfix;
	//int index;

	//index = agent1.generation_index;
	index_str = std::to_string(index);
	file = param.FILE_PREFIX + param.DATE_STR + "_" + std::to_string(param.RANDOM_SEED) + "_" + index_str + ".rst";

	res = analyze(meaning_space, agent1, agent2);

	//basic analyze
	std::ofstream ofs((param.BASE_PATH + file).c_str(),
					  std::ios::app | std::ios::out);

	ofs << "#RESULT" << std::endl;

	//unit
	ofs << "BASIC=" << tr_vector_double_to_string(res[0]) << std::endl;

	//distance
	ofs << "DIST =" << tr_vector_double_to_string(res[1]) << std::endl;
}

void accuracy_meaning_output(MSILMParameters param, std::string file, std::vector<std::vector<int>> data)
{
	{
		std::string path;
		path = param.BASE_PATH + file;
		std::ofstream ofs(path.c_str());

		std::vector<std::vector<int>>::iterator data_it;
		std::vector<int>::iterator inside_it;
		data_it = data.begin();
		for (; data_it != data.end(); data_it++)
		{
			inside_it = (*data_it).begin();
			if (inside_it != (*data_it).end())
			{
				ofs << *inside_it;
				inside_it++;
				for (; inside_it != (*data_it).end(); inside_it++)
				{
					ofs << " " << *inside_it;
				}
			}
			ofs << std::endl;
		}
		ofs.close();
	}
}

double calculate_distance(std::vector<Rule> &meanings,
						  KnowledgeBase &kb1, KnowledgeBase &kb2, double &word_length)
{

	//from kb1 to kb2

	std::vector<Rule>::iterator mean_it, kb1_pat_it, kb2_pat_it, kb1_all_it, kb2_all_it, target_it;
	std::vector<Rule> kb1_pat, kb2_pat, kb1_all, kb2_all, target_rules, temp;
	double ham, min_ham = 1000, lev, min_lev = 2, lev_sum = 0, work_lev = 2, length_cnt = 0;
	int ham_zero_cnt = 0;

	word_length = 0;

	mean_it = meanings.begin();

	for (; mean_it != meanings.end(); mean_it++)
	{
		kb1_pat = kb1.grounded_rules(*mean_it);
		kb2_pat = kb2.grounded_rules(*mean_it);
		if (kb1_pat.size() != 0)
		{
			kb1_pat_it = kb1_pat.begin();
			for (; kb1_pat_it != kb1_pat.end(); kb1_pat_it++)
			{
				kb1_all.push_back(*kb1_pat_it);
				word_length += ((*kb1_pat_it).external.size());
				length_cnt++;
			}
		}
		else
		{
			kb1_all.push_back(kb1.fabricate_for_complementing(*mean_it));
		}
		if (kb2_pat.size() != 0)
		{
			kb2_pat_it = kb2_pat.begin();
			for (; kb2_pat_it != kb2_pat.end(); kb2_pat_it++)
			{
				kb2_all.push_back(*kb2_pat_it);
			}
		}
		else
		{
			kb2_all.push_back(kb2.fabricate_for_complementing(*mean_it));
		}
	}
	word_length = (word_length / length_cnt);
	kb1_all_it = kb1_all.begin();
	for (; kb1_all_it != kb1_all.end(); kb1_all_it++)
	{
		min_ham = 1000;
		kb2_all_it = kb2_all.begin();
		for (; kb2_all_it != kb2_all.end(); kb2_all_it++)
		{
			ham = Distance::hamming((*kb1_all_it).internal, (*kb2_all_it).internal);
			if (ham < min_ham)
			{
				min_ham = ham;
				target_rules = temp;
				target_rules.push_back(*kb2_all_it);
			}
			else if (ham == min_ham)
			{
				target_rules.push_back(*kb2_all_it);
			}
		}

		min_lev = 1;
		target_it = target_rules.begin();
		for (; target_it != target_rules.end(); target_it++)
		{
			lev = Distance::levenstein((*kb1_all_it).external, (*target_it).external);
			if (lev < min_lev)
			{
				min_lev = lev;
			}
		}
		lev_sum += min_lev;
	}

	return lev_sum / (static_cast<double>(kb1_all.size()));
}

void calculate_average_word_length(std::vector<Rule> &meanings, KnowledgeBase &kb1, double &word_length)
{
	std::vector<Rule>::iterator mean_it, kb1_pat_it;
	std::vector<Rule> kb1_pat;
	double length_cnt = 0;

	word_length = 0;

	mean_it = meanings.begin();

	for (; mean_it != meanings.end(); mean_it++)
	{
		kb1_pat = kb1.grounded_rules(*mean_it);
		if (kb1_pat.size() != 0)
		{
			kb1_pat_it = kb1_pat.begin();
			for (; kb1_pat_it != kb1_pat.end(); kb1_pat_it++)
			{
				word_length += ((*kb1_pat_it).external.size());
				length_cnt++;
			}
		}
	}
	word_length = (word_length / length_cnt);
}

std::string
tr_vector_double_to_string(std::vector<double> vector)
{
	std::string res = "(";
	std::stringstream ss;
	std::string work;
	std::vector<double>::iterator double_it = vector.begin();
	ss.clear();
	ss << (*double_it);
	ss >> work;
	res = res + work;
	double_it++;
	for (; double_it != vector.end(); double_it++)
	{
		ss.clear();
		ss << (*double_it);
		ss >> work;
		res = res + "," + work;
	}
	return (res + ")");
}

void logging_off(MSILMParameters &param)
{
	param.LOGGING = false;
	MSILMAgent::logging_off();
}

void logging_on(MSILMParameters &param)
{
	param.LOGGING = true;
	MSILMAgent::logging_on();
}

std::vector<int>
choice_meanings(std::vector<Rule> meanings, MSILMParameters param)
{
	std::vector<int> chosen_meaning_indexs;
	int index;
	bool flag = false;
	for (int i = 0; i < param.MULTIPLE_MEANINGS; i++)
	{
		if (param.MULTIPLE_MEANINGS == meanings.size())
		{
			index = i;
		}
		else
		{
			index = MT19937::irand(0, meanings.size() - 1);
		}
		flag = (std::find(chosen_meaning_indexs.begin(), chosen_meaning_indexs.end(), index) != chosen_meaning_indexs.end());
		while (flag)
		{
			index = MT19937::irand(0, meanings.size() - 1);
			flag = (std::find(chosen_meaning_indexs.begin(), chosen_meaning_indexs.end(), index) != chosen_meaning_indexs.end());
		}
		chosen_meaning_indexs.push_back(index);
	}
	return chosen_meaning_indexs;
}

void cognition_task_init(std::vector<int> &source, MSILMParameters &param)
{

	source.clear();
	for (int i = 0; i < param.TERMS; i++)
	{
		source.push_back(1);
	}
	for (int i = 0; i < param.UTTERANCES - param.TERMS; i++)
	{
		source.push_back(0);
	}
	//Random r;
	//std::random_shuffle(source.begin(), source.end(), r);
	std::shuffle(source.begin(), source.end(), MT19937::igen);
}

int main(int argc, char *argv[])
{
	typedef std::vector<MSILMAgent> Generation;

	/*
	 * ほぼ静的クラスだが、シリアライズのため（Dictionary）と、
	 * デストラクタ実行のため（LogBox）にインスタンスを作成
	 * */
	Dictionary dic;
	LogBox log; //for destructor
	time_t start, now;

	limit_time = 20 * 60; //second

	MSILMParameters param;
	//boost::progress_display *show_progress = 0;

	Generation all_generations;
	std::vector<Rule> meaning_space;
	std::vector<Element> individuals;
	MSILMAgent parent_agent, child_agent;

	int utterance_counter = 0;

	int use_meaning_index = 0;
	int generation_counter = 1;
	int Base_Counter = 0;
	Rule utter, use_meaning;
	std::vector<int> use_meaning_indexs;
	std::vector<Rule> use_meanings, history_utterance;
	std::vector<std::vector<Rule>> history_meanings;
	std::vector<int> cognition_task_flag;

	std::vector<std::vector<int>> cognittion_correct_data;

	/**************************************************
	 *
	 * OPTION PROCESSING
	 *
	 **************************************************/
	ProgramOption opt;
	opt.add_option()("help", "Description")
		//experiment parameters
		/*ランダムシード*/
		("random-seed", ProgramOption::value<int>(), "Random seed (101010)")

		/*実験世代数*/
		("generations", ProgramOption::value<int>(), "Max generation number (100)")
		/*発話回数*/
		("utterances", ProgramOption::value<double>(), "Uttering ratio for meaning space (0.5/[0-1])")
		/*エージェントに渡す意味の数*/
		("multiple-meanings", ProgramOption::value<int>(), "At once utterance process, number of meanings given agent (2)")
		/*直接学習確率の設定*/
		("term", ProgramOption::value<double>(), "Single meaning ratio for utterances (0.0/[0-1])")
		/*バッチ処理期間の設定*/
		("window", ProgramOption::value<int>(), "Number of utterance for prediction of meaning")
		/*対称性バイアス*/
		("symmetry", "When receive multiple meanings, use symmetry bias")
		/*非完全一致対称性バイアス*/
		("ucsymmetry", "When use symmetry bias, apply similarity for symmetry bias thinking")
		/*言葉の省略*/
		("omission", "Omission")
		/*ロギング*/
		("logging,l", "Logging")
		/*分析*/
		("analyze", "Analyze each agent for expression and a number of rules")
		/*生成文字列最大長*/
		("word-length", ProgramOption::value<int>(), "Max length of created word (3)")
		/*世代における解析間隔*/
		("interspace-analysis", ProgramOption::value<int>(), "set analysis interspace for the number of generations")
		/*世代におけるロギング間隔*/
		("interspace-logging", ProgramOption::value<int>(), "set logging interspace for the number of generations")
		/*親と子の選択した意味が合っていたかどうかの結果出力*/
		("accuracy-meaning", "Output logging whether parent and child selected same meaning")
		/*辞書ファイル*/
		("dictionary", ProgramOption::value<std::string>(), "Dictionary file name for meaning space(\"./data.dic\")")

		/*生成規則再利用*/
		("keep-random-rule", "Keep created rules with a random word into parent knowledge-base")
		/*規則削除使用*/
		("delete-redundant-rules", "Delete redundant rules")
		/*補完発話*/
		("invention-utterance", "Uttering with invention")
		/*非重複発話*/
		("unique-utterance", "Do not use the same meaning on utterance process")
		/*FILE PREFIX*/
		("prefix", ProgramOption::value<std::string>(), "Set file prefix (\"MSILM\")")

		/*BASE PATH*/
		("path", ProgramOption::value<std::string>(), "Set folder for output files (\"../RESULT/\")")
		///*再開*/
		//("resume", ProgramOption::value<std::vector<std::string> >(), "Resume an experiment from saved file name. Then used path/resume_file")
		///*保存*/
		//("last-save", "Save the last state of the experiment into the file")
		///*全保存*/
		//("all-save", "Save the all state of the experiment into the file with the prefix")
		///*保存形式*/
		//("format", ProgramOption::value<std::string>(), "Set saving format (bin/[xml])")

		///*プログレスバー*/
		//("progress,p", "Show progress bar")
		;

	opt.parse(argc, argv);

	param.set_option(opt);

	// //resume
	// if (param.RESUME) {
	// 	try {
	// 		std::ifstream ifs((param.BASE_PATH + param.RESUME_FILE).c_str());

	// 		switch (param.SAVE_FORMAT) {
	// 		case MSILMParameters::BIN:
	// 		{
	// 			//exception of boost
	// 			//boost::archive::binary_iarchive ia(ifs);
	// 			//resume_agent(ia, param, MT19937::icount, MT19937::rcount, dic, meaning_space, parent_agent);
	// 		}
	// 		break;

	// 		case MSILMParameters::XML:
	// 		{
	// 			//exception of boost
	// 			//boost::archive::xml_iarchive ia(ifs);
	// 			//resume_agent(ia, param, MT19937::icount, MT19937::rcount, dic, meaning_space, parent_agent);
	// 		}
	// 		break;

	// 		default:
	// 			return 0;
	// 		}
	// 	}
	// 	catch (...) {
	// 		std::cerr << "State file Error" << std::endl;
	// 		return 0;
	// 	}
	// }
	// else {
	//initialize
	/*
		* Meaning Space
		*/
	dic.load(param.DICTIONARY_FILE);
	construct_meanings(meaning_space);
	construct_individuals(individuals, dic);
	// }

	MT19937::set_seed(param.RANDOM_SEED);
	MT19937::waste();
	KnowledgeBase::set_control(KnowledgeBase::ANTECEDE_COMPOSITION | param.CONTROLS);
	if (param.LOGGING)
		MSILMAgent::logging_on();
	if (param.SYMMETRY)
		MSILMAgent::sym_on();
	if (param.UC_SYMMETRY)
		MSILMAgent::ucsym_on();
	if (param.OMISSION)
	{
		MSILMAgent::omission_on();
		KnowledgeBase::MEANING_SPACE = meaning_space;
	}

	child_agent = parent_agent.make_child();
	Base_Counter = parent_agent.generation_index;

	//Log file Path
	LogBox::set_filepath(param.BASE_PATH + param.LOG_FILE);

	//使用意味空間の出力
	if (param.LOGGING)
	{
		LogBox::set_filepath(param.BASE_PATH + param.LOG_FILE);
		std::vector<Rule>::iterator mean_it;

		LogBox::push_log("USED RANDOM SEED");
		LogBox::push_log(std::to_string(param.RANDOM_SEED));

		mean_it = meaning_space.begin();
		LogBox::push_log("USEING MEANINGS");
		for (; mean_it != meaning_space.end(); mean_it++)
		{
			LogBox::push_log((*mean_it).to_s());
		}
		LogBox::push_log("\n");
	}

	/*
	 * Utterance times
	 */
	param.UTTERANCES = (int)round(param.PER_UTTERANCES * meaning_space.size());
	param.TERMS = (int)round(param.PER_TERM * param.UTTERANCES);
	if (param.LOGGING)
	{
		LogBox::push_log("UTTRANCE TIMES = " + std::to_string(param.UTTERANCES));
		LogBox::push_log("MULTIPLE MEANING TIMES = " + std::to_string(param.TERMS));
	}

	// /*
	//  * Progress bar construction
	//  */
	// if (param.PROGRESS) {
	// 	//boost::progress_display show_progress(param.UTTERANCES * param.MAX_GENERATIONS);
	// }

	//Parameter Output
	{
		std::string param_file("Parameters_" + param.DATE_STR + "_" + std::to_string(param.RANDOM_SEED) + ".prm");
		std::ofstream ofs((param.BASE_PATH + param_file).c_str());
		ofs << param.to_s() << std::endl;
	}

	//main loop
	while (generation_counter <= param.MAX_GENERATIONS)
	{
		std::vector<Rule> meanings_copy;
		meanings_copy = meaning_space;
		utterance_counter = 0;
		time(&start);

		//解析間隔の設定
		if (param.INTER_ANALYSIS && generation_counter != 1)
		{
			if (param.ANALYZE)
			{
				if (generation_counter % param.SPACE_ANALYSIS != 0)
				{
					param.ANALYZE = false;
				}
			}
			else
			{
				if (generation_counter % param.SPACE_ANALYSIS == 0)
				{
					param.ANALYZE = true;
				}
			}
		}
		//ロギング間隔の設定
		if (param.INTER_LOG && generation_counter != 1)
		{
			if (param.LOGGING)
			{
				if (generation_counter % param.SPACE_LOG != 0)
				{
					logging_off(param);
				}
			}
			else
			{
				if (generation_counter % param.SPACE_ANALYSIS == 0)
				{
					logging_on(param);
				}
			}
		}

		// #ifdef DEBUG
		// 		std::cerr << "Start Generation:" << generation_counter << std::endl;
		// #endif

		if (param.LOGGING)
		{
			LogBox::push_log("\nGENERATION: " + std::to_string(generation_counter + Base_Counter));
			LogBox::push_log("BEFORE TALKING");
			LogBox::push_log("\nPARRENT KNOWLEDGE");
			LogBox::push_log(parent_agent.to_s());
			LogBox::push_log("\n-->>EDUCATION");
		}
		// #ifdef DEBUG
		// 		std::cerr << "Say & learn" << std::endl;
		// #endif

		cognition_task_init(cognition_task_flag, param);

		std::vector<int> pseudo_meanings;
		cognittion_correct_data.push_back(pseudo_meanings);
		while (utterance_counter < param.UTTERANCES && meanings_copy.size() != 0)
		{
			int old_utterance_counter = utterance_counter;
			time(&now);
			history_meanings.clear();
			history_utterance.clear();
			for (int i = 0; i < param.WINDOW && utterance_counter < param.UTTERANCES; i++)
			{
				std::vector<Rule> meanings;
				if (difftime(now, start) > limit_time)
				{
					std::cerr << "TIME OVER : GENERATION " << generation_counter + Base_Counter << std::endl;
					return 0;
				}

				if (cognition_task_flag[utterance_counter] == 1)
				{
					use_meanings.clear();
					use_meaning_index = MT19937::irand(0, meanings_copy.size() - 1);
					use_meaning = meanings_copy[use_meaning_index];
					use_meanings.push_back(use_meaning);
					meanings = use_meanings;
				}
				else
				{
					use_meaning_indexs.clear();
					use_meanings.clear();
					use_meaning_indexs = choice_meanings(meanings_copy, param);
					std::vector<int>::iterator use_meaning_indexs_it = use_meaning_indexs.begin();
					for (; use_meaning_indexs_it != use_meaning_indexs.end(); use_meaning_indexs_it++)
					{
						use_meanings.push_back(meanings_copy[(*use_meaning_indexs_it)]);
					}
					meanings = use_meanings;
				}

				if (param.LOGGING)
				{
					LogBox::push_log("\n\n\n" + std::to_string(utterance_counter + 1) + " UTTERANCE(S)");
					if (cognition_task_flag[utterance_counter] == 1)
					{
						LogBox::push_log(
							"MEANING INDEX: [" + std::to_string(use_meaning_index) + "]");
						LogBox::push_log("MEANING: " + (meanings_copy[use_meaning_index]).to_s());
					}
					else
					{
						LogBox::push_log("USE MULTIPLE MEANINGS:");
						std::vector<int>::iterator use_meaning_indexs_it = use_meaning_indexs.begin();
						for (; use_meaning_indexs_it != use_meaning_indexs.end(); use_meaning_indexs_it++)
						{
							LogBox::push_log("INDEX: [" + std::to_string((*use_meaning_indexs_it)) + "]");
							LogBox::push_log("MEANING: " + (meanings_copy[(*use_meaning_indexs_it)]).to_s());
						}
					}
				}

				// #ifdef DEBUG
				// 				std::cerr << "Say";
				// #endif

				history_meanings.push_back(meanings);

				if (param.UNIQUE_UTTERANCE)
				{ //future work
					if (param.MULTIPLE_MEANINGS > 1 && cognition_task_flag[utterance_counter] != 1)
					{
						std::vector<int>::iterator use_meaning_indexs_it = use_meaning_indexs.begin();
						std::vector<Rule> eraselist;
						std::vector<Rule>::iterator eraselist_it;
						for (; use_meaning_indexs_it != use_meaning_indexs.end(); use_meaning_indexs_it++)
						{
							Rule store_tmp = meanings_copy[(*use_meaning_indexs_it)];
							eraselist.push_back(store_tmp);
						}
						eraselist_it = eraselist.begin();
						for (; eraselist_it != eraselist.end(); eraselist_it++)
						{
							std::vector<Rule>::iterator meanings_copy_it = meanings_copy.begin();
							for (; meanings_copy_it != meanings_copy.end(); meanings_copy_it++)
							{
								if ((*meanings_copy_it) == (*eraselist_it))
								{
									meanings_copy.erase(meanings_copy_it);
									break;
								}
							}
						}
					}
					else
					{
						meanings_copy.erase(meanings_copy.begin() + use_meaning_index);
					}
				}

				if (param.PROGRESS)
					// ++show_progress;

					utterance_counter++;
			}
			history_utterance = parent_agent.cognittion_say(history_meanings);

			child_agent.cognittion_hear(history_utterance, history_meanings, meaning_space);

			std::vector<Rule> parent_meaning, child_meaning;
			parent_meaning = parent_agent.return_last_selected_meaning();
			child_meaning = child_agent.return_last_selected_meaning();
			for (int index = 0; index < parent_meaning.size() && index < child_meaning.size(); index++)
			{
				if (cognition_task_flag[old_utterance_counter + index] == 1 && parent_meaning[index] == child_meaning[index])
				{
					cognittion_correct_data.back().push_back(1);
				}
				else
				{
					cognittion_correct_data.back().push_back(0);
				}
			}

			// #ifdef DEBUG
			// 			std::cerr << " -> learn" << std::endl;
			// #endif

			child_agent.learn();
		}

		if (param.LOGGING)
		{
			LogBox::push_log("\n<<--EDUCATION");
			LogBox::push_log("\nGENERATION :" + std::to_string(generation_counter + Base_Counter));
			LogBox::push_log("AFTER TALKING");
			LogBox::push_log("\nCHILD KNOWLEDGE");
			LogBox::push_log(child_agent.to_s());
		}

		child_agent = child_agent.grow(meaning_space);

		// #ifdef DEBUG
		// 		std::cerr << "Save State" << std::endl;
		// #endif

		// if (param.SAVE_ALL_STATE)
		// 	all_generations.push_back(child_agent);

		// #ifdef DEBUG
		// 		std::cerr << "Analyze" << std::endl;
		// #endif
		if (param.ANALYZE)
		{
			analyze_and_output(param, meaning_space,
							   child_agent, parent_agent, generation_counter + Base_Counter);
			analyze_and_output(param, meaning_space,
							   parent_agent, child_agent, generation_counter + Base_Counter);
		}

		if (param.LOGGING)
		{
			LogBox::refresh_log();
		}

		// #ifdef DEBUG
		// 		std::cerr << "Change Generation" << std::endl;
		// #endif

		parent_agent = child_agent;
		child_agent = parent_agent.make_child();
		generation_counter++;
	}
	if (param.ACC_MEA)
	{
		std::string mea_file;
		mea_file = param.FILE_PREFIX + "_" + param.DATE_STR + "_" + std::to_string(param.RANDOM_SEED) + ".mea.acc";
		accuracy_meaning_output(param, mea_file, cognittion_correct_data);
	}

	// //saving proc
	// if (param.SAVE_LAST_STATE) {
	// 	std::ofstream ofs((param.BASE_PATH + param.SAVE_FILE).c_str());
	// 	switch (param.SAVE_FORMAT) {
	// 	case MSILMParameters::BIN:
	// 	{
	// 		//exception of boost
	// 		//boost::archive::binary_oarchive oa(ofs);
	// 		//int counter;
	// 		//counter = Base_Counter + param.MAX_GENERATIONS;
	// 		//save_agent<boost::archive::binary_oarchive>
	// 		//	(oa, param, MT19937::icount, MT19937::rcount, dic, meaning_space, parent_agent);
	// 	}
	// 	break;

	// 	case MSILMParameters::XML:
	// 	{
	// 		//exception of boost
	// 		//boost::archive::xml_oarchive oa(ofs);
	// 		//int counter;
	// 		//counter = Base_Counter + param.MAX_GENERATIONS;
	// 		//save_agent<boost::archive::xml_oarchive>
	// 		//	(oa, param, MT19937::icount, MT19937::rcount, dic, meaning_space, parent_agent);
	// 	}
	// 	break;

	// 	default:
	// 		std::cerr << "UNKNOWN FORMAT" << std::endl;
	// 		return 0;
	// 	}

	// }
	// if (param.SAVE_ALL_STATE) {
	// 	std::vector<MSILMAgent>::iterator a_it;
	// 	int index = 0;
	// 	a_it = all_generations.begin();
	// 	while (a_it != all_generations.end()) {
	// 		std::string index_str;
	// 		index_str = std::to_string(index + Base_Counter);
	// 		std::string stf((param.FILE_PREFIX + "_Gen_" + index_str + ".st").c_str());
	// 		std::ofstream ofs((param.BASE_PATH + stf).c_str());

	// 		switch (param.SAVE_FORMAT) {
	// 		case MSILMParameters::BIN:
	// 		{
	// 			//exception of boost
	// 			//boost::archive::binary_oarchive oa(ofs);
	// 			//save_agent(oa, param, MT19937::icount, MT19937::rcount, dic, meaning_space, *a_it);
	// 		}
	// 		break;

	// 		case MSILMParameters::XML:
	// 		{
	// 			//exception of boost
	// 			//boost::archive::xml_oarchive oa(ofs);
	// 			//save_agent(oa, param, MT19937::icount, MT19937::rcount, dic, meaning_space, *a_it);
	// 		}
	// 		break;

	// 		default:
	// 			std::cerr << "UNKNOWN FORMAT" << std::endl;
	// 			return 0;
	// 		}

	// 		index++;
	// 		a_it++;
	// 	}
	// }

	//delete
	//delete show_progress;

	if (param.LOGGING)
		log.refresh_log();

	return 0;
}
