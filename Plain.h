#ifndef MATAMUSH_PLAIN_H
#define MATAMUSH_PLAIN_H

#include <string>
#include "Area.h"

typedef const std::string& cstring;

namespace mtm {
    class Plain : public Area {
        /**
         * Returns a new name for resulting list from the division of another.
         * The new name is the original name, then underscore and a number.
         * The number is the smallest one possible (at least 2) that is not in use.
         * @param original_name The name of the group that was divided.
         * @return The generated name for the new group.
         * @example if original_name = abc, this might return abc_2 or abc_10
         */
        const std::string generateNewGroupName(cstring original_name) const;

    public:
        explicit Plain(cstring name);
        ~Plain() override = default;

        /**
         * Disable copy constructor and assignment operator.
         */
        Plain(const Plain&) = delete;
        Plain &operator=(const Plain&) = delete;

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
        void groupArrive(cstring group_name, cstring clan,
                                 map<string, Clan>& clan_map) override;

    };
}


#endif //MATAMUSH_PLAIN_H
