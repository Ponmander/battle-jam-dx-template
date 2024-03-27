#include "area.h"

#include "battle/common/stage/area_kmr/kmr_05b.inc.c"

extern ActorBlueprint A(jr_troopa);

extern Stage A(kmr_05b);

Formation A(Formation_00) = {
    ACTOR_BY_IDX(A(jr_troopa), BTL_POS_GROUND_C, 10),
};

BattleList A(Formations) = {
    BATTLE(A(Formation_00), A(kmr_05b), "Jr. Troopa Battle"),
    {},
};

StageList A(Stages) = {
    STAGE("kmr_05", A(kmr_05b)),
    {},
};
