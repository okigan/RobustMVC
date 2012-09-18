#include "StdAfx.h"
#include "QuadModelController.h"

#include <Core/Model/QuadModel.h>

QuadModelController::QuadModelController(void)
{
}


QuadModelController::~QuadModelController(void)
{
}

Model* QuadModelController::GetModel() const
{
    return _model;
}

void QuadModelController::SetModel( QuadModel* val )
{
    _model = val;
}

void QuadModelController::IncreaseRadius()
{
    _model->SetRadius(_model->GetRadius() + 0.1);
}

void QuadModelController::DecreaseRadius()
{
    _model->SetRadius(_model->GetRadius() - 0.1);
}

bool QuadModelController::IsActionEnabled( int action )
{
    double current_radius = _model->GetRadius();

    switch( action )
    {
    case QuadModelController::e_increase_radius:
        return current_radius < 1.2;
        break;
    case QuadModelController::e_decrease_radius:
        return current_radius > 0.4;
        break;
    }

    return false;
}
