#include "StdAfx.h"
#include "QuadModelController.h"

#include <Core/Model/QuadModel.h>

QuadModelController::QuadModelController(void)
{
}


QuadModelController::~QuadModelController(void)
{
}

QuadModel* QuadModelController::GetModel() const
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
