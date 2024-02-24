#ifndef _PHQ_SET_H_
#define _PHQ_SET_H_


#include "rb_tree.h"


namespace phq {
template <class Key, class Compare = std::less<Key>>
class set {
public:
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;

private:
    typedef phq::rb_tree<value_type, key_compare> base_type;
    base_type tree_;

public:
    typedef typename base_type::node_type node_type;
    typedef typename base_type::const_pointer pointer;
    typedef typename base_type::const_pointer const_pointer;
    typedef typename base_type::const_reference reference;
    typedef typename base_type::const_reference const_reference;
    typedef typename base_type::const_iterator iterator;
    typedef typename base_type::const_iterator const_iterator;
    typedef typename base_type::const_reverse_iterator reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    typedef typename base_type::size_type size_type;
    typedef typename base_type::difference_type difference_type;
    typedef typename base_type::allocator_type allocator_type;

public:
    set() = default;
    template <class InputIterator>
    set(InputIterator first, InputIterator last) : tree_() {
        tree_.insert_unique(first, last);
    }
    set(std::initializer_list<value_type> ilist) : tree_() {
        tree_.insert_unique(ilist.begin(), ilist.end());
    }
    set(const set &rhs) : tree_(rhs.tree_) {   }
    set(set &&rhs) noexcept : tree_(phq::move(rhs.tree_)) {   }
    set &operator=(const set &rhs) {
        tree_ = rhs.tree_;
        return *this;
    }
    set &operator=(set &&rhs) {
        tree_ = phq::move(rhs.tree_);]
        return *this;
    }
    set &operator=(std::initializer_list<value_type> ilist) {
        tree_.clear();
        tree_.insert_unique(ilist.begin(), ilist.end());
        return *this;
    }

    key_compare key_comp() const { return tree_.key_comp(); }
    value_compare value_comp() const { return tree_.key_comp(); }
    allocator_type get_allocator() const { return tree_.get_allocator(); }

    iterator begin() noexcept { return tree_.begin(); }
    const_iterator begin() const noexcept { return tree_.begin(); }
    iterator end() noexcept { return tree_.end(); }
    const_iterator end() const noexcept { return tree_.end(); }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator rend() const noexcept { return end(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    bool empty() const noexcept { return tree_.empty(); }
    size_type size() const noexcept { return tree_.size(); }
    size_type max_size() const noexcept { return tree_.max_size(); }

    template <class... Args>
    pair<iterator, bool> emplace(Args &&...args) {
        return tree_.emplace_unique(phq::forward<Args>(args)...);
    }
    template <class... Args>
    iterator emplace_hint(iterator hint, Args &&...args) {
        return tree_.emplace_unique_use_hint(hint, phq::forward<Args>(args)...);
    }
    pair<iterator, bool> insert(const value_type &value) {
        return tree_.insert_unique(value);
    }
    pair<iterator, bool> insert(value_type &&value) {
        return tree_.insert_unique(phq::move(value));
    }
    iterator insert(iterator hint, const value_type &value) {
        return tree_.insert_unique(hint, value);
    }
    iterator insert(iterator hint, value_type &&value) {
        return tree_.insert_unique(hint, phq::move(value));
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        tree_.insert_unique(first, last);
    }

    void erase(iterator pos) { tree_.erase(pos); }
    size_type erase(const key_type &key) { return tree_.erase_unique(key); }
    void erase(iterator first, iterator last) { tree_.erase(first, last); }

    void clear() { tree_.clear(); }

    iterator find(const key_type &key) { return tree_.find(key); }
    const_iterator find(const key_type &key) const { return tree_.find(key); }

    size_type count(const key_type &key) const { return tree_.count_unique(key); }

    iterator lower_bound(const key_type &key) { return tree_.lower_bound(key); }
    const_iterator lower_bound(const key_type &key) const { return tree_.lower_bound(key); }
    iterator upper_bound(const key_type &key) { return tree_.upper_bound(key); }
    const_iterator upper_bound(const key_type &key) const { return tree_.upper_bound(key); }

    pair<iterator, iterator> equal_range(const key_type &key) { return tree_.equal_range_unique(key); }
    pair<const_iterator, const_iterator> equal_range(const key_type &key) const { return tree_.equal_range_unique(key); }

    void swap(set &rhs) noexcept { tree_.swap(rhs.tree_); }

public:
    friend bool operator==(const set &lhs, const set &rhs) { return lhs.tree_ == rhs.tree_; }
    friend bool operator<(const set &lhs, const set &rhs) { return lhs.tree_ < rhs.tree_; }
};






}







#endif