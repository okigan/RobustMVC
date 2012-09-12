#pragma once


#include "../Model.h"

class QuadModel : public Model
{
public:
    QuadModel(void);
    ~QuadModel(void);

public:

    double GetRadius() const;
    void SetRadius(double radius);

private:
    double _radius;

};

