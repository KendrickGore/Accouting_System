#ifndef FINANCE_ADVISOR_H
#define FINANCE_ADVISOR_H

#include <string>
#include <vector>
#include "Record.h"

class FinanceAdvisor {
public:
    FinanceAdvisor();

    std::string suggestPlan(const std::vector<Record>& records);
};

#endif  // FINANCE_ADVISOR_H