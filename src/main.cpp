#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"
#include "cStrategy.h"





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
        mySR.combine();
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
