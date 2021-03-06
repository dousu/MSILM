/*
 * KirbyAgent.h
 *
 *  Created on: 2012/12/07
 *      Author: Hiroki Sudo
 */

#ifndef KIRBYAGENT_H_
#define KIRBYAGENT_H_
#include <string>
#include <iostream>
//#include <boost/serialization/serialization.hpp>
//#include <boost/serialization/nvp.hpp>
//#include <boost/lexical_cast.hpp>

#include "IndexFactory.h"
#include "KnowledgeBase.h"
#include "Rule.h"
#include "LogBox.h"

/*!
 * Kirbyモデルのエージェントを表すクラスです。
 */
class KirbyAgent
{
  public:
	//! エージェントが覚えている規則（Rule）が格納されています。
	KnowledgeBase kb;

	int generation_index; //!<世代数を表します。
	static bool LOGGING_FLAG;

	/*!
	 * 受け取った規則の内部言語を見て、知識を使って外部言語を生成し、その外部言語を持つ規則を返します。
	 */
	Rule say(Rule &internal);

	/*!
	 * 受け取った規則を聴き、知識ベースに格納します。
	 */
	void hear(Rule &term);

	/*!
	 * hearで溜めた規則を使って学習を行います。
	 */
	void learn(void);

	/*!
	 * 引数に取った規則の内部言語に対して、外部言語が純粋に構成可能なとき、
	 * trueを返します。
	 */
	bool utterable(Rule &internal);

	/*!
	 * 自分の子エージェントを生成します。
	 */
	KirbyAgent make_child(void);

	/*!
	 * 自分が成長します。これは自分自身を返すだけの関数です。
	 */
	KirbyAgent &grow(std::vector<Rule>);

	/*!
	 * エージェントの文字列表現を返します。具体的には、自分の知識の文字列表現を返します。
	 */
	std::string to_s(void);

	KirbyAgent &operator=(const KirbyAgent &dst);

	KirbyAgent();
	virtual ~KirbyAgent();
	static void logging_on(void);
	static void logging_off(void);

	/*private:
	IndexFactory serial_generator;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int){
    	ar & BOOST_SERIALIZATION_NVP(kb);
    	ar & BOOST_SERIALIZATION_NVP(generation_index);
    }*/
};

#endif /* KIRBYAGENT_H_ */
