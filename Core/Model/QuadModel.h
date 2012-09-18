#pragma once


#include "../Model.h"

/// QuadModel
class QuadModel : public Model
{
public:
    QuadModel(void);
    ~QuadModel(void);

    enum e_property_ids
    {
        e_radius,
    };

public:

    double GetRadius() const;
    void SetRadius(double radius);

private:
    double _radius;

};

