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

    std::string Statement(const Invoice &invoice, const Plays &plays) {
        int total_amount = 0;
        int volume_credits = 0;
        std::string result = "Statement for " + invoice.GetCustomer() + "\n";
        auto format = [](int amount) {
            std::stringstream ss;
            ss.imbue(std::locale("en_US.UTF-8"));
            ss << "$" << std::fixed << std::setprecision(2) << amount;  // $1,234.56
            return ss.str();
        };

        for (const auto& perf: invoice.GetPerformances()) {
            const auto &play = plays.at(perf.GetPlayId());
            int this_amount = AmountFor(perf, play);

            // add volume credits
            volume_credits += std::max(perf.GetAudience() - 30, 0);
            // add extra credit for every ten comedy attendees
            if (Play::PlayType::COMEDY == play.GetType()) volume_credits += std::floor(perf.GetAudience() / 5);

            // print line for this order
            result += "  " + play.GetName() + ": " + format(this_amount / 100) + " (" +
                      std::to_string(perf.GetAudience()) + " seats)\n";
            total_amount += this_amount;
        }
        result += "Amount owed is " + format(total_amount / 100) + "\n";
        result += "You earned " + std::to_string(volume_credits) + " credits\n";
        return result;
    }
}


#endif //SRC_STATEMENT_H_
