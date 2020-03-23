//
// Created by itay.tayar on 15/01/2020.
//
#ifndef CPP_EX3_HASHMAP_H

#include <vector>
#include <algorithm>
#include <iostream>

#define INITIAL_SIZE 16


template <typename KeyT, typename ValueT>
class HashMap
{

private:

    std::vector<std::pair<KeyT, ValueT>> *_map;

    int _size;

    int _capacity;

    double _upperLoadFactor;

    double _lowerLoadFactor;

    int _moduloCalculate(KeyT key) const
    {
        return key & (_size - 1);
    }

    auto _getValue(KeyT key, int mapIndex) const
    {
        return find_if( _map[mapIndex].begin(), _map[mapIndex].end(),
                           [&key](const std::pair<KeyT, ValueT>  &element) -> bool
                           { return element.first == key;} );
    }

public:
    // Constructors and Destructor

    HashMap(): _size(0), _capacity(INITIAL_SIZE), _upperLoadFactor(0.75), _lowerLoadFactor(0.25)
    {
        _map = new std::vector<std::pair<KeyT, ValueT>> [INITIAL_SIZE];
    }

    HashMap(const std::vector<KeyT> keys, const std::vector<ValueT> values)
    {

    }

    HashMap(const HashMap &map):  _size(map._size), _capacity(map._capacity), _upperLoadFactor(map._upperLoadFactor),
                                    _lowerLoadFactor(map._lowerLoadFactor)
    {
        HashMap new_map = map;
        return new_map;
    }

    ~HashMap()
    {
        delete[] _map;
    }

    // Methods

    int size() const
    {
        return _size;
    }

    int capacity() const
    {
        return _capacity;
    }

    bool empty()
    {
        return _size == 0;
    }


    bool insert(KeyT key, ValueT value)
    {
        if (containsKey(key))
        {
            this[key] = value;
        }
    }

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

    double ‫‪getLoadFactor‬‬() const
    {
        return _size/_capacity;
    }

    int bucketSize(const KeyT key) const
    {
        int mapIndex = bucketIndex(key);
        return _map[mapIndex].size();
    }


    int bucketIndex(const KeyT key) const
    {
        if (!containsKey(key))
        {
            std::cerr << "Invalid input" << std::endl;
            return false;
        }
        return _moduloCalculate(key);
    }

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

    KeyT at(const KeyT key) const
    {

    }

    bool erase(KeyT key)
    {
        if (!containsKey(key))
        {
            std::cerr << "Invalid input" << std::endl;
            return false;
        }
    }


    // Operators overloading

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

    KeyT operator[](KeyT key) const
    {
        int mapIndex = bucketIndex(key);
        return *_getValue(key, mapIndex);
    }

    ValueT &operator[](KeyT key)
    {
        int mapIndex = bucketIndex(key);
        return *_getValue(key, mapIndex);
    }


};

#define CPP_EX3_HASHMAP_H

#endif //CPP_EX3_HASHMAP_H
