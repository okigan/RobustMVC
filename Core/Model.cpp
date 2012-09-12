#include "StdAfx.h"
#include "Model.h"


Model::Model( void )
{
    _change_stamp = 0;
}


Model::~Model( void )
{
}


void Model::_NotifyPropertyChanging( int property_id )
{
    if( NULL != _callback )
    {
        callback_params params = {this, e_changing, property_id};
        _callback( params );
    }
}

void Model::_NotifyPropertyChanged( int property_id )
{
    _Touch();

    if( NULL != _callback )
    {
        callback_params params = { this, e_changed, property_id };
        _callback( params );
    }
}

void Model::SetCallback( Callback & callback )
{
    _callback = callback;
}

void Model::_Touch( )
{
    _change_stamp++;
}

uint8_t Model::GetChangeStamp( ) const
{
    return _change_stamp;
}
