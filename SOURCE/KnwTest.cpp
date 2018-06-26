#include "KnowledgeBase.h"

int main(int arg, char **argv)
{
    Element::load_dictionary((char *)"data.dic");
    Rule buf;
    KnowledgeBase kb;
    std::vector<Rule> vec;

    //chunk1 test
    std::cout << "\n****************chunk1 test" << std::endl;
    vec.push_back(Rule(std::string("S hoge hoge foo -> a b c")));
    vec.push_back(Rule(std::string("S hoge hoge hoge -> a d c")));

    kb.send_box(vec);
    std::cout << kb.to_s() << std::endl;

    kb.chunk();
    std::cout << kb.to_s() << std::endl;

    //chunk2 test
    std::cout << "\n****************chunk2 test" << std::endl;
    vec.clear();
    vec.push_back(Rule(std::string("S hoge hoge foo -> a g c")));
    kb.send_box(vec);
    kb.chunk();
    std::cout << kb.to_s() << std::endl;

    //merge test
    std::cout << "\n****************merge test" << std::endl;
    std::cout << "\n%%% previoud" << std::endl;
    std::cout << kb.to_s() << std::endl;
    buf = Rule(std::string("C:2 hoge -> d"));
    kb.word_box.insert(kb.word_box.begin(), buf);
    kb.merge();
    std::cout << "\n%%% after" << std::endl;
    std::cout << kb.to_s() << std::endl;

    //replace test
    std::cout << "\n****************replace test" << std::endl;
    std::cout << "\n%%% previoud" << std::endl;
    std::cout << kb.to_s() << std::endl;
    buf = Rule(std::string("C:3 hoge -> c"));
    kb.word_box.insert(kb.word_box.begin(), buf);
    kb.replace();
    std::cout << "\n%%% after" << std::endl;
    std::cout << kb.to_s() << std::endl;

    //consolidate test
    std::cout << "\n****************consolidate test" << std::endl;
    vec.push_back(Rule(std::string("S hoge hoge foo -> a b c")));
    vec.push_back(Rule(std::string("S hoge hoge hoge -> a d c")));
    vec.push_back(Rule(std::string("C:2 hoge -> d")));
    vec.push_back(Rule(std::string("C:4 hoge -> c")));
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
            std::cout << "ind: " << Element::dic.individual[(*item_it).first] << std::endl;
            std::cout << "rule: " << (*item_it).second.to_s() << std::endl;
            item_it++;
        }
        dit++;
    }

    //fabricate test
    std::cout << "\n****************fabricate test" << std::endl;
    KnowledgeBase kb2;
    Rule input1, input2;
    try
    {
        //kb2.set_seed(11111111);
        MT19937::set_seed(11111111);
        input1 = Rule(std::string("S hoge hoge hoge -> d"));
    }
    catch (const char *msg)
    {
        std::cout << "ERR:" << msg << std::endl;
        throw;
    }
    std::cout << "prev: " << input1.to_s() << std::endl;

    kb2.fabricate(input1);
    std::cout << "fabr: " << input1.to_s() << std::endl;

    input2 = Rule(std::string("S hoge hoge hoge -> d"));
    input2.external.clear();
    kb.fabricate(input2);
    std::cout << "fabr: " << input2.to_s() << std::endl;
    std::cout << "\n****************end" << std::endl;

    //parse test

    return 0;
}