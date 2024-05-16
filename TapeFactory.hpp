#ifndef TAPE_FACTORY_H
#define TAPE_FACTORY_H

template<class T>
class TapeFactory {
public:
    virtual T create() = 0;
};

#endif
