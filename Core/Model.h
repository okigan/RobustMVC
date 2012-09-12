#pragma once

#include <functional>
#include <stdint.h>


class Model
{
public:
    enum stage
    {
        e_changing,
        e_changed,
    };

    struct callback_params
    {
        Model*  model;
        stage   stage;
        int     property_id;
    };

    typedef std::function<void (const callback_params &)> Callback;

    void SetCallback(Callback & callback);

    uint8_t GetChangeStamp() const;

protected:
    Model(void);
    virtual ~Model(void);

    void _NotifyPropertyChanging(int property_id);
    void _NotifyPropertyChanged(int property_id);

private:
    void _Touch();

private:
    /// Change [time] stamp or hash of the change or implementation dependent
    /// Note: storage type is selected small enough so roll over happens in normal usage
    /// and application logic is ready to handle it (255 change stamps shall be sufficient)
    uint8_t  _change_stamp;


    Callback _callback;
};

