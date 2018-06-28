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

class Mean{
	const int obj;
public:
	Mean(int num) : obj(num) {};
	Mean(const Mean & dst) : obj(dst.obj) {};
	bool operator==(const Mean & dst) const {
		return obj == dst.obj;
	};
	bool operator!=(const Mean & dst) const {
		return !(*this == dst);
	};
	bool operator<(const Mean & dst) const{
		return obj < dst.obj;
	};
	int get_obj_id() const {
		return obj;
	}
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
	Variable(const Variable & dst) : cat(dst.cat), obj(dst.obj) {};
	bool operator==(const Variable & dst) const {
		return obj == dst.obj && cat == dst.cat;
	};
	bool operator!=(const Variable & dst) const {
		return !(*this == dst);
	};
	bool operator==(const Nonterminal & dst) const;
	bool operator!=(const Nonterminal & dst) const;
	bool operator<(const Variable & dst) const{
		return obj < dst.obj;
	};
	int get_cat_id() const {
		return cat;
	}
	std::string to_s() const{
		return Prefices::VAR + std::to_string(obj);
	}
};

class Symbol{
	int obj;
public:
	Symbol(int num) : obj(num) {};
	Symbol(const Symbol & dst) : obj(dst.obj) {};
	bool operator==(const Symbol & dst) const {
		return obj == dst.obj;
	};
	bool operator!=(const Symbol & dst) const {
		return !(*this == dst);
	};
	bool operator<(const Symbol & dst) const{
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
	Nonterminal(const Nonterminal & dst) : cat(dst.cat), obj(dst.obj) {};
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
	int get_cat_id() const {
		return cat;
	}
	std::string to_s() const{
		return Prefices::CAT + std::to_string(cat) + Prefices::DEL + Prefices::VAR + std::to_string(obj);
	}
};

class Element{
	using ElementType = std::variant<std::monostate, Mean, Variable, Symbol, Nonterminal>;
	ElementType element;
	//template <int I, typename T>
	//std::variant_alternative_t<I, T> & get() const {return std::get<I>(element);}
	template <int I>
	const decltype(auto) get() const {return std::get<I>(element);}
public:
	Element() : element() {}
	Element(const Element & other) : element(other.element){}
	constexpr std::size_t type() const{
		return element.index();
	};

	template <typename T>
	T & get() const {return std::get<T>(element);};
	
	template <typename T>
	Element & operator=(T && dst){
		element = std::forward(dst);
		return *this;
	}
	/*Element & operator=(Element && dst){
		element = std::forward(dst.element);
		return *this;
	};
	Element & operator=(Mean && dst){
		element = std::forward(dst);
		return *this;
	};
	Element & operator=(Variable && dst){
		element = std::forward(dst);
		return *this;
	};
	Element & operator=(Symbol && dst){
		element = std::forward(dst);
		return *this;
	};
	Element & operator=(Nonterminal && dst){
		element = std::forward(dst);
		return *this;
	};*/
	bool operator==(const Element & dst) const {
		return type() == dst.type() && element == dst.element;
	}
	bool operator!=(const Element & dst) const {
		return !(*this == dst);
	}
	bool operator<(const Element & dst) const{
		return type() < dst.type() || (type() == dst.type() && element < dst.element);
	}
	std::string to_s() const{
		std::string str("");
		switch(type()){
			case ELEM_TYPE::MEAN_TYPE :
				str = Mean(get<ELEM_TYPE::MEAN_TYPE>()).to_s();
				break;
			case ELEM_TYPE::VAR_TYPE :
				str = Variable(get<ELEM_TYPE::VAR_TYPE>()).to_s();
				break;
			case ELEM_TYPE::SYM_TYPE :
				str = Symbol(get<ELEM_TYPE::SYM_TYPE>()).to_s();
				break;
			case ELEM_TYPE::CAT_TYPE :
				str = Nonterminal(get<ELEM_TYPE::CAT_TYPE>()).to_s();
				break;
			default:
				str = "*";
		}
		return str;//(std::get<type(), ElementType>(element)).to_s();
	}
};

#endif /* ELEMENT_H_ */
