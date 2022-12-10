#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"

class cStrategyResults
{
public:
    void read(const std::string &fname);

    std::string text() const;

private:
    std::string myName;
    std::vector<std::pair<std::string, float>> myResult;

    void parse(const std::string &line);

    void add(
        const std::string &date,
        float value);
};

void cStrategyResults::read(const std::string &fname)
{
    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open " + fname);
    std::string line;
    getline(ifs, line);
    myName = line;
    while (getline(ifs, line))
    {
        parse(line);
    }
}
void cStrategyResults::parse(const std::string &line)
{
    std::string date, result;
    int p = line.find(",");
    date = line.substr(0, p - 1);
    result = line.substr(p + 1);
    p = result.find_first_not_of("$ ");
    result = result.substr(p);
    if (result[0] == '(')
        result[0] = '-';
    float value = atof(result.c_str());

    add(date, value);

    // std::cout << date << "|" << result << "\n";
    // std::cout << date << " " << value << "\n";
}

void cStrategyResults::add(
    const std::string &date,
    float value)
{
    if (!myResult.size())
    {
        myResult.push_back(std::make_pair(
            date, value));
        return;
    }
    auto &prev = myResult.back();
    if (date != prev.first)

        myResult.push_back(std::make_pair(
            date, value));
    else
        prev.second += value;
}

std::string cStrategyResults::text() const
{
    std::stringstream ss;
    ss << myName << "\r\n";
    for (auto &p : myResult)
    {
        ss << p.first << "  |  " << p.second << "\r\n";
    }
    return ss.str();
}

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Jacob's Strategy Combiner",
              {50, 50, 1000, 500}),
          lb(wex::maker::make<wex::multiline>(fm))
    {
        mySR.read("../dat/StrategyB.csv");
        std::cout << mySR.text();

        lb.move(10, 10, 900, 450);
        lb.text(mySR.text());
        lb.fontHeight(20);

        show();
        run();
    }

private:
    wex::multiline &lb;
    cStrategyResults mySR;
};

main()
{
    cGUI theGUI;
    return 0;
}
