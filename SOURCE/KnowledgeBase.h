/*
 * KnowledgeBase.h
 *
 *  Created on: 2012/11/20
 *      Author: Hiroki Sudo
 */

#ifndef KNOWLEDGEBASE_H_
#define KNOWLEDGEBASE_H_
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <algorithm>
#include <map>
#include <sstream>

//#include <boost/lexical_cast.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/serialization/serialization.hpp>
//#include <boost/serialization/nvp.hpp>
//#include <boost/serialization/vector.hpp>

#include "Rule.h"
#include "IndexFactory.h"
#include "MT19937.h"
#include "LogBox.h"

#include "Distance.hpp"

class VectorSizeSort
{
  public:
    bool
    operator()(const std::vector<Rule> left,
               const std::vector<Rule> right) const
    {
        return left.size() > right.size();
    }
};

class ExternalSizeSort
{
  public:
    bool
    operator()(const Rule left, const Rule right) const
    {
        return left.external.size() < right.external.size();
    }
};

class RuleCompositionSort
{
  public:
    bool
    operator()(const Rule left, const Rule right) const
    {
        return left.composition() > right.composition();
    }
};

class KnowledgeBaseTypeDef
{
  public:
    typedef std::vector<Rule> RuleDBType;
    typedef std::map<int, std::multimap<int, Rule>> DicDBType;
    typedef std::multimap<Element, Rule> NormalDicType;

    typedef std::vector<Rule> PatternType;
};

/*!
 * 知識集合を表すクラスです
 */
