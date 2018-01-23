#include <string>
#include <stdexcept>
#include "World.h"
#include "Plain.h"
#include "Mountain.h"
#include "River.h"
#include "exceptions.h"

namespace mtm {

    template<typename key_type, typename value_type>
    bool mapContainsKey(key_type key, map<key_type, value_type> map) {
        for (std::pair<key_type, value_type> current : map) {
            if (current.first == key) return true;
        }
        return false;
    }

    bool World::hasGroup(const string &group_name) {
        for (std::pair<string, Clan> current : this->clan_map) {
            if (current.second.doesContain(group_name)) return true;
        }
        return false;
    }

    /**
     * Add a new clan to the world.
     * @param new_clan The name of the new clan
     * @throws WorldInvalidArgument If new_clan is empty
     * @throws WorldClanNameIsTaken If there is or was a clan with the
     *  given name.
     */
    void World::addClan(cstring new_clan) {
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
        if (mapContainsKey<string, Area>(area_name, this->area_map)) {
            throw WorldAreaNameIsTaken();
        }
        if (type == PLAIN) {
            area_map.insert(std::pair(area_name, Plain(area_name)));
        } else if (type == MOUNTAIN) {
            area_map.insert(std::pair(area_name, Mountain(area_name)));
        } else { // type == RIVER
            area_map.insert(std::pair(area_name, River(area_name)));
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
            if (!mapContainsKey<string, Area>(area_name, this->area_map)) {
                throw WorldAreaNotFound();
            }
            clan_map.at(clan_name).addGroup(new_group);
            area_map.at(area_name).groupArrive(group_name,clan_name,clan_map);
        } catch (const GroupInvalidArgs& groupInvalidArgs) {
            throw WorldInvalidArgument();
        } catch (...) {
            // catches WorldGroupNameIsTaken, WorldClanNotFound, WorldAreaNotFound
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
            Area& areaFrom = area_map.at(from);
            Area& areaTo = area_map.at(to);
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

    }

    /**
     * Make two clans friends.
     * @param clan1 The name of one of the clans to become friends.
     * @param clan2 The name of the other clan to become friends with.
     * @throws WorldClanNotFound If at least one of the clans isn't in
     * the world.
     */
    void World::makeFriends(cstring clan1, cstring clan2) {

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

    }

    /**
     * Print a clan to the ostream, using the clan output function.
     * @param os The ostream to print into.
     * @param clan_name The name of the clan to print.
     * @throws WorldClanNotFound If there is no clan with the given name
     *  in the world.
     */
    void World::printClan(std::ostream &os, cstring clan_name) const {

    }
}