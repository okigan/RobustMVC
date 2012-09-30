#pragma once


#include "../Model.h"

/// quad_model
class quad_model : public model
{
public:
    quad_model(void);
    ~quad_model(void);

    enum e_property_ids
    {
        e_radius,
    };

public:

    double get_radius() const;
    void set_radius(double radius);

private:
    double _radius;

};

