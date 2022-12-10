#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"
#include "cStategy.h"

std::vector<cStrategy> cStrategy::theStrategy;

void cStrategy::read(const std::string &fname)
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
    rankCalc();
    theStrategy.push_back(*this);
    rankOrder();
}

void cStrategy::rankCalc()
{
    myMaxLoss = 0;
    myProfit = 0;
    for (auto &r : myResult)
    {
        // profit is the total of profits and losses for every day
        myProfit += r.second;

        // max loss is the largest loss on any one day
        if (r.second < myMaxLoss)
            myMaxLoss = r.second;
    }

    // rank is total profit if no loss occurred on any one day
    // if losses do occurr,
    //    rank is the total profit divided by the absolute value
    //    of the largest loss on a single day
    myRank = myProfit;
    if (myMaxLoss < 0)
        myRank /= -myMaxLoss;
}
void cStrategy::rankOrder()
{
    std::sort(
        theStrategy.begin(),
        theStrategy.end(),
        [](
            const cStrategy &a,
            const cStrategy &b)
        {
            return a.rank() < b.rank();
        });
}

void cStrategy::parse(const std::string &line)
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

void cStrategy::add(
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

std::string cStrategy::text() const
{
    std::stringstream ss;
    for (auto &S : theStrategy)
    {

        ss << S.myName << "\r\n";
        for (auto &p : S.myResult)
        {
            ss << p.first << "  |  " << p.second << "\r\n";
        }

        ss << "Profit " << S.myProfit << "\r\n";
        ss << "Max Loss " << S.myMaxLoss << "\r\n";
        ss << "Rank " << S.myRank << "\r\n\r\n";
    }
    return ss.str();
}

std::string cStrategy::textSummary() const
{

    std::stringstream ss;
    for (auto &S : theStrategy)
    {
        ss << S.myName << "\r\n";
        ss << "Profit " << S.myProfit << "\r\n";
        ss << "Max Loss " << S.myMaxLoss << "\r\n";
        ss << "Rank " << S.myRank << "\r\n\r\n";
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
        mySR.read("../dat/StrategyA.csv");
        mySR.read("../dat/StrategyB.csv");
        std::cout << mySR.text();

        lb.move(10, 10, 900, 450);
        lb.text(mySR.textSummary());
        lb.fontHeight(20);

        show();
        run();
    }

private:
    wex::multiline &lb;
    cStrategy mySR;
};

main()
{
    cGUI theGUI;
    return 0;
}
