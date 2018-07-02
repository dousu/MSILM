/*
 * File:   MSILM_main.cpp
 * Author: hiroki
 *
 * Created on October 28, 2013, 6:12 PM
 */

#include "MSILM_main.h"

void construct_meanings(std::vector<Rule> & meanings)
{
	constexpr int VERB_INDEX_BEGIN = 0;
	constexpr int VERB_INDEX_END = 4;
	constexpr int NOUN_INDEX_BEGIN = 5;
	constexpr int NOUN_INDEX_END = 9;

	std::array<std::size_t, VERB_INDEX_END - VERB_INDEX_BEGIN + 1> verb;
	std::array<std::size_t, NOUN_INDEX_END - NOUN_INDEX_BEGIN + 1> noun;
	std::iota(verb.begin(), verb.end(), 0);
	std::iota(noun.begin(), noun.end(), 5);

	auto func1 = [&noun, &meanings](int i) {
		auto func2 = [&i, &noun, &meanings](int j) {
			auto func3 = [&i, &j, &meanings](int k) {
				if(j != k){
					std::vector<Element> internal;
					Rule mean;

					internal.push_back(Mean(i));
					internal.push_back(Mean(j));
					internal.push_back(Mean(k));

					mean.internal = internal;
					meanings.push_back(mean);
				}
			};
			std::for_each(std::begin(noun), std::end(noun), func3);
		};
		std::for_each(std::begin(noun), std::end(noun), func2);
	};
	std::for_each(std::begin(verb), std::end(verb), func1);
}

std::vector<std::vector<double>> analyze(std::vector<Rule> & meanings,
										 MSILMAgent & agent1, MSILMAgent & agent2)
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

void unit_analyze(std::vector<double> & result_matrix,
				  std::vector<Rule> & meanings, MSILMAgent & agent)
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

double expression(std::vector<Rule> & meanings, MSILMAgent & agent)
{
	int counter = 0;
	auto func = [&agent, &counter](Rule & meaning){
		if(agent.utterable(meaning)){
			counter++;
		}
	};
	std::for_each(std::begin(meanings),std::end(meanings),func);

	return (static_cast<double>(counter)) / (static_cast<double>(meanings.size())) * 100.0;
}

void calculate_language_distance(
	std::vector<double> &lev_matrix,
	std::vector<Rule> &meanings,
	MSILMAgent &agent1, MSILMAgent &agent2)
{
	//言語間距離を計算
	lev_matrix.push_back(calculate_distance(meanings, agent1.kb, agent2.kb));
	lev_matrix.push_back(calculate_average_word_length(meanings, agent1.kb));
}

