#include <algorithm>
#include "Plain.h"

namespace mtm {

    bool isStronger(const GroupPointer& group1, const GroupPointer& group2);

    Plain::Plain(cstring name) : Area(name) {
    }

    const std::string Plain::generateNewGroupName(cstring original_name) const{
        std::string unnumbered = original_name + "_";
        std::string new_name;
        char i = '2';
        do {
            new_name = unnumbered + i;
            ++i;
        } while (this->findGroup(new_name));
        return new_name;
    }

    bool isStronger(const GroupPointer& group1, const GroupPointer& group2) {
        return *group1 > *group2;
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
    void Plain::groupArrive(cstring group_name, cstring clan,
                            map<string, Clan> &clan_map) {
        try {
            Clan& group_clan = getNewGroupClan(group_name, clan, clan_map);
            GroupPointer group = group_clan.getGroup(group_name);
            int group_size = group->getSize();
            int clan_size = group_clan.getSize();
            if (group_size * 3 > clan_size) { /* group size > 1/3 * clan size */
                if (group_size >= 10) {
                    cstring new_name(generateNewGroupName(group->getName()));
                    GroupPointer new_group(new Group(group->divide(new_name)));
                    group_clan.addGroup(*new_group);
                    this->groups.push_back(new_group);
                }
                this->groups.push_back(group);
            } else { /* group size <= 1/3 * clan size */
                bool success = false;
                std::sort(this->groups.begin(), this->groups.end(), isStronger);
                for (const GroupPointer& current : this->groups) {
                    if (current->getClan() != group->getClan()) continue;
                    if (current->unite(*group, clan_size / 3)) {
                        success = true;
                        break;
                    }
                }
                if (!success) this->groups.push_back(group);
            }
        } catch(...) {
            throw;
        }
    }
}

/**
 * Function objects:
 * @code
 * class compareFunction{
 * public:
 *     bool operator()(int a, int b){
 *         return a < b;
 *     }
 * };
 *
 * compareFunction f;
 * f(5,60);
 * @endcode
*/
