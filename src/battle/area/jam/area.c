#include "area.h"

#include "battle/common/newstage/area_jam/jam_01.inc.c"

extern ActorBlueprint A(test);

// TODO Make New Battle Stage Map.
extern Stage A(jam_01);

Formation A(Formation_00) = {
    ACTOR_BY_IDX(A(test), BTL_POS_GROUND_B, 10),
};

BattleList A(Formations) = {
    BATTLE(A(Formation_00), A(jam_01), "Test"),
    {},
};

StageList A(Stages) = {
    STAGE("jam_01", A(jam_01)),
    {},
};
