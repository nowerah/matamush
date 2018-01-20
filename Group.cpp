//
// Created by Noor Athamnah on 1/17/2018.
// hiiiii

#include <iostream>
#include "Group.h" /* includes string and ostream */
#include "exceptions.h"

namespace mtm {

    Group::Group(const std::string &name, const std::string &clan, int children,
                 int adults, int tools, int food, int morale) {
        if (name == "" || clan == "" || adults < 0 || children < 0
            || tools < 0 || food < 0 || morale < 0 || morale > 100) {
            throw GroupInvalidArgs();
        }
        if (adults == 0 && children == 0) throw GroupInvalidArgs();
        this->name = name;
        this->clan = clan;
        this->children = children;
        this->adults = adults;
        this->tools = tools;
        this->food = food;
        this->morale = morale;
    }

    /**
     * Constructor.
     * - The group will not belong to any clan.
     * - The group will have 4 tools for every adult.
     * - The group will have 3 food for every adult,
     *      and 2 food for every child
     * - The group will have 70 morale.
     * @param name The group's name. Can't be empty.
     * @param children The amount of children in the group. Must be
     *  non-negative.
     * @param adults Bigger number means more tools. Must be non-negative.
     * @throws GroupInvalidArgs If at least one of the arguments is
     *  invalid, or both adults and children are 0.
     */
    Group::Group(const std::string &name, int children, int adults) {
        if (name == "" || children < 0 || adults < 0) throw GroupInvalidArgs();
        if (adults == 0 && children == 0)
            throw GroupInvalidArgs();
        this->name = name;
        this->children = children;
        this->adults = adults;
        this->clan = "";
        this->tools = 4 * adults;
        this->food = 3 * adults + 2 * children;
        this->morale = 70;
    }

    /**
     * Copy constructor
     * @param other The group to copy
     */
    Group::Group(const Group &other) = default;

    /** Destructor
     */
    Group::~Group() = default;

    /**
     * Get the name of the group
     * @return The name of the group
     */
    const std::string &Group::getName() const {
        return this->name;
    }

    /**
     * Get the amount of people (children + adults) in the group.
     * @return  Amount of people in the group
     */
    int Group::getSize() const {
        return (this->adults + this->children);
    }

    /**
     * @return The name of the clan that the group belongs to.
     */
    const std::string &Group::getClan() const {
        return this->clan;
    }

    /**
     * Change the clan of the group.
     * If the group had a different clan before, reduce morale by 10%.
     * If the group's clan was the same as the given one, do nothing.
     * Otherwise, increase morale by 10%. (floor in both cases)
     *
     * floor: 0.5 -> 0, 1.5 -> 1 etc...
     * @param clan The name of the new clan that the groups will belong to.
     */
    //!!!!! Maybe should check the application for floor thing!!!!!
    void Group::changeClan(const std::string &clan) {
        if (this->clan == clan) return;
        if (this->clan != "") {
            this->morale = int(0.9 * double(this->morale));
        } else {
            this->morale = int(1.1 * double(this->morale));
        }
        this->clan = clan;
    }

    /**
     * Gets the power of the group.
     * Power is defined : (10nA + 3nC)*(10nT + nF)*morale/100
     * Where as: nA = number of adults, nC = number of children
     * nF = food, nT = tools
     */
    double Group::getPower() const {
        int sum = (10 * this->adults + 3 * this->children)
                  * (10 * this->tools + this->food) * this->morale;
        return double(sum) / 100;
    }


    /**
     * Compare two groups. according to power (defined here), and name.
     *
     * Power: ((10*number_of_adults + 3 * number_of_children) *
     * (10 * number_of_tools + number_of_food)*morale)/100 (floor)
     *
     * @param rhs
     * @return true, if power(this) < power(rhs), or if they're equal and
     *  this group's name is smaller (lexicographic) than rhs's name.
     *  false otherwise.
     */
    bool Group::operator<(const Group &rhs) const {
        double power1 = this->getPower(), power2 = rhs.getPower();
        if (power1 < power2) return true;
        if (power1 > power2) return false;
        if (this->name < rhs.name) return true;
        return false;
    }

