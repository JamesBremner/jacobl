#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "cStrategy.h"
#include <wex.h>
#include "cGUI.h"

#include "commctrl.h"

// Not sure what this does.  More info at https://stackoverflow.com/a/33125299/16582
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

cGUI::cGUI()
    : cStarterGUI(
          "Jacob's Strategy Combiner : Evaluation Version",
          {50, 50, 1200, 500}),
      dropper(wex::maker::make<wex::drop>(fm)),
      droplabel(wex::maker::make<wex::label>(dropper)),
      lb(wex::maker::make<wex::multiline>(fm)),
      lbDateRange(wex::maker::make<wex::label>(fm)),
      lbDateRange2(wex::maker::make<wex::label>(fm)),
      dbDateStart(wex::maker::make<wex::datebox>(fm)),
      dbDateEnd(wex::maker::make<wex::datebox>(fm))
{
    constructMenu();
    constructDatePicker();

    // widget for receiving dropped files
    dropper.move(10, 10, 490, 70);
    droplabel.move(30, 30, 400, 50);
    droplabel.fontHeight(28);
    droplabel.text("Drop strategy files here");

    // lbDateRange.move(550, 30, 100, 30);
    // lbDateRange.text("Dates");
    // // ebDateRange.move(665, 30, 200, 30);
    // // ebDateRange.text("ALL");
    // dbDateStart.move(665, 30, 200, 30);
    // dbDateStart.text("Picker");

    // strategy display
    lb.move(10, 100, 1100, 450);
    lb.fontHeight(18);
    lb.fontName("courier");
    lb.text("");

    // dropped files event handler
    dropper.events().drop(
        [&](const std::vector<std::string> &files)
        {
            // loop over dropped files
            for (auto &f : files)
            {
                // read file
                mySR.read(f);

                // combine with previous
                mySR.combine();
            }
            lb.text(mySR.textSummary());
            fm.update();
        });

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
                         for (auto &f : fb.openMulti())
                         {
                             // read file
                             mySR.read(f);

                             // combine with previous
                             mySR.combine();
                         }
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

void cGUI::constructDatePicker()
{
    int x = 500;
    lbDateRange.move(x, 30, 100, 30);
    lbDateRange.text("Dates from");

    x += 120;
    dbDateStart.move(x, 30, 130, 30);

    x += 180;
    lbDateRange2.move(x, 30, 50, 30);
    lbDateRange2.text("to");

    x += 70;
    dbDateEnd.move(x, 30, 130, 30);

    fm.events().datePick(
        [this](int id, LPNMDATETIMECHANGE date)
        {
            std::string which;
            if (id == dbDateStart.id())
                which = "start";
            else
                which = "end";

            if (date->dwFlags == GDT_NONE)
            {
                std::cout << which << " date disabled\n";
                return;
            }
            std::cout << which << " date changed to "
                      << date->st.wYear << "/"
                      << date->st.wMonth << "/"
                      << date->st.wDay << "\n";
        });
}