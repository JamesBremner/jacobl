#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "cStrategy.h"

std::vector<cStrategy> cStrategy::theStrategy;

void cStrategy::clear()
{
    theStrategy.clear();
}

void cStrategy::read(const std::string &fname)
{
    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open " + fname);

    if (theStrategy.size() > 3)
        throw std::runtime_error(
            " !!!!!!!!!!! Evaluation limit reached  !!!!!!!!!!!!");

    theStrategy.push_back(cStrategy());

    std::string line;
    getline(ifs, line);
    theStrategy.back().myName = line;
    while (getline(ifs, line))
    {
        theStrategy.back().parse(line);
    }
    theStrategy.back().rankCalc();
}

void cStrategy::parse(const std::string &line)
{
    std::string date, result;
    int p = line.find(",");
    date = line.substr(0, p);
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
    sResult R;
    R.sdate = date;

    R.ymd = atoi(date.c_str()) * 100;
    int p = date.find("/");
    if (p == -1)
        return;
    R.ymd += atoi(date.substr(p + 1).c_str());
    p = date.find("/", p + 1);
    if (p == -1)
        return;
    R.ymd += atoi(date.substr(p + 1).c_str()) * 10000;

    R.profit = value;

    if (!myResult.size())
    {
        // first record
        myResult.push_back(R);
        return;
    }
    auto &prev = myResult.back();
    if (date != prev.sdate)
        // new day
        myResult.push_back(R);
    else
        // same day has previous record
        prev.profit += value;
}

void cStrategy::rankCalc()
{
    myMaxLoss = 0;
    myProfit = 0;
    for (auto &r : myResult)
    {
        // profit is the total of profits and losses for every day
        myProfit += r.profit;

        // max loss is the largest loss on any one day
        if (r.profit < myMaxLoss)
            myMaxLoss = r.profit;
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
    std::vector<std::pair<double, int > > vpair;
    int i = 0;
    for( auto& st : theStrategy)
        vpair.push_back( std::make_pair( st.rank(), i++ ));

    std::sort(
        vpair.begin(),
        vpair.end(),
        [](
            const std::pair<double, int > &a,
            const std::pair<double, int > &b)
        {
            return a.first > b.first;
        });

    theRankOrder.clear();
    for( auto& p : vpair )
        theRankOrder.push_back( p.second );

}

void cStrategy::combine(const cStrategy &other)
{
    if (myName.empty())
    {
        // first strategy in combination
        *this = other;
        return;
    }

    // combine names
    myName += "+" + other.myName;

    // temp storage of combined results
    // ( so insertions do not invalidate iterators )
    std::vector<sResult> comb;

    // loop while uncombine results remain
    int I = 0;
    int otherI = 0;
    while (1)
    {
        if (myResult[I].ymd == other.myResult[otherI].ymd)
        {
            // same day - sum profits

            myResult[I].profit += other.myResult[otherI].profit;
            comb.push_back(myResult[I]);

            I++;
            otherI++;
        }
        else if (myResult[I].ymd < other.myResult[otherI].ymd)
        {
            // no date match - insert from other
            comb.push_back(myResult[I]);
            I++;
        }
        else
        {
            // no date match - insert from this
            comb.push_back(other.myResult[otherI]);
            otherI++;
        }
        if (I == myResult.size())
        {
            // this exhausted - add remaining other
            for (int k = otherI; k < other.myResult.size(); k++)
                comb.push_back(other.myResult[k]);
            break;
        }
        if (otherI == other.myResult.size())
        {
            // other exhausted - add remaining this
            for (int k = I; k < other.myResult.size(); k++)
                comb.push_back(other.myResult[k]);
            break;
        }
    }

    // update with combined results
    myResult = comb;
}

void cStrategy::combine()
{
    // store new combinations
    std::vector<cStrategy> newComb;

    // loop over previous strategies
    for (int k = 0; k < theStrategy.size() - 1; k++)
    {
        // new combination, starting with prevous strategy
        cStrategy comb = theStrategy[k];

        // combine previous and latest
        comb.combine(theStrategy.back());

        // calculate rank
        comb.rankCalc();

        // add to new combinations
        newComb.push_back(comb);
    }

    // add new combinations to strategies
    theStrategy.insert(
        theStrategy.end(),
        newComb.begin(), newComb.end());

    // sort into rank order
    rankOrder();
}

std::string cStrategy::text() const
{
    std::stringstream ss;
    for (auto &S : theStrategy)
    {

        ss << S.myName << "\r\n";
        for (auto &p : S.myResult)
        {
            ss << p.sdate << "  |  " << p.profit << "\r\n";
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
    for (int i : theRankOrder)
    {
        auto& S = theStrategy[i];
        ss << "Profit " << std::setw(10) << S.myProfit << ", ";
        ss << "Max Loss " << std::setw(10) << S.myMaxLoss << ", ";
        ss << "Rank " << std::setw(10) << S.myRank << "  ";
        ss << S.myName << "\r\n";
    }
    return ss.str();
}