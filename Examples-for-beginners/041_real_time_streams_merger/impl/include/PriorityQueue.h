#ifndef ORDERED_MERGE_PRIORITY_QUEUE_H
#define ORDERED_MERGE_PRIORITY_QUEUE_H

#include <set>
#include <tr1/unordered_map>
#include <limits>
#include <cassert>

namespace pf 
{
    enum PriorityQueueOrder { PQ_MinAtTop, PQ_MaxAtTop };

    template <typename Key, typename Id,
              PriorityQueueOrder order=PQ_MinAtTop>
    class PriorityQueue
    {
    public:
        struct KeyIdPair
        {
            KeyIdPair(Key const & key, Id const & id)
                : key_(key), id_(id) {}
            Key & key() { return key_; }
            Key const & key() const { return key_; }
            Id & id() { return id_; }
            Id const & id() const { return id_; }
            bool operator==(KeyIdPair const & rhs) const
                { return key_==rhs.key_ && id_==rhs.id_; }
            bool operator<(KeyIdPair const & rhs) const
            {
                if (order==PQ_MaxAtTop) {
                    return key_>rhs.key_ ||
                        (key_==rhs.key_ && id_<rhs.id_);
                } else {
                    return key_<rhs.key_ ||
                        (key_==rhs.key_ && id_<rhs.id_);
                }
            }
            Key key_;
            Id id_;
        };
    private:
        typedef std::set<KeyIdPair> SortedItemsOnKey;
        typedef std::tr1::unordered_map<Id, typename SortedItemsOnKey::iterator> ItemsIndexedById;
    public:
        PriorityQueue (size_t maxItemCount=std::numeric_limits<size_t>::max())
            : maxItemCount_(maxItemCount) {}
        bool empty() const
        {
            return index_.empty();
        }
        size_t size() const
        {
            return index_.size();
        }
        void addItem(Key const & key, Id const & id)
        {
            assert(index_.count(id)==0);
            if (items_.size()==maxItemCount_) {
                KeyIdPair const & bottom = *items_.rbegin();
                Key const & bottomKey = bottom.key();
                bool out = (order==PQ_MaxAtTop) ? (bottomKey >= key) : (bottomKey <= key);
                if (!out) {
                    index_.erase(bottom.id());
                    items_.erase(*items_.rbegin());
                }
            }
            addItemHasSpace(key, id);
        }
        void updateItem(Key const & key, Id const & id)
        {
            typename ItemsIndexedById::iterator it = index_.find(id);
            if (it!=index_.end()) {
                if (key==it->second->key())
                    return; // no change
                items_.erase(it->second);
            }
            addItemHasSpace(key, id);
        }
        void removeItem(Id const & id)
        {
           typename ItemsIndexedById::iterator indexIter = index_.find(id);
           if (indexIter!=index_.end()) {
               items_.erase(indexIter->second);
               index_.erase(indexIter);
           }
        }
        bool hasItem(Id const & id)
        {
            return index_.count(id);
        }
        Key const & getItemKey(Id const & id)
        {
            return index_[id]->key();
        }
        KeyIdPair const & getTopItem() const
        {
            return *items_.begin();
        }
        std::set<KeyIdPair> const & getItems() const
        {
            return items_;
        }
        size_t getSize() const
        {
            return index_.size();
        }
    private:
        void addItemHasSpace(Key const & key, Id const & id)
        {
            index_[id] = items_.insert(KeyIdPair(key, id)).first;
        }
    private:
        size_t maxItemCount_;
        SortedItemsOnKey items_;
        ItemsIndexedById index_;
    };
}

#endif /* ORDERED_MERGE_PRIORITY_QUEUE_H */