void analyze_and_output(MSILMParameters &param, std::vector<Rule> & meaning_space,
						MSILMAgent &agent1, MSILMAgent &agent2, int index)
{
	std::vector<std::vector<double>> res;
	std::string index_str, file, file_postfix;

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

void accuracy_meaning_output(MSILMParameters & param, std::string & file, std::vector<std::vector<int>> & data)
{
	{
		std::string path;
		path = param.BASE_PATH + file;
		std::ofstream ofs(path.c_str());

		auto func1 = [&ofs](const std::vector<int> & vec){
			std::string str;
			auto func2 = [&str](const int & val){
				str += std::to_string(val) + " ";
			};
			std::for_each(std::begin(vec), std::end(vec), func2);
			str.pop_back();
			ofs << str << std::endl;
		};
		std::for_each(std::begin(data), std::end(data), func1);
		ofs.close();
	}
}

double calculate_distance(std::vector<Rule> & meanings,
						  KnowledgeBase & kb1, KnowledgeBase & kb2)
{

	//from kb1 to kb2

	std::vector<Rule> kb1_all, kb2_all;

	auto utt_list_func = [&kb1, &kb2, &kb1_all, &kb2_all](Rule mean){
		std::vector<Rule> rules1, rules2;
		rules1 = kb1.grounded_rules(mean);
		if(rules1.size() != 0){
			std::copy(std::begin(rules1), std::end(rules1), std::back_inserter(kb1_all));
		}else{
			kb1_all.push_back(kb1.fabricate_for_complementing(mean));
		}
		rules2 = kb2.grounded_rules(mean);
		if(rules2.size() != 0){
			std::copy(std::begin(rules2), std::end(rules2), std::back_inserter(kb2_all));
		}else{
			kb2_all.push_back(kb2.fabricate_for_complementing(mean));
		}
	};
	std::for_each(std::begin(meanings), std::end(meanings), utt_list_func);

	double lev_sum = 0.0;
	auto dist = [&kb2_all, &lev_sum](Rule rule1){
		std::vector<Rule> targets;
		double min_ham = std::numeric_limits<double>::max();
		auto hamming = [&rule1, &targets, &min_ham](Rule rule2){
			double ham = Distance::hamming(rule1.internal, rule2.internal);
			if(ham < min_ham){
				min_ham = ham;
				targets.clear();
				targets.push_back(rule2);
			}else if(ham == min_ham){
				targets.push_back(rule2);
			}
		};
		std::for_each(std::begin(kb2_all), std::end(kb2_all), hamming);
		//levenshtein distance
		double min_lev = 1.0;
		auto leven = [&rule1, &targets, &min_lev](Rule rule2){
			double lev = Distance::levenstein(rule1.external, rule2.external);
			if(lev < min_lev){
				min_lev = lev;
			}
		};
		std::for_each(std::begin(targets), std::end(targets), leven);
		lev_sum += min_lev;
	};
	std::for_each(std::begin(kb1_all), std::end(kb1_all), dist);

	return lev_sum / (static_cast<double>(kb1_all.size()));
}

double calculate_average_word_length(std::vector<Rule> & meanings, KnowledgeBase & kb1)
{
	
	double word_length = 0, length_cnt = 0;
	auto func = [&kb1, &word_length, &length_cnt](Rule mean){
		std::vector<Rule> rules1;
		rules1 = kb1.grounded_rules(mean);
		if(rules1.size() != 0){
			std::for_each(std::begin(rules1), std::end(rules1), [&word_length](Rule r){word_length += r.external.size();});
			length_cnt += rules1.size();
		}else{
			word_length += kb1.fabricate_for_complementing(mean).external.size();
			length_cnt++;
		}
	};
	std::for_each(std::begin(meanings), std::end(meanings), func);
	return word_length / length_cnt;
}

std::string
tr_vector_double_to_string(std::vector<double> & vec)
{
	std::string str = "(";
	auto func = [&str](const double & val){
		str += std::to_string(val) + ",";
	};
	std::for_each(std::begin(vec), std::end(vec), func);
	str.pop_back();
	return str + std::string(")");
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

void cognition_task_init(std::vector<int> & source, MSILMParameters & param)
{
	source.assign(param.UTTERANCES, 0);
	std::fill_n(std::begin(source), param.TERMS, 1);
	std::shuffle(source.begin(), source.end(), MT19937::igen);
}

int main(int argc, char *argv[])
{
	typedef std::vector<MSILMAgent> Generation;

	Dictionary dic;
	LogBox log; //for destructor
	time_t start, now;

	limit_time = 20 * 60; //second

	MSILMParameters param;
	//boost::progress_display *show_progress = 0;

	std::vector<Rule> meaning_space;
	MSILMAgent parent_agent, child_agent;

	int utterance_counter = 0;

	int generation_counter = 1;
	Rule utter;
	std::vector<Rule> history_utterance;
	std::vector<std::vector<Rule>> history_meanings;
	std::vector<int> cognition_task_flag;

	std::vector<std::vector<int>> cognition_correct_data;

	/**************************************************
	 *
	 * OPTION PROCESSING
	 *
	 **************************************************/
	ProgramOption opt;
	opt.add_option()("help", "Description")
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
		("logging", "Logging")
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
		("invention", "Uttering with invention")
		/*FILE PREFIX*/
		("prefix", ProgramOption::value<std::string>(), "Set file prefix (\"MSILM\")")
		/*BASE PATH*/
		("path", ProgramOption::value<std::string>(), "Set folder for output files (\"../RESULT/\")")
		///*プログレスバー*/
		//("progress,p", "Show progress bar")
		;

	opt.parse(argc, argv);

	param.set_option(opt);

	//initialize
	/*
	 * Meaning Space
	 */
	dic.load(param.DICTIONARY_FILE);
	construct_meanings(meaning_space);
	MT19937::set_seed(param.RANDOM_SEED);
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

	//Log file Path
	LogBox::set_filepath(param.BASE_PATH + param.LOG_FILE);

	//使用意味空間の出力
	if (param.LOGGING)
	{
		LogBox::push_log("USED RANDOM SEED");
		LogBox::push_log(std::to_string(param.RANDOM_SEED));

		std::for_each(std::begin(meaning_space), std::end(meaning_space), [](Rule & mean){LogBox::push_log(mean.to_s());});
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
		utterance_counter = 0;
		time(&start);

		//解析間隔の設定
		if (param.INTER_ANALYSIS && generation_counter != 1) //初期世代は解析する
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
		if (param.INTER_LOG && generation_counter != 1) //解析に合わせる
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

		if (param.LOGGING)
		{
			LogBox::push_log("\nGENERATION: " + std::to_string(generation_counter));
			LogBox::push_log("BEFORE TALKING");
			LogBox::push_log("\nPARRENT KNOWLEDGE");
			LogBox::push_log(parent_agent.to_s());
			LogBox::push_log("\n-->>EDUCATION");
		}

		//多義意味環境での発話タイミングをランダムに設定
		cognition_task_init(cognition_task_flag, param);

		//各世代での正解数を記録
		cognition_correct_data.push_back(std::vector<int>());
		
		while (utterance_counter < param.UTTERANCES)
		{
			int old_utterance_counter = utterance_counter; //バッチ処理対応
			time(&now); //処理の時間制限
			history_meanings.clear();
			history_utterance.clear();
			for (int i = 0; i < param.WINDOW && utterance_counter < param.UTTERANCES; i++)
			{
				std::vector<Rule> meanings;
				if (difftime(now, start) > limit_time)
				{
					std::cerr << "TIME OVER : GENERATION " << generation_counter << std::endl;
					exit(1);
				}

				if (cognition_task_flag[utterance_counter] == 1)
				{
					meanings.push_back(meaning_space[MT19937::irand(0, meaning_space.size() - 1)]);
				}
				else
				{
					std::sample(std::begin(meaning_space), std::end(meaning_space), std::back_inserter(meanings), param.MULTIPLE_MEANINGS, MT19937::igen);
				}

				if (param.LOGGING)
				{
					LogBox::push_log("\n\n\n" + std::to_string(utterance_counter + 1) + " UTTERANCE(S)");
					if (cognition_task_flag[utterance_counter] == 1)
					{
						LogBox::push_log("MEANING: " + meanings.front().to_s());
					}
					else
					{
						LogBox::push_log("USE MULTIPLE MEANINGS:");
						std::for_each(std::begin(meanings), std::end(meanings), [](Rule & mean){LogBox::push_log("MEANING: " + mean.to_s());});
					}
				}

				history_meanings.push_back(meanings);

				//if (param.PROGRESS)
				//	 ++show_progress;

				utterance_counter++;
			}
			history_utterance = parent_agent.cognition_say(history_meanings);

			child_agent.cognition_hear(history_utterance, history_meanings, meaning_space);

			//要改善
			{
				std::vector<Rule> parent_meaning, child_meaning;
				parent_meaning = parent_agent.return_last_selected_meaning();
				child_meaning = child_agent.return_last_selected_meaning();
				for (int index = 0; index < parent_meaning.size() && index < child_meaning.size(); index++)
				{
					if (cognition_task_flag[old_utterance_counter + index] == 1 && parent_meaning[index] == child_meaning[index])
					{
						cognition_correct_data.back().push_back(1);
					}
					else
					{
						cognition_correct_data.back().push_back(0);
					}
				}
			}

			child_agent.learn();
		}

		if (param.LOGGING)
		{
			LogBox::push_log("\n<<--EDUCATION");
			LogBox::push_log("\nGENERATION :" + std::to_string(generation_counter));
			LogBox::push_log("AFTER TALKING");
			LogBox::push_log("\nCHILD KNOWLEDGE");
			LogBox::push_log(child_agent.to_s());
		}

		child_agent = child_agent.grow(meaning_space);

		if (param.ANALYZE)
		{
			analyze_and_output(param, meaning_space,
							   child_agent, parent_agent, generation_counter);
			analyze_and_output(param, meaning_space,
							   parent_agent, child_agent, generation_counter);
		}

		if (param.LOGGING)
		{
			LogBox::refresh_log();
		}

		parent_agent = child_agent;
		child_agent = parent_agent.make_child();
		generation_counter++;
	}
	if (param.ACC_MEA)
	{
		std::string mea_file;
		mea_file = param.FILE_PREFIX + "_" + param.DATE_STR + "_" + std::to_string(param.RANDOM_SEED) + ".mea.acc";
		accuracy_meaning_output(param, mea_file, cognition_correct_data);
	}

	//delete
	//delete show_progress;

	if (param.LOGGING)
		log.refresh_log();

	return 0;
}
