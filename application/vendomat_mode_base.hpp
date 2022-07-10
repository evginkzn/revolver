#ifndef VENDOMAT_MODE_BASE_HPP
#define VENDOMAT_MODE_BASE_HPP

class VendomatModeBase
{
public:
    VendomatModeBase();
    ~VendomatModeBase(){}

public:
    virtual void init() = 0;
    virtual void deinit() = 0;
    
    virtual void tick() = 0;
};

#endif // ! VENDOMAT_MODE_BASE_HPP