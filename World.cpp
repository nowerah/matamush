#include <string>
#include <stdexcept>
#include <cassert>
#include "World.h"
#include "Plain.h"
#include "Mountain.h"
#include "River.h"
#include "exceptions.h"

namespace mtm {

    /**
     * (A static template function.)
     * Checks if a std::map contains the given key. Returns true, if so.
     * @tparam key_type Type of the map's key field.
     * @tparam value_type Type of the map's value field.
     * @param key The key whose existence in the map is checked.
     * @param map The map to search for the key in.
     * @return True if the key was found in the map, and false, otherwise.
     */
    template<typename key_type, typename value_type>
    bool mapContainsKey(key_type key, map <key_type, value_type> map) {
        for (std::pair<key_type, value_type> current : map) {
            if (current.first == key) return true;
        }
        return false;
    }

    bool World::hasGroup(cstring group_name) const {
        for (const std::pair<const string, Clan>& current : this->clan_map) {
            if (current.first.empty()) continue;
            const Clan& clan = current.second;
            if (clan.doesContain(group_name)) return true;
        }
        return false;
    }

    const GroupPointer& World::getGroup(cstring group_name) const {
        for (const std::pair<const string, Clan>& current : this->clan_map) {
            if (current.first.empty()) continue;
            const Clan& clan = current.second;
            if (clan.doesContain(group_name)) {
                return clan.getGroup(group_name);
            }
        }
        throw WorldGroupNotFound();
    }

    const string World::getGroupArea(cstring group_name) const {
        for (const std::pair<const string, AreaPtr>& current : this->area_map) {
            const AreaPtr& area = current.second;
            if (area->getGroupsNames().contains(group_name)) {
                return current.first;
            }
        }
        return "";
    }

    /**
     * Add a new clan to the world.
     * @param new_clan The name of the new clan
     * @throws WorldInvalidArgument If new_clan is empty
     * @throws WorldClanNameIsTaken If there is or was a clan with the
     *  given name.
     */
    void World::addClan(cstring new_clan) {
        /* DISCLAIMER:
         * This function allows names of emptied clans (united or lost in fight)
         * to be reused. Although this contradicts the comments above, this is
         * based on a discussion in the forum. Thread ID in forum: 405278.
         */
        if (new_clan.empty()) throw WorldInvalidArgument();
        if (mapContainsKey<string, Clan>(new_clan, this->clan_map)) {
            throw WorldClanNameIsTaken();
        }
        clan_map.insert(std::pair<string, Clan>(new_clan, Clan(new_clan)));
    }

    /**
     * Add a new area to the world.
     * @param area_name The name of the area
     * @param type The type of the area (PLAIN, MOUNTAIN, RIVER)
     * @throws WorldInvalidArgument If area_name is empty
     * @throws WorldAreaNameIsTaken If there is already an area with the
     *  given name.
     */
    void World::addArea(cstring area_name, AreaType type) {
        if (area_name.empty()) throw WorldInvalidArgument();
        if (mapContainsKey<string, AreaPtr>(area_name, this->area_map)) {
            throw WorldAreaNameIsTaken();
        }
        if (type == PLAIN) {
            area_map.insert(std::pair<string, AreaPtr>
                                (area_name, AreaPtr(new Plain(area_name))));
        } else if (type == MOUNTAIN) {
            area_map.insert(std::pair<string, AreaPtr>
                                (area_name, AreaPtr(new Mountain(area_name))));
        } else { // type == RIVER
            area_map.insert(std::pair<string, AreaPtr>
                                (area_name, AreaPtr(new River(area_name))));
        }
    }

    /**
     * Add a new group to the world, with given size, clan, and starting
     *  area (the group "arrives" to the area).
     *  The group should have 77 morale when entering the area.
     * @param group_name The name of the new group
     * @param clan_name The name of the clan of the group.
     * @param num_children Number of children in the group.
     * @param num_adults Number of the adults in the group.
     * @param area_name The name of the area the group starts in (arrives
     *  to it first)
     * @throws WorldInvalidArgument If group_name is empty, or at least
     * one of num_children and num_adults is negative, or both are 0.
     * @throws WorldGroupNameIsTaken If there is already a group with the
     *  given name in the world.
     * @throws WorldClanNotFound If there is no clan with the given name
     *  in the world.
     * @throws WorldAreaNotFound If there is no area with the given name
     *  in the world.
     */
    void World::addGroup(cstring group_name, cstring clan_name,
                         int num_children, int num_adults, cstring area_name) {
        try {
            Group new_group(group_name, num_children, num_adults);
            if (this->hasGroup(group_name)) throw WorldGroupNameIsTaken();
            if (!mapContainsKey<string, Clan>(clan_name, this->clan_map)) {
                throw WorldClanNotFound();
            }
            if (!mapContainsKey<string, AreaPtr>(area_name, this->area_map)) {
                throw WorldAreaNotFound();
            }
            clan_map.at(clan_name).addGroup(new_group);
            area_map.at(area_name)->groupArrive(group_name,clan_name,clan_map);
        } catch (const GroupInvalidArgs& groupInvalidArgs) {
            throw WorldInvalidArgument();
        } catch (...) {
            // this catches WorldGroupNameIsTaken, WorldClanNotFound,
            //  WorldAreaNotFound
            throw;
        }
    }

