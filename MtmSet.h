#ifndef MTM4_SET_H
#define MTM4_SET_H

#include "exceptions.h"

namespace mtm{
    /**
     * A set
     * @tparam Type The type of an element on the group.
     *      Required to implement:
     *                             - operator ==
     *                             - copy constructor
     */
    template<typename Type>
    class MtmSet{
        /**
         * A node in the set
         */
        class Node{
            Node* next;
            Type element;

        public:
            Node() = default;
            explicit Node(Type& element) : next(nullptr), element(element) {
            }
            explicit Node(const Type& element) : next(nullptr), element(element) {
            }
            Node(const Node& other) : next(nullptr), element(other.element) {
            }
            ~Node() = default;
            Node* getNext() const {
                return this->next;
            }
            Type& getElement() {
                return this->element;
            }
            const Type& getElement() const {
                return this->element;
            }
            Node* setNext(Node* node) {
                this->next = node;
                return node;
            }
//            Type& setElement(const Type& element) {
//                this->element = Type(element);
//                return this->element;
//            }
            bool operator==(const Node&rhs) const {
                return this->element == rhs.element;
            };
        };

        /**
         * Properties of class MtmSet
         */
        int set_size;
        Node* head;

        /**
         * Deletes first node of the set, and updates head to the next node.
         */
        void deleteHead() {
            if (!head) return;
            Node* new_head = head->getNext();
            delete head;
            head = new_head;
            --set_size;
        }
    
    public:
        //Forward declaration
        class const_iterator;
        
        /**
         * A iterator for Set
        */
        class iterator{
            Node* node;

        public:
            /**
             * Empty constructor. Should not be dereferenced.
             * Same as MtmSet::end()
             */
            iterator() : node(nullptr) {
            }
            
            /**
             * Constructor of Set iterator
             * @param node The node the iterator points to
             */
            explicit iterator(Node *node) : node(node) {
            }
            
            /**
             * Copy constructor
             * @param it The iterator to copy
             */
            iterator(const iterator& it) : node(it.node) {
            }

            /**
             * Destructor
             */
            ~iterator() = default;

            /**
             * Copy assignment operator.
             * @param rhs The iterator to copy.
             * @return A reference to this iterator.
             */
            iterator& operator=(const iterator& rhs){
                this->node = rhs.node;
                return *this;
            }
            
            /**
             * Dereference operator * .
             * Used like dereference of a pointer with * .
             * @return A reference of the data in the node the iterator
             *  points to.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type& operator*() const{
                if (!node) throw NodeIsEndException();
                return node->getElement();
            }
            
            /**
             * Dereference operator -> .
             * Used like dereference of a pointer with -> .
             * @return A pointer to the data in the node the iterator
             *  points to.
             * C++ use the -> operator on the returned pointer,
             * this allows the user to treat the iterator like a pointer.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type *operator->() const{
                if (!node) throw NodeIsEndException();
                return &node->getElement();
            }
            
            /**
             * Prefix increment operator (++i)
             * @return a reference to the iterator;
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            iterator& operator++(){
                if (!node) throw NodeIsEndException();
                node = node->getNext();
                return *this;
            }
            
            /**
             * Postfix increment operator (i++)
             * @return iterator that points to the same node as this before
             * the increment
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            iterator operator++(int){
                if (!node) throw NodeIsEndException();
                iterator old(*this);
                node = node->getNext();
                return old;
            }
            
            /**
             * Compare an iterator with const_iterator. (When there is a
             *  regular iterator on the left.)
             * @param rhs the right const_iterator.
             * @return true if the two iterators point to the same node
             */
            bool operator==(const const_iterator& rhs) const{
                return rhs == const_iterator(this->node);
            }
            
            /**
             * Compare an iterator with const_iterator. (when there is a
             *  regular iterator on the left.
             * @param rhs the right const_iterator.
             * @return true if the two iterators don't point to the same node
             */
            bool operator!=(const const_iterator& rhs) const{
                return !(*this == rhs);
            }

