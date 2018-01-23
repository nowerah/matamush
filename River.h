#ifndef MATAMUSH_RIVER_H
#define MATAMUSH_RIVER_H

#include <string>
#include "Area.h"

typedef const std::string& cstring;

namespace mtm {
    class River : public Area {

    public:
        explicit River(cstring name);
        ~River() override = default;

        /**
         * Disable copy constructor and assignment operator.
         */
        River(const River&) = delete;
        River &operator=(const River&) = delete;

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

#endif //MATAMUSH_RIVER_H
