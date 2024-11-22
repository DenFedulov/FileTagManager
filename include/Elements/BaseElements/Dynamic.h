#pragma once

class Dynamic
{
protected:
    virtual void updateSurface() = 0;

public:
    virtual ~Dynamic() {};
};