            friend class const_iterator;
        };
        
        /**
         * A const_iterator for Set
        */
        class const_iterator{
            Node* node;

        public:
            /**
             * Empty constructor. Should not be dereferenced.
             * Same as MtmSet::end()
             */
            const_iterator() : node(nullptr) {
            }
            
            /**
             * Constructor of Set const_iterator
             * @param node The node the const_iterator points to
             */
            explicit const_iterator(Node *node) : node(node) {
            }
            
            /**
             * Copy constructor
             * @param it The const_iterator to copy
             */
            const_iterator(const const_iterator& it) : node(it.node) {
            }
            
            /**
             * Constructor from iterator (not const_iterator)
             * Allows casting from iterator to const_iterator
             * @param it The iterator to "copy" to a const_iterator
             */
            /****************** IGNORE ERROR, IT COMPILES *****************/
            const_iterator(const iterator& it) : node(it.node) {
                /* We need this to be implicit, to allow comparisons
                 * of the form "it == set.end()" */
            }

            /**
             * Destructor
             */
            ~const_iterator() = default;

            /**
             * Copy assignment operator.
             * @param rhs The iterator to copy.
             * @return A reference to this const_iterator.
             */
            const_iterator& operator=(const const_iterator& rhs){
                this->node = rhs.node;
                return *this;
            }
            
            /**
             * Dereference operator * .
             * Used like dereference of a pointer with * .
             * @return A reference of the data in the node the const_iterator
             *  points to.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type& operator*() const{
                if (!node) throw NodeIsEndException();
                return node->getElement();
            }
            
            /**
             * Dereference operator -> .
             * Used like dereference of a pointer with -> .
             * @return A pointer to the data in the node the const_iterator
             *  points to.
             *  C++ use the -> operator on the returned pointer,
             *  this allows the user to treat the const_iterator like a pointer.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type *operator->() const{
                if (!node) throw NodeIsEndException();
                return &node->getElement();
            }
            
            /**
             * Prefix increment operator (++i)
             * @return a reference to the iterator;
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const_iterator& operator++(){
                if (!node) throw NodeIsEndException();
                node = node->getNext();
                return *this;
            }
            
            /**
             * Postfix increment operator (i++)
             * @return const_iterator that points to the same node as this
             * before the increment
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const_iterator operator++(int){
                if (!node) throw NodeIsEndException();
                const_iterator old(*this);
                node = node->getNext();
                return old;
            }
            
            /**
             * Compare two const_iterators.
             * @param rhs the right const_iterator
             * @return true if the two const_iterators point to the same node
             */
            bool operator==(const const_iterator& rhs) const{
                return this->node == rhs.node;
            }
            
            /**
             * Compare two const_iterators.
             * @param rhs the right const_iterator
             * @return true if the two const_iterators don't point to the same
             * node
             */
            bool operator!=(const const_iterator& rhs) const{
                return this->node != rhs.node;
            }
        };
        
        /**
         * Empty constructor
         * Creates an empty set
         */
        MtmSet() : set_size(0), head(nullptr) {
        }
        
        /**
         * Copy constructor
         * @param set the Set to copy
         */
        MtmSet(const MtmSet& set) : set_size(set.set_size), head(nullptr) {
            Node* original = set.head;
            if (!original) return;
            this->head = new Node(*original);
            Node* current = this->head;
            original = original->getNext();
            for ( ; original; original = original->getNext()) {
                Node* new_node = new Node(*original);
                current = current->setNext(new_node);
            }
        }
        
        /**
         * Destructor
         * Free all allocated memory in the set.
         */
        ~MtmSet(){
            this->clear();
        }
        
