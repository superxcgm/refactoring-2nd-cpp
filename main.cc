#include <iostream>

#include "src/statement.h"

int main() {
    // invoice and plays should be load from json file, but json lib too cumbersome, so I create it manually
    Invoice invoice{
        "BigCo", {
                    Performance{"halmet", 55},
                    {"as-like", 35},
                    {"othello", 40}
        }
    };

    statement::Plays plays{
            {"halmet", Play{"Halmet", Play::PlayType::TRAGEDY}},
            {"as-like", Play{"As You Like It", Play::PlayType::COMEDY}},
            {"othello", Play{"Othello", Play::PlayType::TRAGEDY}},
    };

    std::cout << statement::Statement(invoice, plays);
    return 0;
}