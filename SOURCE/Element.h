/*
 * Element.h
 *
 *  Created on: 2012/11/19
 *      Author: Hiroki Sudo
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_
#include <iostream>
#include <map>
#include <string>
#include <variant>

#include "Dictionary.h"
#include "Prefices.h"

/*!
 * Elementクラスが取るタイプのインデックスを定義しています。
 */
namespace ELEM_TYPE
{
	enum Type{
		//!意味:internal
		MEAN_TYPE = 0;
		//!変数:internal
		VAR_TYPE = 1;
		//!シンボル:external
		SYM_TYPE = 2;
		//!カテゴリ付き変数:external
		CAT_TYPE = 3;		
	}
}

//型
/*!
 * 内部言語と外部言語の要素を表すクラスです。
 * メンバ変数は全て数値で扱われ、インデックスで表現されています。
 * このインデックスは静的メンバ変数のDictionaryクラスインスタンスに依存します。
 *
 * なお、boost%:%:serializationに対応しています。
 */
/*class Element
{

  public:
	//メンバ
	//! 要素を区別するインデックスを格納しています。（例: これが「like」で、likeのインデックスが2、ならobj=2）
	int obj;

	//!Elementがカテゴリ付き変数または変数である場合、そのカテゴリのインデックスを格納します。（例: C1/x2なら、cat=1）
	int cat;

	//! Elementのタイプを格納しています。タイプはELEM_TYPEに示されるとおりです。
	ELEM_TYPE::Type type;

	static Dictionary dictionary;

	//コンストラクタ
	Element();

	//デストラクタ
	//virtual ~Element();

	//operator
	//!等号。型が異なると偽を返します。型が等しい場合はインデックスが等しいか比べます。
	bool operator==(const Element &dst) const;
	//!等号の否定です
	bool operator!=(const Element &dst) const;
	bool operator<(const Element &dst) const;

	//!代入です
	Element &operator=(const Element &dst);

	//method
	//! 変数インデックスと、カテゴリインデックスを取り、それを使ってインスタンスをカテゴリ付き変数に初期化します。
	Element &set_cat(int var, int cat);
	//! 変数インデックスと、カテゴリインデックスを取り、それを使ってインスタンスを変数に初期化します。内部言語の変数は、必ず外部言語でカテゴリ付き変数として出現するので、初期化にはそれと等しいカテゴリインデックスが必要です。
	Element &set_var(int var, int cat);
	//! 対象のインデックスを引数に取り、それを使ってインスタンスを内部言語の対象に初期化します。
	Element &set_ind(int id);
	//! 対象のインデックスを引数に取り、それを使ってインスタンスを外部言語の記号に初期化します。
	Element &set_sym(int id);

	//! インスタンスが変数ならtrueを、そうでなければfalseを返します。
	bool is_var(void) const;
	//! インスタンスがカテゴリ付き変数であればtrueを、そうでなければfalseを返します。
	bool is_cat(void) const;
	//! インスタンスが対象であればtrueを、そうでなければfalseを返します。
	bool is_ind(void) const;
	//! インスタンスが記号であればtrueを、そうでなければfalseを返します。
	bool is_sym(void) const;

	Element &set(int type_id, int obj_id, int sub_id);

	//! インスタンスの文字列表現をstringで返します。（例:インスタンスが、カテゴリインデックス1で、変数インデックス2なら、"C1/x2"が返ってきます。）
	std::string to_s(void);

};*/

class Mean{
	int obj;
public:
	Mean(int num) : obj(num) {};
	bool operator==(const Mean &dst) const {
		return obj == dst.obj;
	};
	//!等号の否定です
	bool operator!=(const Mean &dst) const {
		return !(*this == dst);
	};
	bool operator<(const Mean &dst) const{
		return obj < dst.obj;
	};
};

class Variable{
	int obj;
public:
	Variable(int num) : obj(num) {};
	bool operator==(const Variable &dst) const {
		return obj == dst.obj;
	};
	//!等号の否定です
	bool operator!=(const Variable &dst) const {
		return !(*this == dst);
	};
	bool operator<(const Variable &dst) const{
		return obj < dst.obj;
	};
};

class Symbol{
	int obj;
public:
	Symbol(int num) : obj(num) {};
	bool operator==(const Symbol &dst) const {
		return obj == dst.obj;
	};
	//!等号の否定です
	bool operator!=(const Symbol &dst) const {
		return !(*this == dst);
	};
	bool operator<(const Symbol &dst) const{
		return obj < dst.obj;
	};
};

class Nonterminal{
	int cat;
	int obj;
public:
	Nonterminal(int cat_num, int obj_num) : cat(cat_num), obj(obj_num) {};
	bool operator==(const Variable &dst) const {
		return cat == dst.cat && obj == dst.obj;
	};
	//!等号の否定です
	bool operator!=(const Variable &dst) const {
		return !(*this == dst);
	};
	bool operator<(const Variable &dst) const{
		return cat < dst.cat || (cat == dst.cat && obj < dst.obj);
	};
};

class newElement{
	ELEM_TYPE::Type type;
	std::variant<Mean, Variable, Symbol, Nonterminal> element;
public:
	Element() : element() {};
	Element(const Element & other){
		switch(other.type){
			case ELEM_TYPE::CAT_TYPE :
				element = std::get<Nonterminal>(other.element);
				break;
			case ELEM_TYPE::SYM_TYPE :
				element = std::get<Symbol>(other.element);
				break;
			case ELEM_TYPE::VAR_TYPE :
				element = std::get<Variable>(other.element);
				break;
			case ELEM_TYPE::MEAN_TYPE :
				element = std::get<Mean>(other.element);
				break;
			default :
				std::cerr << "Element Substitution Error" << std::endl;
				exit(1);
		}
		return *this;
	};

	template <typename T>
	T get() {return std::get<T>(element)};

	//operator
	//!等号。型が異なると偽を返します。型が等しい場合はインデックスが等しいか比べます。
	bool operator==(const Element &dst) const {
		return type == dst.type && element == dst.element;
	};
	//!等号の否定です
	bool operator!=(const Element &dst) const {
		return !(*this == dst);
	};
	bool operator<(const Element &dst) const{
		return type < dst.type || (type == dst.type && element < dst.element);
	};
};

#endif /* ELEMENT_H_ */
