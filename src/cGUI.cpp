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
          {50, 50, 1200, 500}),
      lb(wex::maker::make<wex::multiline>(fm))
{
    constructMenu();

    lb.move(10, 10, 1100, 450);
    lb.fontHeight(18);
    lb.fontName("courier");
    lb.text("");

    show();
    run();
}

void cGUI::constructMenu()
{
    wex::menubar mbar(fm);
    wex::menu mfile(fm);
    mfile.append("New",
                 [&](const std::string &title)
                 {
                     mySR.clear();
                     fm.update();
                 });
    mfile.append("Open",
                 [&](const std::string &title)
                 {
                     wex::filebox fb(fm);

                     try
                     {
                         mySR.read(fb.open());
                         mySR.combine();
                         lb.text(mySR.textSummary());
                         fm.update();
                     }
                     catch (std::runtime_error &e)
                     {
                         wex::msgbox m(e.what());
                         return;
                     }
                 });

    mbar.append("File", mfile);
}