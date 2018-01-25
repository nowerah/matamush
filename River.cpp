#include "River.h"

namespace mtm {

    River::River(const string &name) : Area(name) {
    }

    /**
     * Get a group into the area, according to the rules of the homework.
     * @param group_name The name of the group that get into the area.
     * @param clan The name of the clan that the group belongs to.
     * @param clan_map The map of clans that contains the clan of the group
     * @throws AreaClanNotFoundInMap If there is no clan with the given
     * name in the map.
     * @throws AreaGroupNotInClan If there is no group with the given name
     * in the clan with the given name in the map.
     * @throws AreaGroupAlreadyIn If group with same name already in the
     *  area.
     */
    void River::groupArrive(cstring group_name, cstring clan,
                            map<string, Clan> &clan_map) {
        try {
            Clan& group_clan = getNewGroupClan(group_name, clan, clan_map);
            const GroupPointer& group = group_clan.getGroup(group_name);
            this->sortByStrongest();
            for (const GroupPointer& current : this->groups) {
                if (current->getSize() == 0) continue;
                if (!group_clan.isFriend(clan_map.at(current->getClan()))) {
                    continue;
                }
                if (current->trade(*group)) break;
            }
            this->groups.push_back(group);
        } catch(...) {
            throw;
        }
    }
}