//
// Created by Noor Athamnah on 1/22/2018.
//

#ifndef MATAMUSH_MOUNTAIN_H
#define MATAMUSH_MOUNTAIN_H

#include "Area.h"

namespace mtm {
    class Mountain : public Area {
        GroupPointer ruler;

    public:
        explicit Mountain(const string& name);
        ~Mountain() override = default;

        /**
         * Disable copy constructor and assignment operator.
         */
        Mountain(const Mountain&) = delete;
        Mountain &operator=(const Mountain&) = delete;

        /**
         * Get a group into the area, according to the given rules.
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
        void groupArrive(const string& group_name, const string& clan,
                         map<string, Clan>& clan_map) override;


        void groupLeave(const string& group_name) override;
    };
}

#endif //MATAMUSH_MOUNTAIN_H
