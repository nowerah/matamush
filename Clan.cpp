#include <algorithm>
#include "Clan.h"
#include "exceptions.h"

namespace mtm{

    bool compareGroups(const GroupPointer& group1, const GroupPointer& group2);

    /**
     * Constructor
     * @param name The name of the new clan, can't be empty
     * @throws ClanEmptyName if name is empty.
     */
    Clan::Clan(const std::string& name){
        if (name.empty()) throw ClanEmptyName();
        this->name = name;
    }

    /**
     * Add a group (copy of it) to the clan
     * @param group The group to add to the clan
     * @throws ClanGroupIsEmpty if the group has no people (empty)
     * @throws ClanGroupNameAlreadyTaken if there is already a group
     * with the same name in the clan.
     */
    void Clan::addGroup(const Group& group){
        if (group.getSize() == 0) throw ClanGroupIsEmpty();
        if (this->doesContain(group.getName()))
            throw ClanGroupNameAlreadyTaken();
        GroupPointer ptr(new Group(group));
        this->groups.push_back(ptr);
        ptr->changeClan(this->name);
    }

    /**
     * Get a group from the clan, with a given name
     * @param group_name The name of the group the return
     * @return A reference to the group with the given name
     * @throws ClanGroupNotFound if there is no group in the clan with the
     * given name.
     */
    const GroupPointer& Clan::getGroup(const std::string& group_name) const{
        for (std::list<GroupPointer>::const_iterator itr = this->groups.begin(); itr!=groups.end(); ++itr) {
            if ((*itr)->getName() == group_name) {
                return *itr;
            }
        }
        throw ClanGroupNotFound();
    }

    bool Clan::doesContain(const std::string& group_name) const{
        for (std::list<GroupPointer>::const_iterator itr = this->groups.begin(); itr!=groups.end(); ++itr) {
            if ((*itr)->getName() == group_name) {
                return true;
            }
        }
        return false;
    }

    /**
     * The function returns the amount of people in the clan.
     * A person belongs to the clan, if he belongs to a group, that
     * belongs to the clan.
     * @return The amount of people in the clan.
     */
    int Clan::getSize() const{
        int total_size = 0;
        for (std::list<GroupPointer>::const_iterator itr = this->groups.begin(); itr!=groups.end(); ++itr) {
            total_size += (*itr)->getSize();
        }
        return total_size;
    }

    /**
     * Make two clans unite, to form a new clan, with a new name. All the
     * groups of each clan will change their clan.
     * The friends of the new clan are all the friends of either one of
     * the old 2 clans.
     * The new clan will replace this clan, and the other clan (received
     * by an argument) will become empty (i.e no groups).
     *
     * If One of the exception is thrown, both clan shouldn't change.
     * @param other The clan to unite with
     * @param new_name The name of the new clan. If the name is of one of
     *  the uniting clans (assume to clan A), that means the other clan,
     *  joined clan A, and the groups of the clan A, don't change their
     *  clan.
     * @return A reference to the new clan (this).
     * @throws ClanEmptyName new_name is empty.
     * @throws ClanCantUnite If other is the same clan as this or if
     *  there is a group in one of the clans, that has the same name as a
     *  group in the other clan.
     */
    Clan& Clan::unite(Clan& other, const std::string& new_name){
        if (new_name == "") throw ClanEmptyName();
        if (this == &other) throw ClanCantUnite();
        for(const GroupPointer& ptr : this->groups) {
            if (other.doesContain(ptr->getName())) {
                throw ClanCantUnite();
            }
        }
        if (new_name == this->name){
            for (std::list<GroupPointer>::const_iterator itr = other.groups.begin(); itr!=groups.end(); ++itr){
                (*itr)->changeClan(new_name);
            }
        }
        else{
            this->name = new_name;
            for (std::list<GroupPointer>::const_iterator itr = this->groups.begin(); itr!=groups.end(); ++itr) {
                (*itr)->changeClan(new_name);
            }
        }
        for(std::list<GroupPointer>::const_iterator itr = other.groups.begin()
                ; itr!=other.groups.end(); ++itr) {
            this->addGroup(**itr);
        }
        for(MtmSet<Clan*>::const_iterator itr = other.friends.begin(); itr!=other.friends.end(); ++itr) {
            this->makeFriend(**itr);
            other.removeFriend(**itr);
        }
        other.clear();
        return *this;
    }


    /**
    * Clears a clan. makes name empty.
    * Clears Friends and Group sets.
    */
    void Clan::clear(){
        this->name = "";
        this->friends.clear();
        this->groups.clear();
    }

    /**
     * Make this clan, and a given clan, friends. If the two clans already
     * were friends, does nothing.
     * Assume that every clan has a unique name. (In copied clans, you
     * can assume one of the copies will not be used again).
     * @param other The clan to become a friend with.
     */
    void Clan::makeFriend(Clan& other){
        if (this->isFriend(other)) return;
        this->friends.insert(&other);
        other.friends.insert(this);
    }

    /**
     * Check if a given clan is a friend of this clan.
     * Every clan is a friend of itself.
     * @param other The clan to check if is a friend.
     * @return true, if the given clan is a friend of this clan, false
     *  otherwise.
     */
    bool Clan::isFriend(const Clan& other) const{
        if (this == &other) return true;
        for(Clan* curr : this->friends){
            if (curr->isEqual(other)) return true;
        }
        return false;
    }

    /**
     * Checks whether the two clans have the same name.
     * @param other the clan to check with
     * @return true is yes and false if not.
     */
    bool Clan::isEqual(const Clan& other) const{
        if (this->name == other.name) return true;
        return false;
    }


    /**
     * Removes a Clan (this) from the friend set of another clan.
     * Only used when the two are friends
     * Note: is asymmetrical
     * @param friend that should be removed
     */
    void Clan::removeFriend(Clan other){
        for(Clan* curr : other.friends) {
            other.friends.erase(curr);
        }
    }

    /**
     * Print The clan name, and it groups, sorted by groups comparison
     * operators, from strongest to weakest. In the next Format:
     *
     * Clan's name: [name]
     *     [1'st group name]
     *     [2'nd group name]
     *     [3'rd group name]
     *             *
     *             *
     *             *
     *
     * @param os The output stream
     * @param clan The clan to print
     * @return A reference to the output stream
     */
    std::ostream& operator<<(std::ostream& os, const Clan& clan){
        std::list<GroupPointer> copied(clan.groups);
        copied.sort(compareGroups);
        os << "Clan's name: " << clan.name << std::endl
           << "Clan's groups:" << std::endl;
        for(std::list<GroupPointer>::const_iterator itr = copied.begin(); itr!= copied.end() ; ++itr ){
            os << (*itr)->getName() << std::endl;
        }
        return os;
    }

    bool compareGroups(const GroupPointer& group1, const GroupPointer& group2) {
        return *group1 > *group2;
    }



}


