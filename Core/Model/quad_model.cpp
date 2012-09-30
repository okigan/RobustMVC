#include "StdAfx.h"
#include "quad_model.h"


quad_model::quad_model(void)
{
    _radius = 0.8;
}

quad_model::~quad_model(void)
{
}

double quad_model::get_radius() const
{
    return _radius;
}

/** some info
    @callgraph
*/
void quad_model::set_radius( double radius )
{
    _NotifyPropertyChanging(e_radius);

    _radius = radius;

    _NotifyPropertyChanged(e_radius);
}
