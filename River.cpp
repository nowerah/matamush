#include "River.h"

namespace mtm {

    River::River(const string &name) : Area(name) {
    }

    void River::groupArrive(const string &group_name, const string &clan, map<string, Clan> &clan_map) {

        Area::groupArrive(group_name, clan, clan_map);
    }
}