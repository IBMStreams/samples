/**
 * Copyright (C) 2016, International Business Machines Corporation.
 * All Rights Reserved.
 */
/*
 * \file NBUnorderedMap.h \brief Definition of the NBUnorderedMap class template.
 */

#ifndef SPL_RUNTIME_OPERATOR_STATE_NB_UNORDERED_MAP_H
#define SPL_RUNTIME_OPERATOR_STATE_NB_UNORDERED_MAP_H

#include <SPL/Runtime/Operator/State/Checkpoint.h>
#include <SPL/Runtime/Serialization/ByteBuffer.h>
#include <SPL/Runtime/Operator/State/DataStoreException.h>
#include <tr1/unordered_map>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

namespace NBCkpt
{
    /// \brief Class template that represents an unordered_map with non-blocking checkpointing capability
    template<typename Key, typename T>
    class NBUnorderedMap 
    {
    public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef std::pair<const key_type, mapped_type> value_type;
        typedef uint32_t size_type;

        /// Per-key meta-data
        struct Entry
        {
            uint8_t bits_;   // bit 1 is dirty bit; bit 0 indicates which copy (0 or 1) is current
            value_type * data_[2]; // pointers to two copies of data

            /// Constructor
            Entry() : bits_(0x00) 
            {
                data_[0] = data_[1] = NULL; 
            }

            /// Copy constructor
            Entry(const Entry & rhs) : bits_(rhs.bits_)
            {
                data_[0] = rhs.data_[0];
                data_[1] = rhs.data_[1];
            }

            /// Destructor
            ~Entry()
            {
                delete data_[0];
                delete data_[1];
            }
        };

        /// Forward iterator
        struct Iterator 
        {
            typename std::tr1::unordered_map<Key, Entry>::iterator iter_;

            /// Default constructor
            Iterator() { }

            /// Constructor
            Iterator(typename std::tr1::unordered_map<Key, Entry>::iterator & it) : iter_(it) { }

            /// Overloaded operator*() function
            value_type & operator*() 
            {
                Entry & entry = iter_->second;
                return *(entry.data_[entry.bits_ & 0x01]);
            }

            /// Overloaded operator->() function
            value_type * operator->() 
            {
                Entry & entry = iter_->second;
                return entry.data_[entry.bits_ & 0x01];
            }

            /// Overloaded operator==() function
            bool operator==(const Iterator & rhs) 
            {
                return (iter_ == rhs.iter_);
            }

            /// Overloaded operator!=() function
            bool operator!=(const Iterator & rhs) 
            {
                return (iter_ != rhs.iter_);
            }

            /// Overloaded operator++() function
            Iterator & operator++()
            {
                ++iter_;
                return *this;
            }

            /// Overloaded operator++() function
            const Iterator operator++(int) 
            {
                Iterator temp(iter_);
                ++(*this);
                return temp;
            }

            /// Overloaded operator--() function
            Iterator & operator--()
            {
                --iter_;
                return *this;
            }
        };

        /// Forward Constant iterator
        struct ConstIterator 
        {
            typename std::tr1::unordered_map<Key, Entry>::const_iterator iter_;

            /// Default constructor
            ConstIterator() { }

            /// Constructor
            ConstIterator(typename std::tr1::unordered_map<Key, Entry>::const_iterator & it) : iter_(it) { }

            /// Overloaded operator*() function
            const value_type & operator*() 
            {
                Entry & entry = iter_->second;
                return *(entry.data_[entry.bits_ & 0x01]);
            }

            /// Overloaded operator->() function
            const value_type * operator->() 
            {
                Entry & entry = iter_->second;
                return entry.data_[entry.bits_ & 0x01];
            }

            /// Overloaded operator==() function
            bool operator==(const ConstIterator & rhs) 
            {
                return (iter_ == rhs.iter_);
            }

            /// Overloaded operator!=() function
            bool operator!=(const ConstIterator & rhs) 
            {
                return (iter_ != rhs.iter_);
            }

            /// Overloaded operator++() function
            ConstIterator & operator++()
            {
                ++iter_;
                return *this;
            }

            /// Overloaded operator++() function
            const Iterator operator++(int) 
            {
                ConstIterator temp(iter_);
                ++(*this);
                return temp;
            }

            /// Overloaded operator--() function
            ConstIterator & operator--()
            {
                --iter_;
                return *this;
            }
        };

        typedef Iterator iterator;
        typedef ConstIterator const_iterator;

        /// Constructor
        NBUnorderedMap() : ckptSize_(0), ckptEntries_(NULL) { }

        /// Destructor
        ~NBUnorderedMap()
        {
            if (ckptEntries_ != NULL) {
                free(ckptEntries_);
            }
        }

        /// Wrapper function for unordered_map::insert()
        std::pair<iterator, bool> insert(const value_type & value)
        {
            std::pair<entry_iterator, bool> pos = map_.insert(std::make_pair(value.first, Entry()));
            if (pos.second == true) {
                Entry & entry = pos.first->second;
                entry.bits_ = 0x00; // dirty bit = 0, use copy 0
                entry.data_[0] = new value_type(value);
                entry.data_[1] = new value_type(value);
            }
            return std::make_pair<iterator, bool>(iterator(pos.first), pos.second);
        }

        /// Wrapper function for unordered_map::erase()
        iterator erase(iterator pos)
        {
            // TODO: rease() can be supported by using an aditional bit to flag
            // whether the key is deleted, and actually delete it in next call
            // to prepareForNonBlockingCheckpoint()
            throw "erase() is not supported";
        }

