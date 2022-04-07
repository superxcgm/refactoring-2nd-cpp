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

    std::string Usd(int amount) {
        std::stringstream ss;
        ss.imbue(std::locale("en_US.UTF-8"));
        ss << "$" << std::fixed << std::setprecision(2) << amount / 100;  // $1,234.56
        return ss.str();
    }

    int TotalVolumeCredits(const Invoice &invoice, const Plays &plays) {
        int result = 0;
        for (const auto& perf: invoice.GetPerformances()) {
            result += volumeCreditsFor(plays, perf);
        }
        return result;
    }

    int TotalAmount(const Invoice &invoice, const Plays &plays) {
        int total_amount = 0;
        for (const auto& perf: invoice.GetPerformances()) {
            total_amount += AmountFor(perf, PlayFor(plays, perf));
        }
        return total_amount;
    }

    struct StatementData {

    };

    std::string RenderPlainText(const Invoice &invoice, const Plays &plays, const StatementData data) {
        std::string result = "Statement for " + invoice.GetCustomer() + "\n";

        for (const auto& perf: invoice.GetPerformances()) {
            // print line for this order
            result += "  " + PlayFor(plays, perf).GetName() + ": " + Usd(
                    AmountFor(perf, PlayFor(plays, perf))) + " (" +
                      std::to_string(perf.GetAudience()) + " seats)\n";
        }

        result += "Amount owed is " + Usd(TotalAmount(invoice, plays)) + "\n";
        result += "You earned " + std::to_string(TotalVolumeCredits(invoice, plays)) + " credits\n";
        return result;
    }

    std::string Statement(const Invoice &invoice, const Plays &plays) {
        StatementData statement_data;
        return RenderPlainText(invoice, plays, statement_data);
    }
}


#endif //SRC_STATEMENT_H_
