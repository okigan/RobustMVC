#pragma once

class QuadModel;

class QuadModelController
{
public:
    QuadModelController(void);
    ~QuadModelController(void);

    QuadModel* GetModel() const;
    void SetModel(QuadModel* val);
    void IncreaseRadius();
    void DecreaseRadius();
protected:
    QuadModel* _model;
};

