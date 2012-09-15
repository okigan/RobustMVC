#include "StdAfx.h"
#include "SharedPropertyBag.h"


SharedPropertyBag::SharedPropertyBag(void)
{
}


SharedPropertyBag::~SharedPropertyBag(void)
{
}

bool SharedPropertyBag::Put( const std::string & key, std::shared_ptr<boost::any> & value )
{
    auto wp = std::weak_ptr<boost::any>(value);

    auto p = _property_bag.insert(std::make_pair(key, wp));

    return p.second;
}

bool SharedPropertyBag::Get( const std::string & key, std::shared_ptr<boost::any> & value )
{
    auto it = _property_bag.find(key);

    if( _property_bag.end() != it ) {
        value = it->second.lock();
        return true;
    }

    return false;
}

std::shared_ptr<boost::any> SharedPropertyBag::Get( const std::string & key )
{
    std::shared_ptr<boost::any> value;

    Get(key, value);

    return value;


}

bool SharedPropertyBag::Get( const std::string & key, std::shared_ptr<const boost::any> & value ) const
{
    auto it = _property_bag.find(key);

    if( _property_bag.end() != it ) {
        value = it->second.lock();
        return true;
    }

    return false;
}


size_t SharedPropertyBag::Size()
{
    return _property_bag.size();
}
