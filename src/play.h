#ifndef SRC_PLAY_H_
#define SRC_PLAY_H_

#include <string>
#include <cstdint>
#include <utility>

class Play {
public:
    enum class PlayType: int8_t {
        TRAGEDY,
        COMEDY
    };

    Play(std::string name, PlayType type) : name(std::move(name)), type(type) {}

    PlayType GetType() const {
        return type;
    }

    const std::string &GetName() const {
        return name;
    }

private:
    std::string name;
    PlayType type;
};

#endif //SRC_PLAY_H_