    /**
     * Make that an area reachable from another area.
     * (make 'to' reachable from 'from')
     * @param from The name of the area that the other area will be
     * reachable from.
     * @param to The name of the area that should be reachable from the
     * other area.
     * @throws WorldAreaNotFound If at least one of the areas isn't in
     *  the world.
     */
    void World::makeReachable(cstring from, cstring to) {
        try {
            Area& areaFrom = *area_map.at(from);
            Area& areaTo = *area_map.at(to);
            areaFrom.addReachableArea(to);
        } catch (const std::out_of_range& oor) {
            throw WorldAreaNotFound();
        }
    }

    /**
     * Move a group to destination area.
     * @param group_name The name of the group that should move
     * @param destination The name of the area the group should move to.
     * @throws WorldGroupNotFound If there is no group with the given
     *  name in the world.
     * @throws WorldAreaNotFound If there is no area with the given name
     *  in the world.
     * @throws WorldGroupAlreadyInArea If the group is already in the
     *  destination area.
     * @throws WorldAreaNotReachable If the destination area isn't
     *  reachable from the area the group is currently in.
     */
    void World::moveGroup(cstring group_name, cstring destination) {
        try {
            if (group_name.empty() || !this->hasGroup(group_name)) {
                throw WorldGroupNotFound();
            }
            if (!mapContainsKey<string, AreaPtr>(destination, area_map)) {
                throw WorldAreaNotFound();
            }
            Area &dest_area = *area_map.at(destination);
            if(dest_area.getGroupsNames().contains(group_name)){
                throw WorldGroupAlreadyInArea();
            }
            Area &group_area = *area_map.at(getGroupArea(group_name));
            if (!group_area.isReachable(destination)) {
                throw WorldAreaNotReachable();
            }
            const GroupPointer &group = this->getGroup(group_name);
            group_area.groupLeave(group_name);
            dest_area.groupArrive(group_name, group->getClan(), clan_map);
        } catch (const std::out_of_range& oor) {
            // The group was originally in an illegal area (in another world?)
            throw WorldAreaNotReachable();
        }
    }

    /**
     * Make two clans friends.
     * @param clan1 The name of one of the clans to become friends.
     * @param clan2 The name of the other clan to become friends with.
     * @throws WorldClanNotFound If at least one of the clans isn't in
     * the world.
     */
    void World::makeFriends(cstring clan1, cstring clan2) {
        if(!mapContainsKey(clan1,clan_map) || !mapContainsKey(clan2,clan_map)){
            throw WorldClanNotFound();
        }
        clan_map.at(clan1).makeFriend(clan_map.at(clan2));
    }

    /**
     * Unite to clans to a new clan with a new name.
     * @param clan1 The name of one of the clan that need to unite.
     * @param clan2 The name of the second clan that need to unite.
     * @param new_name The name of the new clan.
     * @throws WorldInvalidArgument If new_name is empty.
     * @throws WorldClanNameIsTaken If new_name was already used for a
     * clan that is not clan1 or clan2.
     * @throws WorldClanNotFound If clan1 or clan2 are not in the world.
     */
    void World::uniteClans(cstring clan1, cstring clan2, const
    string &new_name) {
        if (new_name.empty()) throw WorldInvalidArgument();
        if (clan1 == clan2) throw WorldInvalidArgument(); // based on forum
        if (new_name != clan1 && new_name != clan2 &&
                mapContainsKey(new_name, clan_map)) {
            throw WorldClanNameIsTaken();
        }
        if(!mapContainsKey(clan1,clan_map) || !mapContainsKey(clan2,clan_map)){
            throw WorldClanNotFound();
        }
        Clan& given_clan1 = clan_map.at(clan1);
        Clan& given_clan2 = clan_map.at(clan2);
        Clan united(given_clan1.unite(given_clan2, new_name));
        
        // To change the key of the new clan, we remove the two old keys,
        // and add a new one.
        clan_map.erase(clan1);
        clan_map.erase(clan2);
        clan_map.insert(std::pair<string, Clan>(new_name, united));
    }

    /**
     * Print a group to the ostream, using the group output function (<<).
     * Add to it another line (after the last one of a regular print) of
     * the form:
     *      Group's current area: [area name]
     * That print the area which the group is in.
     * @param os The ostream to print into.
     * @param group_name The name of the group to print.
     * @throws WorldGroupNotFound If there is no group in the world with
     *  the given name.
     */
    void World::printGroup(std::ostream &os, cstring group_name) const {
        try {
            const GroupPointer& group = this->getGroup(group_name);
            os << *group << "Group's current area: "
               << this->getGroupArea(group_name) << std::endl;
        } catch (const WorldGroupNotFound& worldGroupNotFound) {
            throw WorldGroupNotFound(); // make this function the source
        }
    }

    /**
     * Print a clan to the ostream, using the clan output function.
     * @param os The ostream to print into.
     * @param clan_name The name of the clan to print.
     * @throws WorldClanNotFound If there is no clan with the given name
     *  in the world.
     */
    void World::printClan(std::ostream &os, cstring clan_name) const {
        try {
            const Clan& clan = clan_map.at(clan_name);
            os << clan;
        } catch(const std::out_of_range& oor) {
            throw WorldClanNotFound();
        }
    }
}