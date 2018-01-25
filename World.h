#ifndef MTM4_WORLD_H
#define MTM4_WORLD_H

#include "Clan.h"
#include "Area.h"
#include <map>

namespace mtm{
    typedef std::shared_ptr<Area> AreaPtr;
    typedef const string& cstring;

    enum AreaType{ PLAIN, MOUNTAIN, RIVER };
    
    class World{
        map<string, Clan> clan_map;
        map<string, AreaPtr> area_map;

        /**
         * Checks if any of the World's Areas contain the given group.
         */
        bool hasGroup(cstring group_name) const;

        /**
         * Returns a shared pointer to the group whose name is given.
         * The group is found by looking in all clans of the world.
         * @return shared pointer to the group
         * @throws WorldGroupNotFound if group was not found
         */
        const GroupPointer& getGroup(cstring group_name) const;

        /**
         * Searches for the group in all the World's Areas.
         * @return name of Area the group is in, or empty string, if not found
         */
        const string getGroupArea(cstring group_name) const;
        
    public:
        /**
         * Empty constructor
         */
        World() = default;
        
        /**
         * Disable copy constructor
         */
        World(const World& world) = delete;

        /**
         * Disable assignment operator
         */
        World &operator=(const World &) = delete;

        ~World() = default;
        
        /**
         * Add a new clan to the world.
         * @param new_clan The name of the new clan
         * @throws WorldInvalidArgument If new_clan is empty
         * @throws WorldClanNameIsTaken If there is or was a clan with the
         *  given name.
         */
        void addClan(cstring new_clan);
        
        /**
         * Add a new area to the world.
         * @param area_name The name of the area
         * @param type The type of the area (PLAIN, MOUNTAIN, RIVER)
         * @throws WorldInvalidArgument If area_name is empty
         * @throws WorldAreaNameIsTaken If there is already an area with the
         *  given name.
         */
        void addArea(cstring area_name, AreaType type);
        
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
        void addGroup(cstring group_name, cstring clan_name, int
        num_children, int num_adults, cstring area_name);
        
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
        void makeReachable(cstring from, cstring to);
        
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
        void moveGroup(cstring group_name, cstring destination);
        
        /**
         * Make to clans friends.
         * @param clan1 The name of one of the clans to become friends.
         * @param clan2 The name of the other clan to become friends with.
         * @throws WorldClanNotFound If at least one of the clans isn't in
         * the world.
         */
        void makeFriends(cstring clan1, cstring clan2);
        
        
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
        void uniteClans(cstring clan1, cstring clan2, const
        string& new_name);
        
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
        void printGroup(std::ostream& os, cstring group_name) const;

        /**
         * Print a clan to the ostream, using the clan output function.
         * @param os The ostream to print into.
         * @param clan_name The name of the clan to print.
         * @throws WorldClanNotFound If there is no clan with the given name
         *  in the world.
         */
        void printClan(std::ostream& os, cstring clan_name) const;
    };
    
} // namespace mtm
#endif //MTM4_WORLD_H