        /**
         * Insert a new element to the set, doesn't insert if there is already
         *  an equal element in the set.
         * If an element was't inserted, all iterators should stay valid.
         * @param elem The element to insert to the set
         * @return An iterator to the inserted element, or to the equal
         * element if the element wasn't inserted.
         */
        iterator insert(const Type& elem){
            if (!head) {
                head = new Node(elem);
                set_size = 1;
                return iterator(head);
            }
            Node* current = head;
            for ( ; current->getNext(); current = current->getNext()) {
                if (current->getElement() == elem) return iterator(current);
            }
            if (current->getElement() == elem) return iterator(current);
            Node* new_node = new Node(elem);
            current = current->setNext(new_node);
            ++set_size;
            return iterator(current);
        }
        
        /**
         * Remove an element from the set. If there is no element equal to
         *  elem in the set, does nothing.
         * If an element wasn't removed, all iterators should stay valid.
         * @param elem the element to remove.
         */
        void erase(const Type& elem){
            if (!head) return;
            if (head->getElement() == elem) {
                deleteHead();
                return;
            }
            Node* current = head;
            for ( ; current && current->getNext(); current = current->getNext()) {
                if (current->getNext()->getElement() == elem) {
                    Node* next_next = current->getNext()->getNext();
                    delete current->getNext();
                    current->setNext(next_next);
                    --set_size;
                    return;
                }
            }
        }
        
        /**
         * Remove the element the iterator points to from the set.
         * Iterator to another set is undefined.
         * @param it The iterator to the element to the set.
         */
        void erase(const iterator& it){
            erase(*it);
        }
        
        /**
         * Get the amount of elements in the set.
         * @return The amount of elements in the set.
         */
        int size() const{
            return set_size;
        }
        
        /**
         * Check if the set is empty.
         * @return true is the set is empty.
         */
        bool empty() const{
            return set_size == 0;
        }
        
        /**
         * Empty the set, free all allocated memory in the set.
         */
        void clear(){
            while (head) deleteHead();
        }
        
        /**
         * Get an iterator to the first element in the set.
         * If set is empty, return the same as end.
         * @return
         */
        iterator begin(){
            return iterator(head);
        }
        
        /**
         * Get a const_iterator to the first element in the set.
         * If set is empty, return the same as end.
         * @return
         */
        const_iterator begin() const{
            return const_iterator(head);
        }
        
        /**
         * Returns an iterator referring to the past-the-end element in the set.
         * The past-the-end element is the theoretical element that would follow
         * the last element in the set. It does not point to any element, and
         * thus should not be dereferenced.
         * @return Iterator to past-the-end element.
         */
        iterator end(){
            /* In our implementation, the past-the-end element is always
             * nullptr, since the 'next' of last node is defined to be so. */
            return iterator(nullptr);
        }
        
        /**
         * Returns a const_iterator referring to the past-the-end element in
         * the set.
         * The past-the-end element is the theoretical element that would follow
         * the last element in the set. It does not point to any element, and
         * thus should not be dereferenced.
         * @return const_iterator to past-the-end element.
         */
        const_iterator end() const{
            return const_iterator(nullptr);
        }
        
        /**
         * Find an element in the set.
         * @param elem The element to find
         * @return An iterator that points to the elem.
         */
        iterator find(const Type& elem){
            if (!head) return iterator(nullptr);
            Node* current = head;
            for ( ; current; current = current->getNext()) {
                if (current->getElement() == elem) return iterator(current);
            }
            return iterator(nullptr);
        }
        
        /**
         * Find an element in the set.
         * @param elem The element to find
         * @return A const_iterator that points to the elem.
         */
        const_iterator find(const Type& elem) const{
            if (!head) return const_iterator(nullptr);
            Node* current = head;
            for ( ; current; current = current->getNext()) {
                if (current->getElement() == elem) {
                    return const_iterator(current);
                }
            }
            return const_iterator(nullptr);
        }
        
        /**
         * Check if an element is in the set.
         * @param elem The element to check if it's in the set.
         * @return True if the element is in the set, false otherwise.
         */
        bool contains(const Type& elem) const{
            const_iterator find = this->find(elem);
            return find != const_iterator(nullptr);
        }
        
