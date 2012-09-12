#pragma once

#include <functional>


class Model
{
public:
    enum Stage
    {
        e_changing,
        e_changed,
    };

    struct callback_params
    {
        Model*  model;
        Stage   stage;
        int     property_id;
    };

    typedef std::function<void (const callback_params &)> Callback;

    void SetCallback(Callback & callback);

protected:
    Model(void);
    ~Model(void);

    void _NotifyPropertyChanging(int property_id);
    void _NotifyPropertyChanged(int property_id);

private:
    Callback _callback;
};

