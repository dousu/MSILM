#include "KnowledgeBase.h"

int main(int arg, char **argv)
{
    Dictionary dic;
    dic.load(std::string("./SOURCE/data.dic"));
    Rule buf;
    KnowledgeBase kb;
    std::vector<Rule> vec;

    //dictionary check
    std::cout << "Dictionary" << std::endl;
    std::cout << "meaning" << std::endl;
    auto it1 = std::begin(dic.conv_individual);
    for (; it1 != std::end(dic.conv_individual); it1++)
    {
        std::cout << "\tstring: " << (*it1).first << "\tnumber: " << (*it1).second << std::endl;
    }
    std::cout << "symbols" << std::endl;
    auto it2 = std::begin(dic.conv_symbol);
    for (; it2 != std::end(dic.conv_symbol); it2++)
    {
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
    buf = Rule(std::string("C3/mary->d"));
    kb.word_box.insert(kb.word_box.begin(), buf);
    std::cout << kb.to_s() << std::endl;
    kb.merge();
    std::cout << "\n%%% after" << std::endl;
    std::cout << kb.to_s() << std::endl;

    //replace test
    std::cout << "\n****************replace test" << std::endl;
    std::cout << "\n%%% previous" << std::endl;
    buf = Rule(std::string("C4/mary->c"));
    kb.word_box.insert(kb.word_box.begin(), buf);
    std::cout << kb.to_s() << std::endl;
    kb.replace();
    std::cout << "\n%%% after" << std::endl;
    std::cout << kb.to_s() << std::endl;

    //consolidate test
    std::cout << "\n****************consolidate test" << std::endl;
    kb.clear();
    vec.clear();
    vec.push_back(Rule(std::string("S/like(mary,heather)->abc")));
    vec.push_back(Rule(std::string("S/like(mary,john)->adc")));
    vec.push_back(Rule(std::string("S/like(john,john)->add")));
    vec.push_back(Rule(std::string("C2/john->d")));
    vec.push_back(Rule(std::string("C4/mary->c")));
    kb.send_box(vec);

    std::cout << "\n%%% previous" << std::endl;
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

    //acceptable test
    std::cout << "\n****************acceptable test" << std::endl;
    Rule query = Rule(std::string("S/like(heather,heather)->zxy"));
    query.external.clear();
    bool accepted = kb.acceptable(query);
    std::cout << "Target: " << query.to_s() << std::endl
              << "result: " << std::boolalpha << accepted << std::noboolalpha << std::endl;
    query = Rule(std::string("S/like(john,mary)->yzz"));
    query.external.clear();
    accepted = kb.acceptable(query);
    std::cout << "Target: " << query.to_s() << std::endl
              << "result: " << std::boolalpha << accepted << std::noboolalpha << std::endl;
    query = Rule(std::string("S/like(mary,mary)->zyx"));
    query.external.clear();
    accepted = kb.acceptable(query);
    std::cout << "Target: " << query.to_s() << std::endl
              << "result: " << std::boolalpha << accepted << std::noboolalpha << std::endl;
    std::cout << "\n****************end" << std::endl;

    return 0;
}
