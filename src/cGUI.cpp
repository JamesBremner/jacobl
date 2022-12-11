    #include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "cStrategy.h"
#include <wex.h>
#include "cGUI.h"

    cGUI::cGUI()
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