    /**
     * Compare two groups. according to power, and name.
     * @param rhs
     * @return true, if power(this) > power(rhs), or if they're equal and
     *  this group's name is bigger (lexicographic) than rhs's name.
     *  false otherwise.
     */
    bool Group::operator>(const Group &rhs) const {
        return rhs < *this;
    }

    /**
     * Compare two groups. according to power, and name.
     * @param rhs
     * @return true, if power(this) < power(rhs), or if they're equal and
     *  this group's name is not bigger (lexicographic) than rhs's name.
     *  false otherwise.
     */
    bool Group::operator<=(const Group &rhs) const {
        return !(*this > rhs);
    }

    /**
     * Compare two groups. according to power, and name.
     * @param rhs
     * @return true, if power(this) > power(rhs), or if they're equal and
     *  this group's name is not smaller (lexicographic) than rhs's name.
     *  false otherwise.
     */
    bool Group::operator>=(const Group &rhs) const {
        return !(*this > rhs);
    }

    /**
     * Compare two groups. according to power, and name.
     * @param rhs
     * @return true, if power(this) == power(rhs), and this group's name is
     * equal to rhs's name.
     * false otherwise.
     */
    bool Group::operator==(const Group &rhs) const {
        if (*this < rhs || *this > rhs) return false;
        return true;
    }

    /**
     * Compare two groups. according to power, and name.
     * @param rhs
     * @return true, if power(this) != power(rhs), and this group's name
     * isn't equal to rhs's name.
     * false otherwise.
     */
    bool Group::operator!=(const Group &rhs) const {
        return !(*this == rhs);
    }

    /**
     * Try to unite two groups, this group, and given group.
     * Will succeed only if the two groups are not the same group,
     * belong to the same clan, both have at least 70 morale, and they
     * don't surpass together a given amount of people.
     * Should the union succeed, this group will change according to:
     * - name = name of the more powerful group
     * - clan = old clan
     * - children = sum of the children in the two groups
     * - adults = sum of the adults in the two groups
     * - tools = sum of the tools in the two groups
     * - food = sum of the food in the two groups
     * - morale = average of the morales, counting the amount of people
     * (floor)
     *
     * The other group will become empty.
     * @param other The group to unite with.
     * @param max_amount If the two groups have more people combined then
     *  max_amount, then the union fails.
     * @return True if Unite was successful, false otherwise.
     */
    bool Group::unite(Group &other, int max_amount) {
        if (this == &other) return false;
        int size_this = this->getSize();
        int size_other = other.getSize();
        if (this->clan != other.clan || this->morale < 70 || other.morale < 70
            || (size_this + size_other) > max_amount)
            return false;
        if (this->getPower() < other.getPower()) {
            this->name = other.name;
        }
        this->adults += other.adults;
        this->children += other.children;
        this->food += other.food;
        this->tools += other.tools;
        int morale_total = this->morale * size_this + other.morale * size_other;
        double morale_new = double(morale_total) / double(size_other + size_this);
        this->morale = int(morale_new);
        other.clearGroup();
        return true;
    }

