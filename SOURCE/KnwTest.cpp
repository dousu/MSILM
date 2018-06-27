#include "KnowledgeBase.h"

int main(int arg, char **argv)
{
    Element::load_dictionary((char *)"./SOURCE/data.dic");
    Rule buf;
    KnowledgeBase kb;
    std::vector<Rule> vec;

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
    std::cout << "\n%%% previoud" << std::endl;
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
    kb.clear();
    vec.push_back(Rule(std::string("S/like(mary,heather)->abc")));
    vec.push_back(Rule(std::string("S/like(mary,john)->adc")));
    vec.push_back(Rule(std::string("S/like(john,john)->add")));
    vec.push_back(Rule(std::string("C2/john -> d")));
    vec.push_back(Rule(std::string("C4/mary -> c")));
    kb.send_box(vec);

    std::cout << "\n%%% previoud" << std::endl;
    std::cout << kb.to_s() << std::endl;
    kb.consolidate();
    std::cout << "\n%%% after" << std::endl;
    std::cout << kb.to_s() << std::endl;

    //build index test
    std::cout << "\n****************build index test" << std::endl;
    std::map<unsigned int, std::multimap<unsigned int, Rule>>::iterator dit;
    std::multimap<unsigned int, Rule>::iterator item_it;
    kb.build_word_index();

    dit = kb.word_dic.begin();
    while (dit != kb.word_dic.end())
    {
        std::cout << "\nNOW... C:" << (*dit).first << std::endl;
        item_it = (*dit).second.begin();
        while (item_it != (*dit).second.end())
        {
            std::cout << "\tind: " << Element::dic.individual[(*item_it).first] << std::endl;
            std::cout << "\trule: " << (*item_it).second.to_s() << std::endl;
            item_it++;
        }
        dit++;
    }

    //fabricate test
    std::cout << "\n****************fabricate test" << std::endl;
    Rule input = Rule(std::string("S hoge hoge hoge -> d")), output;
    input.external.clear();
    output = kb.fabricate(input1);
    std::cout << "fabr: " << output.to_s() << std::endl;
    std::cout << "\n****************end" << std::endl;

    return 0;
}