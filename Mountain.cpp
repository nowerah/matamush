#include "Mountain.h"

namespace mtm {

    Mountain::Mountain(const string &name) : Area(name), ruler("") {
    }

    // Deal with the option of a group becoming empty after a fight.
    void Mountain::groupArrive(const string &group_name, const string &clan,
                               map<string, Clan> &clan_map) {
        try {
            Clan& group_clan = getNewGroupClan(group_name, clan, clan_map);
            const GroupPointer& group = group_clan.getGroup(group_name);
            this->groups.push_back(GroupPointer(group));
            if (ruler.empty()){
                ruler = group_name;
                return;
            }
            const GroupPointer& group_ruler(this->findGroup(ruler));
            if (group_ruler->getClan() == clan) {
                if (*group_ruler < *group) ruler = group_name;
                return;
            }
            if (group->fight(*group_ruler) == WON) ruler = group_name;
            
        } catch(...) {
            throw;
        }
    }

    void Mountain::groupLeave(const string &group_name) {
        try{
            const GroupPointer group = this->findGroup(group_name);
            Area::groupLeave(group_name);
            if (ruler != group_name) return;
            if(this->groups.empty()){
                this->ruler = "";
                return;
            }
            this->sortByStrongest();
            const GroupPointer& strongest = *this->groups.begin();
            this->ruler = strongest->getName();
            for (const GroupPointer& current : this->groups){
                if (current->getSize() == 0) continue;
                if (current->getClan() == group->getClan()){
                    this->ruler = current->getName();
                    return;
                }
            }
        } catch(...){
            throw;
        }

    }


}