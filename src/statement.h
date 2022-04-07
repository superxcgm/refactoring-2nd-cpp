#ifndef SRC_STATEMENT_H_
#define SRC_STATEMENT_H_

#include "create_statement_data.h"


namespace statement {
    std::string Usd(int amount) {
        std::stringstream ss;
        ss.imbue(std::locale("en_US.UTF-8"));
        ss << "$" << std::fixed << std::setprecision(2) << amount / 100;  // $1,234.56
        return ss.str();
    }

    std::string RenderPlainText(const Plays &plays, const StatementData &data) {
        std::string result = "Statement for " + data.customer + "\n";

        for (const auto &perf: data.performances) {
            // print line for this order
            result += "  " + perf.GetPlay().GetName() + ": " + Usd(perf.GetAmount()) +
                      " (" + std::to_string(perf.GetAudience()) + " seats)\n";
        }

        result += "Amount owed is " + Usd(data.total_amount) + "\n";
        result += "You earned " + std::to_string(data.total_volume_credit) + " credits\n";
        return result;
    }

    std::string Statement(const Invoice &invoice, const Plays &plays) {
        return RenderPlainText(plays, CreateStatementData(invoice, plays));
    }
}


#endif //SRC_STATEMENT_H_