        /// Wrapper function for unordered_map::find()
        iterator find(const Key & key)
        {
            entry_iterator pos = map_.find(key);
            if (pos == map_.end() || // key does not exist
                pos->second.bits_ & 0x02) { // already dirty
                return iterator(pos);
            }
            else { // copy on first write
                // copy data
                Entry & entry = pos->second;
                uint8_t srcCopy = entry.bits_;
                entry.data_[1-srcCopy]->second = entry.data_[srcCopy]->second; 
                // flip dirty bit from 0 to 1, flip the copy used for foreground computation
                entry.bits_ ^= 0x03;
                return iterator(pos);
            }
        }

        /// Wrapper function for unordered_map::find()
        const_iterator find(const Key & key) const
        {
            return const_iterator(map_.find(key));
        }

        /// Wrapper function for unordered_map::clear()
        void clear()
        {
            // NOTE: there may be race condition. call clear() when there is 
            // no background non-blocking checkpointing
            map_.clear();
        }

        /// Wrapper function for unordered_map::begin()
        iterator begin()
        {
            return iterator(map_.begin());
        }

        /// Wrapper function for unordered_map::begin()
        const_iterator begin() const
        {
            return const_iterator(map_.begin());
        }

        /// Wrapper function for unordered_map::end()
        iterator end()
        {
            entry_iterator it = map_.end();
            return iterator(it);
        }

        /// Wrapper function for unordered_map::end()
        const_iterator end() const
        {
            return const_iterator(map_.end());
        }

        /// Wrapper function for unordered_map::size()
        size_type size() const
        {
            return map_.size();
        }

        /// Wrapper function for unordered_map::empty()
        bool empty() const
        {
            return map_.empty();
        }

        /// Prepare for non-blocking checkpointing.
        /// When this function is called, ther is no ongoing computation or checkpointing.
        /// @param seqId checkpoint sequence ID
        void prepareForNonBlockingCheckpoint(int64_t)
        {
            if (ckptSize_ < map_.size()) {
                ckptEntries_ = (Entry *)realloc(ckptEntries_, map_.size() * sizeof(Entry));
            }
            // for each key in entry map, copy the entry and clear dirty bit
            int i = 0;
            for (entry_iterator it = map_.begin(); it != map_.end(); ++it, i ++) {
                it->second.bits_ &= 0xfd; // clear dirty bit
                ckptEntries_[i] = it->second;
            }
            ckptSize_ = map_.size();
        }

        /// serialize data to the given Checkpoint
        /// @param ckpt Checkpoint handle
        void serialize(Checkpoint & ckpt)
        {
            ckpt.addUInt32(ckptSize_);
            for(uint32_t i = 0; i < ckptSize_; i ++) {
                // which copy to use
                uint8_t index = ckptEntries_[i].bits_;
                value_type * pr = ckptEntries_[i].data_[index];
                ckpt << pr->first;
                ckpt << pr->second;
            }
        }

        /// deserialize data from the given Checkpoint
        /// @param ckpt Checkpoint handle
        void deserialize(Checkpoint & ckpt)
        {
            map_.clear();
            uint32_t totalSize = ckpt.getUInt32();
            std::pair<Key, T> pr;
            for(; totalSize > 0; totalSize --) {
                ckpt >> pr.first;
                ckpt >> pr.second;
                insert(pr);
            }
        }

    private:
        typedef std::tr1::unordered_map<Key, Entry> entry_map_type;
        typedef typename entry_map_type::iterator entry_iterator;
        typedef typename entry_map_type::const_iterator const_entry_iterator;

        entry_map_type map_;    // internal unordered map handle
        unsigned int ckptSize_; // number of items to checkpoint
        Entry * ckptEntries_;   // a copy of entries to be checkpointed
    };

    /// Overloaded operator << () function for serializing NBUnorderedMap
    /// @param ckpt a Checkpoint handle
    /// @param data a NBUnorderedMap to serialize
    /// @throws DataStoreException if there is any error during serialization
    template<typename Key, typename T>
    Checkpoint & operator << (Checkpoint & ckpt, const NBUnorderedMap<Key, T> & data)
    {
        const_cast<NBUnorderedMap<Key, T> &>(data).serialize(ckpt);
        return ckpt;
    }

    /// Overloaded operator >> () function for deserializing NBUnorderedMap
    /// @param ckpt a Checkpoint handle
    /// @param data return deserialized data
    /// @throws DataStoreException if there is any error during deserialization
    template<typename Key, typename T>
    Checkpoint & operator >> (Checkpoint & ckpt, NBUnorderedMap<Key, T> & data)
    {
        data.deserialize(ckpt);
        return ckpt;
    }

    /// Overloaded operator << () function for serializing NBDeQue
    /// @param ckpt an SPL::ByteBuffer<Checkpoint> handle
    /// @param data a NBUnorderedMap to serialize
    /// @throws DataStoreException if there is any error during serialization
    template<typename Key, typename T>
    SPL::ByteBuffer<Checkpoint> & operator << (SPL::ByteBuffer<Checkpoint> & ckpt, const NBUnorderedMap<Key, T> & data)
    {
        const_cast<NBUnorderedMap<Key, T> &>(data).serialize(static_cast<Checkpoint&>(ckpt));
        return ckpt;
    }

    /// Overloaded operator >> () function for deserializing NBUnorderedMap
    /// @param ckpt an SPL::ByteBuffer<Checkpoint> handle
    /// @param data return deserialized data
    /// @throws DataStoreException if there is any error during deserialization
    template<typename Key, typename T>
    SPL::ByteBuffer<Checkpoint> & operator >> (SPL::ByteBuffer<Checkpoint> & ckpt, NBUnorderedMap<Key, T> & data)
    {
        data.deserialize(static_cast<Checkpoint&>(ckpt));
        return ckpt;
    }
} // namespace NBCkpt

#endif // SPL_RUNTIME_OPERATOR_STATE_NB_UNORDERED_MAP_H

