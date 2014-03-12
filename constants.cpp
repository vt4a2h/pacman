namespace {
    constexpr size_t H_CELL_COUNT = 21;
    constexpr size_t V_CELL_COUNT = 21;
    constexpr size_t CELL_WIDTH  = 20;
    constexpr size_t CELL_HEIGHT = 20;

    constexpr int PACMAN_UPTIME = 200; // msec.
    constexpr int GHOSTS_UPTIME = 199; // msec.

    constexpr int POINTS_COUNT_FOR_INKY_FIRST_STEP  = 30;
    constexpr int POINTS_COUNT_FOR_CLYDE_FIRST_STEP = 59; // points_count / 3. might be calculate dynamically

    constexpr float MARGIN = 2;
}
