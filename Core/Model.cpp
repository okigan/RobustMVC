#include "StdAfx.h"
#include "Model.h"


model::model( void )
{
    _change_stamp = 0;
}


model::~model( void )
{
}

void model::_NotifyPropertyChanging( int property_id )
{
    if( NULL != _callback )
    {
        callback_params params = {this, e_changing, property_id};
        _callback( params );
    }
}

void model::_NotifyPropertyChanged( int property_id )
{
    _Touch();

    if( NULL != _callback )
    {
        callback_params params = { this, e_changed, property_id };
        _callback( params );
    }
}

void model::SetCallback( Callback & callback )
{
    _callback = callback;
}

void model::_Touch( )
{
    _change_stamp++;
}

unsigned char model::GetChangeStamp() const
{
    return _change_stamp;
}
