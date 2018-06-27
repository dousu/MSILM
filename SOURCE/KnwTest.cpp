#include "KnowledgeBase.h"

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

int main(int arg, char **argv)
{
    std::vector<Rule> meanings;
    std::vector<Element> symbols;
    Dictionary dic;
    dic.load(std::string("./SOURCE/data.dic"));
    construct_meanings(meanings);
    construct_individuals(symbols, dic);
    Rule buf;
    KnowledgeBase kb;
    std::vector<Rule> vec;

    //dictionary check
    std::cout << "Dictionary" << std::endl;
    std::cout << "meaning" << std::endl;
    auto it1 = std::begin(dic.conv_individual);
    for(;it1 != std::end(dic.conv_individual);it1++){
    	std::cout << "\tstring: " << (*it1).first << "\tnumber: " << (*it1).second << std::endl;
    }
    std::cout << "symbols" << std::endl;
    auto it2 = std::begin(dic.conv_symbol);
    for(;it2 != std::end(dic.conv_symbol);it2++){
    	std::cout << "\tstring: " << (*it2).first << "\tnumber: " << (*it2).second << std::endl;
    }

    //chunk1 test
    std::cout << "\n****************chunk1 test" << std::endl;
    vec.push_back(Rule(std::string("S/like(mary,john)->abc")));
    vec.push_back(Rule(std::string("S/like(mary,mary)->adc")));

    kb.send_box(vec);
    std::cout << kb.to_s() << std::endl;

    kb.chunk();
    std::cout << kb.to_s() << std::endl;

    //chunk2 test
    std::cout << "\n****************chunk2 test" << std::endl;
    vec.clear();
    vec.push_back(Rule(std::string("S/like(mary,heather)->agc")));
    kb.send_box(vec);
    kb.chunk();
    std::cout << kb.to_s() << std::endl;

    //merge test
    std::cout << "\n****************merge test" << std::endl;
    std::cout << "\n%%% previous" << std::endl;
    std::cout << kb.to_s() << std::endl;
    buf = Rule(std::string("C3/mary->d"));
    kb.word_box.insert(kb.word_box.begin(), buf);
    kb.merge();
    std::cout << "\n%%% after" << std::endl;
    std::cout << kb.to_s() << std::endl;

    //replace test
    std::cout << "\n****************replace test" << std::endl;
    std::cout << "\n%%% previoud" << std::endl;
    std::cout << kb.to_s() << std::endl;
    buf = Rule(std::string("C4/mary->c"));
    kb.word_box.insert(kb.word_box.begin(), buf);
    kb.replace();
    std::cout << "\n%%% after" << std::endl;
    std::cout << kb.to_s() << std::endl;

    //consolidate test
    std::cout << "\n****************consolidate test" << std::endl;
    kb.clear();vec.clear();
    vec.push_back(Rule(std::string("S/like(mary,heather)->abc")));
    vec.push_back(Rule(std::string("S/like(mary,john)->adc")));
    vec.push_back(Rule(std::string("S/like(john,john)->add")));
    vec.push_back(Rule(std::string("C2/john->d")));
    vec.push_back(Rule(std::string("C4/mary->c")));
    kb.send_box(vec);

    std::cout << "\n%%% previoud" << std::endl;
    std::cout << kb.to_s() << std::endl;
    kb.consolidate();
    std::cout << "\n%%% after" << std::endl;
    std::cout << kb.to_s() << std::endl;

    //fabricate test
    std::cout << "\n****************fabricate test" << std::endl;
    Rule input = Rule(std::string("S/like(heather,heather)->z")), output;
    input.external.clear();
    output = kb.fabricate(input);
    std::cout << "fabricated: " << output.to_s() << std::endl;
    std::cout << "\n****************end" << std::endl;

    return 0;
}
