#ifndef SRC_PERFORMANCE_H_
#define SRC_PERFORMANCE_H_

#include <string>
#include <utility>

class Performance {
    std::string playID;
    int audience;
public:

    Performance(std::string play_id, int audience) : playID(std::move(play_id)), audience(audience) {}

    const std::string &GetPlayId() const {
        return playID;
    }

    int GetAudience() const {
        return audience;
    }
};

#endif //SRC_PERFORMANCE_H_
