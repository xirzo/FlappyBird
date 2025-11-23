#ifndef FLAPPYBIRD_GAME_H
#define FLAPPYBIRD_GAME_H

#include "global.h"

namespace fb {
    void init(const game_state_t &state);

    bool should_close();

    void process_input();

    void draw(const game_state_t &state);

    void update(game_state_t &state);

    void deinit();
}

#endif //FLAPPYBIRD_GAME_H
