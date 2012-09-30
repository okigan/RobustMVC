#include "StdAfx.h"
#include "dictionary.h"


dictionary::dictionary(void)
{
}


dictionary::~dictionary(void)
{
}

bool dictionary::insert( const std::string & key, boost::any & value )
{
    auto p = _property_bag.insert(std::make_pair(key, value));

    return p.second;
}

bool dictionary::find( const std::string & key, boost::any & value )
{
    auto it = _property_bag.find(key);

    if( _property_bag.end() != it ) {
        value = it->second;
        return true;
    }

    return false;
}

const boost::any & dictionary::find( const std::string & key )
{
    return _property_bag[key];
}

size_t dictionary::size() const
{
    return _property_bag.size();
}
