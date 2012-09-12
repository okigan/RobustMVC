#include "StdAfx.h"
#include "QuadModel.h"


QuadModel::QuadModel(void)
{
    _radius = 0.5;
}

QuadModel::~QuadModel(void)
{
}

double QuadModel::GetRadius() const
{
    return _radius;
}

void QuadModel::SetRadius( double radius )
{
    _NotifyPropertyChanging(e_radius);

    _radius = radius;

    _NotifyPropertyChanged(e_radius);
}
