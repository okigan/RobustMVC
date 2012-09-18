#pragma once

class QuadModel;

#include "../Controller.h"

class QuadModelController : public Controller 
{
public:
    QuadModelController(void);
    ~QuadModelController(void);

public:
    enum e_action_ids
    {
        e_increase_radius,
        e_decrease_radius,
    };

    virtual Model* GetModel() const;
    virtual void SetModel(QuadModel* val);


    void IncreaseRadius();
    void DecreaseRadius();

    virtual bool IsActionEnabled( int action );

protected:
    QuadModel* _model;
};

