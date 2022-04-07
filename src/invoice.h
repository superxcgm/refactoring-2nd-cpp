#ifndef SRC_INVOICE_H_
#define SRC_INVOICE_H_

#include <string>
#include <utility>
#include <vector>

#include "performance.h"

class Invoice {
    std::string customer;
    std::vector<Performance> performances;

public:

    Invoice(std::string customer, std::vector<Performance> performances) : customer(std::move(customer)),
                                                                           performances(std::move(performances)) {}

    const std::string &GetCustomer() const {
        return customer;
    }

    const std::vector<Performance> &GetPerformances() const {
        return performances;
    }
};


#endif //SRC_INVOICE_H_
