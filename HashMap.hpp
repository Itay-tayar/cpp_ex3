//
// Created by itay.tayar on 15/01/2020.
//
#ifndef CPP_EX3_HASHMAP_H
#define CPP_EX3_HASHMAP_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#define INVALID_USAGE std::cerr << "Usage: SpamDetector <database path> <massage path> <threshold>" << std::endl;
#define INVALID_INPUT std::cerr << "Invalid input" << std::endl;
#define EXIT exit(EXIT_FAILURE);

static const int INITIAL_SIZE = 16;
static const double INITIAL_UPPER_FACTOR = 0.75;
static const double INITIAL_LOWER_FACTOR = 0.25;
static const int RESIZE_FACTOR = 2;

/**
 * A class that represent hash map data structure
 * @tparam KeyT key
 * @tparam ValueT value
 */
template <typename KeyT, typename ValueT>
class HashMap
{

private:

    std::vector<std::pair<KeyT, ValueT>> *_map;

    int _size;

    int _capacity;

    double _upperLoadFactor;

    double _lowerLoadFactor;

    std::hash<KeyT> _hashFunc;

    /**
     *  calculate in which bucket the pair will be in
     * @param key
     * @return the index of the bucket
     */
    int _moduloCalculate(KeyT key) const
    {
        return _hashFunc(key) & (_size - 1);
    }

    /**
     *
     * @param key
     * @param mapIndex
     * @return
     */
    auto _getValue(KeyT key, int mapIndex) const
    {
        return find_if( _map[mapIndex].begin(), _map[mapIndex].end(),
                        [&key](const std::pair<KeyT, ValueT>  &element) -> bool
                        { return element.first == key;} );
    }

    /**
     * calculate the load factor of the hash map
     * @return load factor
     */
    double _loadFactor() const
    {
        return (double)_size / (double)_capacity;
    }

    /**
     * This function is making the rehashing process, it first check if the new capacity should be larger or smaller
     * and then resize it accordingly
     */
    void _reHashing()
    {
        if (_loadFactor() > _upperLoadFactor)  //larger then upper load factor
        {
            _capacity = RESIZE_FACTOR * _capacity;

        }
        if (_loadFactor() < _lowerLoadFactor) //smaller then upper load factor
        {
            _capacity =  _capacity / RESIZE_FACTOR;

        }
        std::vector<std::pair<KeyT, ValueT>> tempMap = new std::vector<std::pair<KeyT, ValueT>> [_capacity];

        HashMap<KeyT, ValueT>::ConstIterator iter;
        for (iter = cbegin(); iter != cend(); iter++) // insert all pairs to the resize map
        {
            int bucketIndex = _moduloCalculate(*iter->first);
            std::pair<KeyT, ValueT> pair = std::make_pair(*iter->first, *iter->second);
            tempMap[bucketIndex].push_back(pair);
        }
        _map = tempMap;
        delete[] tempMap;
    }

    /**
     * insert a pair of key and value to the hash map.
     * @param key
     * @param value
     */
    void _insert(KeyT key, ValueT value)
    {
        int index = _moduloCalculate(key);
        std::pair<KeyT, ValueT> pair = std::make_pair(key, value);
        _map[index].push_back(pair);
        ++_size;
        _reHashing();
    }

public:
    // Constructors and Destructor

    /**
     * default constructor which initialize empty hash map
     */
    HashMap(): _size(0), _capacity(INITIAL_SIZE), _upperLoadFactor(INITIAL_UPPER_FACTOR),
    _lowerLoadFactor(INITIAL_LOWER_FACTOR)
    {
        _map = new std::vector<std::pair<KeyT, ValueT>> [_capacity];
    }

    /**
     * constructor which initialize hash map with the given keys and value such that the i'th pair is key[i], values[i]
     * @param keys vector of keys
     * @param values  vector of values
     */
    HashMap(const std::vector<KeyT> keys, const std::vector<ValueT> values): HashMap()
    {
        if (keys.size() != values.size())
        {
            std::cerr << "Invalid input" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < keys.size(); ++i)
        {
            if (containsKey(keys[i])) // check if the key is already in the map, and replace his value
            {
                (*this)[keys[i]] = values[i];
            }
            else
            {
                _insert(keys[i], values[i]);
            }
        }
    }

