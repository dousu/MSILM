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
		MEAN_TYPE = 1,
		//!変数:internal
		VAR_TYPE = 2,
		//!シンボル:external
		SYM_TYPE = 3,
		//!カテゴリ付き変数:external
		CAT_TYPE = 4		
	};
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
	bool operator!=(const Mean &dst) const {
		return !(*this == dst);
	};
	bool operator<(const Mean &dst) const{
		return obj < dst.obj;
	};
	std::string to_s() const{
		if (Dictionary::individual.find(obj) == Dictionary::individual.end())
		{
			return "*";
		}else{
			return Dictionary::individual[obj];
		}
	}
};

class Nonterminal;

class Variable{
	friend class Nonterminal;
	int cat;
	int obj;
public:
	Variable(int cat_num, int var_num) : cat(cat_num), obj(var_num) {};
	bool operator==(const Variable & dst) const {
		return obj == dst.obj;
	};
	bool operator!=(const Variable & dst) const {
		return !(*this == dst);
	};
	bool operator==(const Nonterminal & dst) const;
	bool operator!=(const Nonterminal & dst) const;
	bool operator<(const Variable & dst) const{
		return obj < dst.obj;
	};
	std::string to_s() const{
		return Prefices::VAR + std::to_string(obj);
	}
};

class Symbol{
	int obj;
public:
	Symbol(int num) : obj(num) {};
	bool operator==(const Symbol &dst) const {
		return obj == dst.obj;
	};
	bool operator!=(const Symbol &dst) const {
		return !(*this == dst);
	};
	bool operator<(const Symbol &dst) const{
		return obj < dst.obj;
	};
	std::string to_s() const{
		if (Dictionary::symbol.find(obj) == Dictionary::symbol.end())
		{
			return "*";
		}else{
			return Dictionary::symbol[obj];
		}
	}
};

class Nonterminal{
	friend class Variable;
	int cat;
	int obj;
public:
	Nonterminal(int cat_num, int obj_num) : cat(cat_num), obj(obj_num) {};
	bool operator==(const Nonterminal & dst) const {
		return cat == dst.cat;
	};
	bool operator!=(const Nonterminal & dst) const {
		return !(*this == dst);
	};
	bool operator==(const Variable & dst) const;
	bool operator!=(const Variable & dst) const;
	bool operator<(const Nonterminal & dst) const{
		return cat < dst.cat || (cat == dst.cat && obj < dst.obj);
	};
	std::string to_s() const{
		return Prefices::CAT + std::to_string(cat) + Prefices::DEL + Prefices::VAR + std::to_string(obj);
	}
};

class Element{
	using ElementType = std::variant<std::monostate, Mean, Variable, Symbol, Nonterminal>;
	ElementType element;
public:
	Element() : element() {};
	Element(const Element & other){
		element = other.element;
	};
	constexpr std::size_t type() const{
		return element.index();
	};

	template <typename T>
	T & get() {return std::get<T>(element);};

	template <int I>
	std::variant_alternative_t<I, ElementType> & get() {return std::get<I>(element);};
	Element & operator=(const Element &dst){
		element = dst.element;
		return *this;
	};
	Element & operator=(const Mean & dst){
		element = dst;		
		return *this;
	};
	Element & operator=(const Variable & dst){
		element = dst;		
		return *this;
	};
	Element & operator=(const Symbol & dst){
		element = dst;		
		return *this;
	};
	Element & operator=(const Nonterminal & dst){
		element = dst;		
		return *this;
	};
	bool operator==(const Element &dst) const {
		return type() == dst.type() && element == dst.element;
	};
	bool operator!=(const Element &dst) const {
		return !(*this == dst);
	};
	bool operator<(const Element &dst) const{
		return type() < dst.type() || (type() == dst.type() && element < dst.element);
	};
	std::string to_s() const{
		std::string str("");
		Mean m; Variable v; Symbol s; Nonterminal n;
		switch(type()){
			case ELEM_TYPE::MEAN_TYPE :
				m = get<type()>();
				str = m.to_s();
				break;
			case ELEM_TYPE::VAR_TYPE :
				v = get<type()>();
				str = v.to_s();
				break;
			case ELEM_TYPE::SYM_TYPE :
				s = get<type()>();
				str = s.to_s();
				break;
			case ELEM_TYPE::CAT_TYPE :
				n = get<type()>();
				str = n.to_s();
				break;
			default:
				str = "*";
		}
		return str;//(std::get<type(), ElementType>(element)).to_s();
	}
};

#endif /* ELEMENT_H_ */
