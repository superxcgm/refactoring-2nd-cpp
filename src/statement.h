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

    class EnrichedPerformance : public Performance {
        Play play;
        int amount;
    public:
        EnrichedPerformance(const Performance &perf, const Play &play, int amount) : Performance(perf.GetPlayId(),
                                                                                                 perf.GetAudience()),
                                                                                     play(play), amount(amount) {}

        const Play &GetPlay() const {
            return play;
        }

        int GetAmount() const {
            return amount;
        }
    };

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

    int volumeCreditsFor(const Plays &plays, const EnrichedPerformance &perf) {
        int result = std::max(perf.GetAudience() - 30, 0);
        if (Play::PlayType::COMEDY == perf.GetPlay().GetType())
            result += floor(perf.GetAudience() / 5);
        return result;
    }

    std::string Usd(int amount) {
        std::stringstream ss;
        ss.imbue(std::locale("en_US.UTF-8"));
        ss << "$" << std::fixed << std::setprecision(2) << amount / 100;  // $1,234.56
        return ss.str();
    }

    int TotalVolumeCredits(const std::vector<EnrichedPerformance> &performances, const Plays &plays) {
        int result = 0;
        for (const auto &perf: performances) {
            result += volumeCreditsFor(plays, perf);
        }
        return result;
    }

    int TotalAmount(const std::vector<statement::EnrichedPerformance> &performances, const Plays &plays) {
        int total_amount = 0;
        for (const auto &perf: performances) {
            total_amount += AmountFor(perf, perf.GetPlay());
        }
        return total_amount;
    }

    struct StatementData {
        std::string customer;
        std::vector<EnrichedPerformance> performances;
        int total_amount;
    };

    std::string RenderPlainText(const Plays &plays, const StatementData &data) {
        std::string result = "Statement for " + data.customer + "\n";

        for (const auto &perf: data.performances) {
            // print line for this order
            result += "  " + perf.GetPlay().GetName() + ": " + Usd(perf.GetAmount()) +
                      " (" + std::to_string(perf.GetAudience()) + " seats)\n";
        }

        result += "Amount owed is " + Usd(data.total_amount) + "\n";
        result += "You earned " + std::to_string(TotalVolumeCredits(data.performances, plays)) + " credits\n";
        return result;
    }

    std::vector<EnrichedPerformance> EnrichPerformances(const Invoice &invoice, const Plays &plays) {
        auto origin_performances = invoice.GetPerformances();
        std::vector<EnrichedPerformance> enriched_performances;
        enriched_performances.reserve(origin_performances.size());
        for (const auto &perf: origin_performances) {
            const Play &play = PlayFor(plays, perf);
            EnrichedPerformance enriched_performance{perf, play, AmountFor(perf, play)};
            enriched_performances.push_back(enriched_performance);
        }
        return enriched_performances;
    }

    std::string Statement(const Invoice &invoice, const Plays &plays) {
        StatementData statement_data;
        statement_data.customer = invoice.GetCustomer();
        statement_data.performances = EnrichPerformances(invoice, plays);
        statement_data.total_amount = TotalAmount(statement_data.performances, plays);
        return RenderPlainText(plays, statement_data);
    }
}


#endif //SRC_STATEMENT_H_
