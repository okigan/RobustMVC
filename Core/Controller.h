#pragma once

class Model;

class Controller
{
public:
    Controller( void );
    virtual ~Controller( void );

    virtual Model* GetModel( ) const = 0;
    virtual void SetModel( Model* val ) = 0;

    virtual bool IsActionEnabled( int action ) = 0;
};

