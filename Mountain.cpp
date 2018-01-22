//
// Created by Noor Athamnah on 1/22/2018.
//



#include "Mountain.h"

namespace mtm {

    Mountain::Mountain(const string &name) : Area(name), ruler(nullptr) {
    }


    // Deal with the option of a group becoming empty after a fight.
    void Mountain::groupArrive(const string &group_name, const string &clan, map<string, Clan> &clan_map) {
        try {
            Clan& group_clan = getNewGroupClan(group_name, clan, clan_map);
            const GroupPointer& group = group_clan.getGroup(group_name);
            this->groups.push_back(group_clan.getGroup(group_name));
            if (!ruler){
                ruler = group;
                return;
            }
            if (ruler->getClan() == clan) {
                if (*ruler < *group) ruler = group;
                return;
            }
            if (group->fight(*ruler) == WON) ruler = group;
        } catch(...) {
            throw;
        }
    }

    void Mountain::groupLeave(const string &group_name) {
        try{
            const GroupPointer group = this->findGroup(group_name);
            Area::groupLeave(group_name);
            if (ruler->getName() != group_name) return;
            if(this->groups.empty()){
                this->ruler = nullptr;
                return;
            }
            this->sortByStrongest();
            this->ruler = *this->groups.begin();
            for (const GroupPointer& current : this->groups){
                if (current->getClan() == group->getClan()){
                    this->ruler = current;
                    return;
                }
            }
        } catch(...){
            throw;
        }

    }


}