    /**
     * Divide the group.
     *
     * After the division this group will have the same
     *  name, clan, morale. And ceiling of half of children , adults ,
     *  tools, food
     *
     * The other group will have a new name, same clan, same morale,
     *  and floor of half of children , adults, tools, food.
     *
     * Ceiling: 0.5 -> 1, 1.5 -> 2 etc...
     *
     * floor: 0.5 -> 0, 1.5 -> 1 etc...
     * @param name The name of the new group.
     * @return The new group, that divided from this group.
     * @throws GroupInvalidArgs If name is empty.
     * @throws GroupCantDivide If the divided group Would be empty (can
     * only happen of this group has no more than 1 child and 1 adult).
     */
    Group Group::divide(const std::string &name){
        if (name == "") throw GroupInvalidArgs();
        if(this->children <= 1 && this->adults <= 1){
            throw GroupCantDivide();
        }
        double half_children = double(children)/2;
        double half_adults = double(adults)/2;
        double half_food = double(food)/2;
        double half_tools = double(tools)/2;
        Group new_group(name, clan, int(half_children), int(half_adults),
                        int(half_tools), int(half_food), morale);

        this->children = int(half_children+0.5);
        this->adults = int(half_adults+0.5);
        this->food = int(half_food+0.5);
        this->tools = int(half_tools+0.5);
        return new_group;
    }

    /**
     * This group fights an opponent group. The bigger group according to
     *  the relational operators is the winner. If both groups are equal,
     *  the groups will not fight.
     *
     * Fights effects on the loser group:
     * - Losing third of each of the children and adult population (ceiling)
     * - Losing Half of the tools, and half of the food (ceiling)
     * - Morale decreases by 20%. (ceiling)
     * For example: if a group that had 5 adults and 3 tools lost and 50
     * morale, it will have 3 adults, 1 tool, and 40 morale after the fight.
     *
     * Fights effects on the winner group:
     * - Losing a quarter of the adults (floor) and no children.
     * - Losing a quarter of the tools (floor).
     * - gaining half of the food the loser group lost in the fight. (floor)
     *      The gain is equal to ceiling(loser.food / 2) / 2, even if the
     *      loser group lost all of it's people.
     * - Morale increases by 20%. (ceiling)
     * @param opponent The group to fight with.
     * @return WON if this group won, LOST if this group lost, and DRAW
     * in case the 2 groups are equal.
     * @throws GroupCantFightWithItself if opponent is the same as this
     * @throws GroupCantFightEmptyGroup if this group, or the other group
     *  is empty (i.e no people).
     */
    FIGHT_RESULT Group::fight(Group &opponent){
        if (this == &opponent) throw GroupCantFightWithItself();
        if ((this->getSize()==0) || (opponent.getSize() == 0)){
            throw GroupCantFightEmptyGroup();
        }
        if (*this == opponent) return DRAW;
        else if (*this > opponent){ //This group wins
            this->handleFight(opponent);
            if(this->getPower()==0) this->clearGroup();
            if(opponent.getPower()==0) this->clearGroup();
            return WON;
        }
        else {
            opponent.handleFight(*this);
            if(this->getPower()==0) this->clearGroup();
            if(opponent.getPower()==0) this->clearGroup();
            return LOST;
        }
    }

    /**
        * Clears fields in group.
        * all numbers become 0, all strings become empty.
        */
    void Group::clearGroup(){
        this->children = 0;
        this->adults = 0;
        this->food = 0;
        this->tools = 0;
        this->morale = 0;
        this->clan = "";
        this->name = "";
    }

    /**
     * handleFight: applies the aftermath of a fight.
     * This Group is the winner, loser group in param
     * Fights effects on the loser group:
     * - Losing third of each of the children and adult population (ceiling)
     * - Losing Half of the tools, and half of the food (ceiling)
     * - Morale decreases by 20%. (ceiling)
     * For example: if a group that had 5 adults and 3 tools lost and 50
     * morale, it will have 3 adults, 1 tool, and 40 morale after the fight.
     *
     * Fights effects on the winner group:
     * - Losing a quarter of the adults (floor) and no children.
     * - Losing a quarter of the tools (floor).
     * - gaining half of the food the loser group lost in the fight. (floor)
     *      The gain is equal to ceiling(loser.food / 2) / 2, even if the
     *      loser group lost all of it's people.
     * - Morale increases by 20%. (ceiling)
     * @param loser The losing Group
     * This function assumes groups fought (can fight)
     */
    void Group::handleFight(Group &loser){
        int lost_food = int(loser.food/2 + 0.5);
        loser.children -= int(double(loser.children)/3 + 0.7);
        loser.adults -= int(double(loser.adults)/3 + 0.7);
        loser.tools -= int(double(loser.tools)/2 + 0.5);
        loser.food -= lost_food;
        loser.morale -= int(double(loser.morale)*0.2 + 0.8);
        this->adults -= int(double(this->adults)/4);
        this->tools -= int(double(this->tools)/4);
        this->food += int(double(lost_food)/2);
        this->morale += int(double(this->morale)*0.2 + 0.8);
    }

