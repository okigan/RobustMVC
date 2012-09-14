#pragma once

#include <memory>
#include <map>
#include <unordered_map>
#include <string>

#include <boost/any.hpp>

class SharedPropertyBag
{
public:
    SharedPropertyBag(void);
    ~SharedPropertyBag(void);

    bool Put( const std::string & key, std::shared_ptr<boost::any> & value );

    bool Get( const std::string & key, std::shared_ptr<boost::any> & value );

    size_t Size();

protected:

#ifdef _DEBUG
    std::map
#else
    std::unordered_map
#endif
        <std::string, std::weak_ptr<boost::any>> _property_bag;
};

