#include <algorithm>
#include <stdexcept>
#include "Area.h"

namespace mtm {

    bool compareGroups(const GroupPointer& group1, const GroupPointer& group2);

    /**
     * Class fields:
     *  std::string name;
     *  std::vector<GroupPointer> groups;
     *  MtmSet<std::string> reachableAreas;
     */

    Area::Area(const std::string& name) {
        if (name.empty()) throw AreaInvalidArguments();
        this->name = name;
    }

    Area::~Area() = default;

    const GroupPointer Area::findGroup(const string &group_name) const {
        for (GroupPointer group_ptr : this->groups) {
            if (group_ptr->getName() == group_name) return group_ptr;
        }
        return nullptr;
    }

    /**
        * Returns the clan of the group that might be added to the area.
        * @param group_name The name of the group that get into the area.
        * @param clan The name of the clan that the group belongs to.
        * @param clan_map The map of clans that contains the clan of the group
        * @throws AreaClanNotFoundInMap
        * @throws AreaGroupNotInClan
        * @throws AreaGroupAlreadyIn
    */
    Clan& Area::getNewGroupClan(const string &group_name, const string &clan,
                          map<string, Clan> &clan_map) {
        try {
            Clan& group_clan = clan_map.at(clan);
            if (!group_clan.doesContain(group_name)) throw AreaGroupNotInClan();
            if (this->getGroupsNames().contains(group_name)) {
                throw AreaGroupAlreadyIn();
            }
            return group_clan;
        } catch (const std::out_of_range& oor) {
            throw AreaClanNotFoundInMap();
        }
    }

    void Area::sortByStrongest() {
        std::sort(this->groups.begin(), this->groups.end(), compareGroups);
    }

    void Area::addReachableArea(const std::string& area_name) {
        this->reachableAreas.insert(area_name);
    }

    bool Area::isReachable(const std::string& area_name) const {
        return this->name == area_name ||
                this->reachableAreas.contains(area_name);
    }

    /**
     * Get a group into the area.
     * @param group_name The name of the group that get into the area.
     * @param clan The name of the clan that the group belongs to.
     * @param clan_map The map of clans that contains the clan of the group
     * @throws AreaClanNotFoundInMap If there is no clan with the given
     * name in the map.
     * @throws AreaGroupNotInClan If there is no group with the given name
     * in the clan with the given name in the map.
     * @throws AreaGroupAlreadyIn If group with same name already in the
     *  area.
     *
     * Assert that the map does contain the clan that the group belongs to.
     */
    void Area::groupArrive(const string &group_name, const string &clan,
                           map<string, Clan> &clan_map) {
        try {
            Clan& group_clan = getNewGroupClan(group_name, clan, clan_map);
            this->groups.push_back(group_clan.getGroup(group_name));
        } catch(...) {
            throw;
        }
    }

    /**
     * Get a group out of the area.
     * @param group_name The name of the leaving group.
     * @throws AreaGroupNotFound If there is no group in the area with the
     *  same name;
     */
    void Area::groupLeave(const std::string &group_name) {
        std::vector<GroupPointer>::iterator it = groups.begin();
        for ( ; it != groups.end(); ++it) {
            Group& group(*(*it));
            if (group.getName() == group_name) {
                this->groups.erase(it);
                return;
            }
        }
        throw AreaGroupNotFound();
    }

    MtmSet<std::string> Area::getGroupsNames() const {
        MtmSet<string> names;
        for (const GroupPointer& ptr : this->groups) {
            names.insert(ptr->getName());
        }
        return names;
    }
}