    /**
     * copy constructor
     * @param map another hash map to copy from
     */
    HashMap(const HashMap &map):  _size(map._size), _capacity(map._capacity), _upperLoadFactor(map._upperLoadFactor),
                                  _lowerLoadFactor(map._lowerLoadFactor)
    {
        HashMap new_map = map;
        return new_map;
    }

    /**
     * Hash map destructor
     */
    ~HashMap()
    {
        delete[] _map;
    }

    // Methods

    /**
     *
     * @return the current amount of pairs inside the map
     */
    int size() const
    {
        return _size;
    }

    /**
     *
     * @return the max amount of pairs inside the map
     */
    int capacity() const
    {
        return _capacity;
    }

    /**
     * check in the hash map is empty
     * @return true it empty, false otherwise.
     */
    bool empty()
    {
        return _size == 0;
    }


    /**
     *
     * @param key
     * @param value
     * @return
     */
    bool insert(KeyT key, ValueT value)
    {
        if (containsKey(key))
        {
            std::cerr << "Invalid input" << std::endl;
            exit(EXIT_FAILURE);
        }
        _insert(key, value);
        return true;
    }


    /**
     * check if a given key is inside the hash map
     * @param key key to check
     * @return true if the key inside, flase otherwise.
     */
    bool containsKey(const KeyT key) const
    {
        if (empty())
        {
            return false;
        }
        int mapIndex = _moduloCalculate(key);
        auto it = _getValue(key, mapIndex);
        return it != _map[mapIndex].end();
    }

    /**
     * return the load factor of the hash map
     */
    double ‫getLoadFactor() const
    {
        return _loadFactor();
    }

    /**
     * get a key and check what is the size of the bucket it in
     * @param key key to check
     * @return ths bucket size
     */
    int bucketSize(const KeyT key) const
    {
        int mapIndex = bucketIndex(key);
        return _map[mapIndex].size();
    }


    /**
     * get a key and check what is the index of the bucket it in
     * @param key key to check
     * @return the index of the bucket
     */
    int bucketIndex(const KeyT key) const
    {
        if (!containsKey(key))
        {
            INVALID_INPUT
            EXIT
        }
        return _moduloCalculate(key);
    }

    /**
     * clear all the hash map without changing the capacity
     */
    void clear()
    {
        if (empty())
        {
            return;
        }
        for (int i = 0 ; i< _map->size(); ++i)
        {
            _map[i].clear();
        }
        _size = 0;
    }

    /**
     *
     * @param key
     * @return
     */
    KeyT at(const KeyT key) const
    {
        if (!containsKey(key))
        {
            std::cerr << "Invalid input" << std::endl;
            exit(EXIT_FAILURE);
        }
        int mapIndex = bucketIndex(key);
        for (auto const& element : _map[mapIndex])
        {
            if (element.first == key)
            {
                return element.second;
            }
        }

    }

    /**
     * remove a pair of key and value form the hash map according to the given key
     * @param key key of pair to remove
     * @return true if the pair removed successfully, false otherwise
     */
    bool erase(const KeyT key)
    {
        int index = bucketIndex(key);
        for (auto iter = _map[index].begin(); iter!=_map[index].end(); iter++)
        {
            if (iter->first == key) 
            {
                _map[index] . erase(iter);
                -- _size;
                _reHashing();
                return true;
            }
        }
        return false;
    }

    //Iterator

    /**
     *
     */
    class ConstIterator
    {
            

    private:

        HashMap _hashMap;

        int _indexInBucket;

        int _mapInx;