        /**
         * Check if this set is a superset of a given set.
         * Superset: A superset of B if B is a subset of A, that mean's that
         *  A contains all the elements in B.
         * @param subset The set to check if it's a subset.
         * @return True if the given set is a subset of this set.
         */
        bool isSuperSetOf(const MtmSet& subset) const{
            Node* current = subset.head;
            for ( ; current; current = current->getNext()) {
                if (!contains(current->getElement())) {
                    return false;
                }
            }
            return true;
        }
        
        /**
         * Check if two set are equal, meaning, they contain the same elements.
         * @param rhs The set right of the == operator.
         * @return true if thw two set conatain the same elements, false
         *  otherwise.
         */
        bool operator==(const MtmSet& rhs) const{
            return this->isSuperSetOf(rhs) && rhs.isSuperSetOf(*this);
        }
        
        /**
         * Check if two set are equal, meaning, they contain the same elements.
         * @param rhs The set right of the != operator.
         * @return false if thw two set conatain the same elements, true
         *  otherwise.
         */
        bool operator!=(const MtmSet& rhs) const{
            return !(*this == rhs);
        }
        
        /**
         * Insert all the elements in the given set to this set (union).
         * @param set The set to insert all the elements from.
         * @return A reference to this set.
         */
        MtmSet& unite(const MtmSet& set){
            Node* current = set.head;
            for ( ; current; current = current->getNext()) {
                this->insert(current->getElement());
            }
            return *this;
        }
        
        /**
         * Returns a new set that is an union of this set, and a given set.
         * @param set The other set (other than this) to be in the union.
         * @return The new set.
         */
        MtmSet unite(const MtmSet& set) const{
            MtmSet copied_set(*this);
            MtmSet& united = copied_set.unite(set);
            return united;
        }
        
        /**
         * Remove all the elements from this set, that are not in the given set
         * (intersection).
         * @param set The other set in the intersection.
         * @return A reference to this set.
         */
        MtmSet& intersect(const MtmSet& set){
            Node* current = this->head;
            while (current) {
                Type& elem = current->getElement();
                current = current->getNext();
                if (!set.contains(elem)) this->erase(elem);
            }
            return *this;
        }
        
        /**
         * Returns a new set that is an intersection of this set,
         *  and a given set.
         * @param set The other set (other than this) to be in the intersection.
         * @return The new set.
         */
        MtmSet intersect(const MtmSet& set) const{
            MtmSet copied(*this);
            MtmSet& intersected = copied.intersect(set);
            return intersected;
        }
        
        /**
         * Remove all the elements in the set, that doesn't meet a given
         *  condition.
         * @tparam func - A function of an object-function that receive 1
         *  argument, of the same type as an element in the set, and returns
         *  a bool. i.e.: bool () (Type)
         * @param condition - function (or object-function) that returns true
         * if the element is meeting the condition and false otherwise.
         * @return A reference to this set.
         */
        template<typename func>
        MtmSet& getSubSet(func condition){
            Node* current = this->head;
            while (current) {
                Type& elem = current->getElement();
                current = current->getNext();
                if (!condition(elem)) this->erase(elem);
            }
            return *this;
        }

        MtmSet& operator=(const MtmSet& set) {
            Node* original = set.head;
            this->clear();
            if (!original) return *this;
            this->head = new Node(*original);
            Node* current = this->head;
            original = original->getNext();
            for ( ; original; original = original->getNext()) {
                Node* new_node = new Node(*original);
                current = current->setNext(new_node);
            }
            return *this;
        }
        
        /**
         * Get a subset of this set, that contains all the elements in the set,
         * that doesn't meet a given condition.
         * @tparam func - A function of an object-function that receive 1
         *  argument, of the same type as an element in the set, and returns
         *  a bool.
         * @param condition - function (or object-function) that returns true
         * if the element is meeting the condition and false otherwise.
         * @return A the new set.
         */
        template<typename func>
        MtmSet getSubSet(func condition) const{
            MtmSet copied(*this);
            copied.getSubSet(condition);
            return copied;
        }
    };
} // namespace mtm

#endif //MTM4_SET_H
