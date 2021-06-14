inline class PreEvacFSM {
  public:
    //// void printState();
    void wait(int ms, void (PreEvacFSM::*f)(void));

    void LINE_TRACK(); // 1
    void RESET(); // 2
    void READ_COLOUR_SENSORS(); // 3
    void READ_BLACK_LINE(); // 4
    void INITIAL_TURN(); // 5
    //// void INITIAL_FORWARD();
    void PICKUP(); // 6
    void WAIT(); // 7
    void CHECK_RED();

    unsigned long initialTime;
    int duration;
    int turnDirection;
    int cycles = 0;

    //* Obstacle avoidance
    void ORIENTATE();
    void FORWARD();
    void CLOCKWISE_TURN();

    int finalAngle = 0;

    void (PreEvacFSM::*nextState)(void);

    void (PreEvacFSM::*currentState)(void);
    void run();
    PreEvacFSM();
} preEvacFSM;

class LineTrack {
  private:
    double maxError = 0;
    double lastError = 0;
  public:
    void operator()(double* sensors);
    void reset();
};