        void _getNext()
        {
            if (_indexInBucket == _hashMap->_map[_mapInx].size() - 1)
            {
                _indexInBucket = 0;
                _mapInx++;
                while (_hashMap->_map[_mapInx].empty() && _mapInx != _hashMap.capacity())
                {
                    _mapInx++;
                }
            }
            else
            {
                _indexInBucket++;
            }
        }

    public:
        ConstIterator(HashMap<KeyT,ValueT> &map, int col=0, int row=0):
        _hashMap(map), _indexInBucket(col), _mapInx(row)
        {
            while(_hashMap->_map[_mapInx].empty())
            {
                _mapInx++;
            }
        }

        std::pair<KeyT, ValueT> &operator*() const
        {
            return _hashMap->_map[_mapInx].at(_indexInBucket);
        }

        std::pair<KeyT, ValueT> *operator->() const
        {
            return &(_hashMap->_map[_mapInx].at(_indexInBucket));
        }

        const ConstIterator operator++(int) const
        {
            ConstIterator tmp =  *this;
            (*this)++;
            return tmp;
        }

        ConstIterator &operator++()
        {
            _getNext();
            return *this;
        }

        bool operator==(ConstIterator const& other) const
        {
            return (_hashMap == other._hashMap && _indexInBucket == other._indexInBucket && _mapInx == other._mapInx);
        }

        bool operator!=(ConstIterator const& other) const
        {
            return !((*this) == other);
        }
    };

    /**
     * First iterator of the hash map
     * @return the iterator of the beginning of the map
     */
    ConstIterator cbegin() const
    {
        return ConstIterator(this);
    }

    /**
     * Last iterator of the hash map
     * @return the iterator of the end of the map
     */
    ConstIterator cend() const
    {
        return ConstIterator(this, 0, _capacity);
    }

    /**
     * First iterator of the hash map
     * @return the iterator of the beginning of the map
     */
    ConstIterator begin() const
    {
        return ConstIterator(this);
    }

    /**
     * Last iterator of the hash map
     * @return the iterator of the end of the map
     */
    ConstIterator end() const
    {
        return ConstIterator(this, 0, _capacity);
    }


    // Operators overloading

    /**
     *
     * @param other
     * @return
     */
    HashMap &operator=(const HashMap< KeyT,ValueT> &other)
    {
        _size = other._size;
        _capacity = other._capacity;
        _lowerLoadFactor = other._lowerLoadFactor;
        _upperLoadFactor = other._upperLoadFactor;

        for (int i = 0; i < other._map->size(); ++ i)
        {
            _map[i] = other._map[i];
        }

        return *this;
    }

    /**
     *
     * @param key
     * @return
     */
    ValueT operator[](KeyT key) const
    {
        int mapIndex = bucketIndex(key);
        for (auto const& element : _map[mapIndex])
        {
            if (element.first == key)
            {
                return element.second;
            }
        }
        ValueT val = ValueT();
        _map[mapIndex].push_back(std::make_pair(key, val));
        return val;
    }

    /**
     *
     * @param key
     * @return
     */
    ValueT &operator[](KeyT key)
    {
        int mapIndex = bucketIndex(key);
        for (auto const& element : _map[mapIndex])
        {
            if (element.first == key)
            {
                return *element.second;
            }
        }
        ValueT *val = &ValueT();
        _map[mapIndex].push_back(std::make_pair(key, val));
        return val;
    }

    /**
     *
     * @param otherMap
     * @return
     */
    bool operator==(const HashMap &otherMap)
    {
        if (_size != otherMap._size)
        {
            return false;
        }
        HashMap<KeyT, ValueT>::ConstIterator it1 = cbegin();
        HashMap<KeyT, ValueT>::ConstIterator it2 = otherMap.cbegin();
        while (it1 != cend())
        {
            if (*it1 != *it2)
            {
                return false;
            }
            it1++;
            it2++;
        }
        return true;
    }

    /**
     *
     * @param otherMap
     * @return
     */
    bool operator!=(const HashMap &otherMap)
    {
        return !(this == otherMap);
    }
};



#endif //CPP_EX3_HASHMAP_H
