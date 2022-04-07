#ifndef SRC_STATEMENT_H_
#define SRC_STATEMENT_H_

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <map>

#include "invoice.h"
#include "play.h"

namespace statement {
    using PlayID = std::string;
    using Plays = std::map<PlayID, Play>;

    int AmountFor(const Performance &perf, const Play &play) {
        int result = 0;

        switch (play.GetType()) {
            case Play::PlayType::TRAGEDY:
                result = 40000;
                if (perf.GetAudience() > 30) {
                    result += 1000 * (perf.GetAudience() - 30);
                }
                break;
            case Play::PlayType::COMEDY:
                result = 30000;
                if (perf.GetAudience() > 20) {
                    result += 10000 + 500 * (perf.GetAudience() - 20);
                }
                result += 300 * perf.GetAudience();
                break;
        }
        return result;
    }

    const Play &PlayFor(const Plays &plays, const Performance &perf) { return plays.at(perf.GetPlayId()); }

    int volumeCreditsFor(const Plays &plays, const Performance &perf) {
        int result = std::max(perf.GetAudience() - 30, 0);
        if (Play::PlayType::COMEDY == PlayFor(plays,
                                              perf).GetType()) result += floor(perf.GetAudience() / 5);
        return result;
    }

    std::string Format(int amount) {
        std::stringstream ss;
        ss.imbue(std::locale("en_US.UTF-8"));
        ss << "$" << std::fixed << std::setprecision(2) << amount;  // $1,234.56
        return ss.str();
    }

    std::string Statement(const Invoice &invoice, const Plays &plays) {
        int total_amount = 0;
        int volume_credits = 0;
        std::string result = "Statement for " + invoice.GetCustomer() + "\n";

        for (const auto& perf: invoice.GetPerformances()) {
            volume_credits += volumeCreditsFor(plays, perf);

            // print line for this order
            result += "  " + PlayFor(plays, perf).GetName() + ": " + Format(
                    AmountFor(perf, PlayFor(plays, perf)) / 100) + " (" +
                      std::to_string(perf.GetAudience()) + " seats)\n";
            total_amount += AmountFor(perf, PlayFor(plays, perf));
        }
        result += "Amount owed is " + Format(total_amount / 100) + "\n";
        result += "You earned " + std::to_string(volume_credits) + " credits\n";
        return result;
    }
}


#endif //SRC_STATEMENT_H_
