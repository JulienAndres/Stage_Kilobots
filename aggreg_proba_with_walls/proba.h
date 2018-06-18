#ifndef PROBABILITY
    #define PROBABILITY

    #define PROBA_LEAVE 0.0001
    #define PROBA_JOIN 0.001

    void makeJoinDecision();
    void makeLeaveDecision();

    int uniform_proba();
    int clusters_proba();
    int clusters_proba2();

    int nbvoisin_bestvoisin();
    int clusters_proba3(int nb);


#endif