    /**
     * Try to do a trade between the two groups.
     * Each groups tries make its tools and food equal.
     * To do that, each group offers |group's tools - group's food| / 2
     * (ceiling) of the resource it has more of.
     * If the other group offers the other resource, a trade is made.
     * In the trade, each group gives the average of the offers [(offer1 +
     * offer2)/2 (ceiling)], from the resource it has more of. A group
     * can't give more tools or food than it has, and the two groups
     * always trade the same amount of resources.
     *
     * If one of the groups has the same amount of tools as amount of
     * food, or if both has more tools than food, or vice versa,
     * the trade would not be made.
     * @param other Group to trade with.
     * @return true if a trade was made, false otherwise.
     * @throws GroupCantTradeWithItself if other is the same as this
     */
    bool Group::trade(Group &other){
        if (this == &other) throw GroupCantTradeWithItself();
        int food1 = this->food, tools1 = this->tools;
        int food2 = other.food, tools2 = other.tools;
        if (food1 == tools1 || food2 == tools2 ||
                (food1 > tools1 && food2 > tools2)||
                (food1 < tools1 && food2 < tools2)) return false;
        int trade_amount = this->checkTradeAmount(other);
        if(food1>tools1){
            this->food -= trade_amount;
            other.food += trade_amount;
            other.tools -= trade_amount;
            this->tools += trade_amount;
        }
        else {
            this->tools -= trade_amount;
            other.tools += trade_amount;
            other.food -= trade_amount;
            this->food += trade_amount;
        }
        return true;
    }

    /**
     * Checks how much trade should be done.
     * assumes trade has succeeded and calculates amount.
     * @param other
     * @return the amount that should be traded
     */
    int Group::checkTradeAmount(Group const &other) const{
        int diff1 = int((this->food - this->tools)/2.0 + 0.5);
        int diff2 = int((other.food - other.tools)/2.0 + 0.5);
        int average;
        if(diff1 < 0) {
            diff1 = -diff1;
            average = int((diff1 + diff2)/2.0 + 0.5);
            if(average > this->tools) return this->tools;
            if(average > other.food) return other.food;
            return average;
        }
        else {
            diff2 = -diff2;
            average = int((diff1 + diff2)/2.0 + 0.5);
            if(average > this->food) return this->food;
            if(average > other.tools) return other.tools;
            return average;
        }
    }

    /**
     * Print the data of a given group. Output form:
     *      Group's name: [group's name]
     *      Group's clan: [group's clan]
     *      Group's children: [group's children]
     *      Group's adults: [group's adults]
     *      Group's tools: [group's tools]
     *      Group's food: [group's food]
     *      Group's morale: [group's morale]
     *
     * @param os the output stream
     * @param group The group to print
     * @return the output stream
     */
    std::ostream& operator<<(std::ostream &os, const Group &group){
        return os << "Group's name: " << group.name << std::endl
         << "Group's clan: " << group.clan << std::endl
         << "Group's children: " << group.children << std::endl
         << "Group's adults: " << group.adults << std::endl
         << "Group's tools: " << group.tools << std::endl
         << "Group's food: " << group.food << std::endl
         << "Group's morale: " << group.morale << std::endl;
    }
}