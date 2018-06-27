/*
 * File:   MSILM_main.cpp
 * Author: hiroki
 *
 * Created on October 28, 2013, 6:12 PM
 */

#include "MSILMParameters.h"

int main(int argc, char *argv[])
{
	MSILMParameters param;

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
		("invention-utterance", "Uttering with invention")
		/*非重複発話*/
		("unique-utterance", "Do not use the same meaning on utterance process")
		/*FILE PREFIX*/
		("prefix", ProgramOption::value<std::string>(), "Set file prefix (\"MSILM\")")
		/*BASE PATH*/
		("path", ProgramOption::value<std::string>(), "Set folder for output files (\"../RESULT/\")")
		///*プログレスバー*/
		//("progress,p", "Show progress bar")
		;
	std::cout << "Complete setting options." << std::endl;
	opt.parse(argc, argv);
	std::cout << "Complete parsing argv." << std::endl;

	param.set_option(opt);

	/*
	 * Utterance times
	 */
	param.UTTERANCES = (int)round(param.PER_UTTERANCES * static_cast<std::size_t>(100));
	param.TERMS = (int)round(param.PER_TERM * param.UTTERANCES);

	//Parameter Output
	std::cout << param.to_all_s() << std::endl;

	return 0;
}
