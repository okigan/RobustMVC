#pragma once

#include <memory>
#include <map>
#include <unordered_map>
#include <string>

#include <boost/any.hpp>

class dictionary
{
public:
    dictionary(void);
    ~dictionary(void);

    bool insert( const std::string & key, boost::any & value );

    bool find( const std::string & key, boost::any & value );

    const boost::any & find( const std::string & key );

    size_t size() const;

public:
    template <typename T>
    bool insert( const std::string & key, T & value );

    template <typename T>
    bool insert_as_weak_ptr( const std::string & key, std::shared_ptr<typename T> & ptr );

protected:

#ifdef _DEBUG
    std::map
#else
    std::unordered_map
#endif
        <std::string, boost::any> _property_bag;
};

template <typename T>
bool dictionary::insert( const std::string & key, T & value )
{
    return insert(key, boost::any(value));
}

template <typename T>
bool dictionary::insert_as_weak_ptr( const std::string & key, std::shared_ptr<typename T> & ptr )
{
    std::weak_ptr<typename T> wptr = ptr;

    return insert(key, boost::any(wptr));
}

