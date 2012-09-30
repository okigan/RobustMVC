#pragma once

class model;

class controller
{
public:
    controller( void );
    virtual ~controller( void );

    virtual model* get_model( ) const = 0;

    virtual bool is_action_enabled( int action_id ) = 0;
};

