/*
 * MT19937.h
 *
 *  Created on: 2012/12/04
 *      Author: Hiroki Sudo
 */

#ifndef MT19937_H_
#define MT19937_H_

#include <vector>
//#include <boost/random.hpp>
#include <climits>
#include <random>

/*!
 * 乱数発生器を保持するクラス。
 * このクラスは二つの乱数発生器に対するアクセスを可能にする。
 * 一つは整数乱数器、もう一つは0-1の実数乱数器。
 * このクラスの意味は、
 * 1．どのシードを使ったかを保持する
 * 2．いくつ乱数を生成したのかを保持する
 * これにより、シリアライズデータからレジュームした場合は、
 * 正確に使用済み乱数を破棄する。
 */
class MT19937 {
public:
	MT19937();
	virtual ~MT19937();


	static unsigned long long int icount;
	static unsigned long long int rcount;


	static std::mt19937 igen;
	static std::uniform_int_distribution<> idist;
	// static boost::variate_generator<boost::mt19937, boost::uniform_int<int> > _irand;

	static std::mt19937 rgen;
	static std::uniform_real_distribution<> rdist;
	// static boost::variate_generator<boost::mt19937, boost::uniform_real<double> > _rrand;

	/*!
	 * 整数乱数器
	 */
	static int irand(void);

	static int irand(int, int);

	/*!
	 * 実数乱数器
	 */
	static double rrand(void);

	static double rrand(double,double);

	/*!
	 * 使用済み乱数を破棄する
	 */
	static void waste(void);

	/*!
	 * 乱数シードを設定する
	 */
	static void set_seed(std::uint32_t seed_value);
};

#endif /* MT19937_H_ */