class KnowledgeBase : public KnowledgeBaseTypeDef, public RuleTypeDef
{
  public:
    enum PATTERN_TYPE
    {
        ABSOLUTE,
        COMPLETE,
        SEMICOMPLETE
    };
    IndexFactory cat_indexer;
    RuleDBType sbox_buffer;
    RuleDBType sentence_box;
    RuleDBType word_box;
    DicDBType word_dic;
    RuleDBType sentenceDB;
    RuleDBType wordDB;
    bool DIC_BLD;
    static bool LOGGING_FLAG;
    static int ABSENT_LIMIT;
    static uint32_t CONTROLS;
    static int buzz_length;
    static const uint32_t USE_OBLITERATION = 0x01;
    static const uint32_t USE_SEMICOMPLETE_FABRICATION = 0x02;
    static const uint32_t USE_ADDITION_OF_RANDOM_WORD = 0x04;
    static const uint32_t ANTECEDE_COMPOSITION = 0x08;
    static bool OMISSION_FLAG;
    static std::vector<Rule> MEANING_SPACE;
    /*!
     * 実行ログを取るようにします。
     */
    static void
    logging_on(void);
    /*!
     * ログの取得を停止します。
     */
    static void
    logging_off(void);
    static void
    omission_on(void);
    static void
    omission_off(void);
    static void
    set_control(uint32_t FLAGS);
    KnowledgeBase();
    ~KnowledgeBase();
    KnowledgeBase &
    operator=(const KnowledgeBase &dst);
    void
    clear(void);
    /*!
     * 知識集合の文字列表現を返します
     */
    std::string
    to_s(void);
    /*!
     * 知識に対して、Chunk、Merge、Replaceを実行します。
     * これはいずれのルールも適用不可能になるまで実行されます。
     */
    bool
    consolidate(void);
    /*!
     * Chunkを全てのルールに対して実行します。
     * ただし、Chunkが不可能になるまでChunkを繰り返すことを保証しません。
     */
    bool
    chunk(void); //チャンク
    /*!
     * Mergeを全てのルールに対して実行します。
     * ただし、Mergeが不可能になるまでMergeを繰り返すことを保証しません。
     */
    bool
    merge(void); //マージ
    /*!
     * Replaceを全てのルールに対して実行します。
     * ただし、Replaceが不可能になるまでReplaceを繰り返すことを保証しません。
     */
    bool
    replace(void); //リプレイス
    /*!
     * Ruleを受け取り、その内部言語列に対応する外部言語列を生成し、
     * その外部言語列をRuleに代入して返します。なお生成ルールは以下のようになります。
     * -# Ruleの内部言語列を構成可能で、合成度の高いルールを使用する
     * -# 合成度の高いルールで、内部言語1要素だけが適合しない場合、その1要素についてランダムの文字列を当てて外部言語列を生成する
     * -# 完全に内部言語が一致するルールを使用する
     * -# 上記に当てはまらない場合、指定された長さ以下のランダムの文字列を生成する
     * .
     *
     */
    Rule
    fabricate(Rule &src1);
    /*!
     * Ruleを受け取り、その内部言語列に対応する外部言語列を生成し、
     * その外部言語列をRuleに代入して返します。なお生成ルールは以下のようになります。
     * 正確な発話ができない場合の補完用
     * -# 合成度の高いルールで、内部言語1要素だけが適合しない場合、その1要素についてランダムの文字列を当てて外部言語列を生成する
     * .
     *
     */ 
    Rule
    fabricate_for_complementing(Rule &src1);
    /*!
     * 渡されたRuleの内部言語から完全に外部言語列を構成可能かどうかを返す。
     * 真なら構成可能、偽なら不可能
     */
    bool
    acceptable(Rule &src);
    /*!
     * Ruleを知識集合のメールボックスに送ります。Ruleはそのまま知識集合に格納されません。
     * それは、未処理のRuleだからです。Chunk、Merge、Replaceなどの処理が終わって始めて
     * 知識集合に格納されます。
     */
    void
    send_box(Rule &mail);
    void
    send_box(std::vector<Rule> &mails);
    /*!
     * Ruleを直接知識集合に追加します。
     */
    void
    send_db(Rule &mail);
    void
    send_db(std::vector<Rule> &mails);
    void
    ground_with_pattern(Rule &src, PatternType &pattern);
    std::vector<Rule>
    grounded_rules(Rule src);
    std::vector<Rule>
    rules(void);
    std::vector<Rule>
    utterances(void);
    /*!
     * 実行速度を上げるために、単語規則のハッシュを構成します。
     * これはfabricateが始めて実行されるときに自動的に呼び出されます。
     * 将来的にprivate
     */
    void
    build_word_index(void);
  private:
    bool
    clipping(Rule &mean, KnowledgeBase::PatternType &ptn, KnowledgeBase::PatternType &res);
    /*!
     * Ruleの内部言語列に対応する外部言語列を構成可能なとき、その構成に必要なRuleの集合を
     * 構成可能パターンとして返します。返値はMapクラスで、パターンのタイプをキーとして、パターンの集合が入っています。
     * パターンのタイプとは、
     * - 1:完全一致（変数がないRule）
     * - 2:完全構成（変数を含むRuleと単語Ruleの集合）
     * - 3:不完全構成（変数を含むRuleと、単語Rule、そしてランダム生成された単語規則）
     */
    std::map<PATTERN_TYPE, std::vector<PatternType>>
    construct_grounding_patterns(Rule &src);
    std::vector<std::vector<Element>>
    recognize_terminal_strings(Rule &target);
    std::vector<Rule>
    chunking(Rule &src, Rule &dst);
    bool
    chunking_loop(Rule &unchecked_sent, RuleDBType &checked_rules);
    bool
    merging(Rule &src);
    void collect_merge_cat(Rule &src, std::vector<Rule> &words, std::map<int, bool> &unified_cat);
    void
    merge_noun_proc(Rule &src, RuleDBType &DB,
                    std::map<int, bool> &unified_cat);
    RuleDBType
    merge_sent_proc(Rule &base_word, RuleDBType &DB,
                    std::map<int, bool> &unified_cat);
    bool
    replacing(Rule &word, RuleDBType &checking_sents);
    KnowledgeBase::ExType
    construct_buzz_word(void);
    void
    unique(RuleDBType &DB);
    std::string
    string_join(const std::vector<std::string> &str_v, const std::string &delim);
    /*!
     * 例外ルールである、単語削除を行います。
     * これは、単語規則について、内部言語が等しいものに対し、外部言語が最も短いものを残し、
     * その他を削除するルールです。
     */
    bool
    obliterate(void); //最短単語残す
};

#endif /* KNOWLEDGEBASE_H_ */
