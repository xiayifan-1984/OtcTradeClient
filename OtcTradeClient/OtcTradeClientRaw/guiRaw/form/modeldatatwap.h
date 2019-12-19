#ifndef MODELDATATWAP_DEFINE_H
#define MODELDATATWAP_DEFINE_H

#include "modeldataorder.h"

class ModelTwapOrder: public ModelSingleOrder
{
public:
    ModelTwapOrder(QObject *parent = nullptr);
    ~ModelTwapOrder();

public:
    void        setTwapMgr(std::string twapRef, QOrderMgr* p);

private:
    /* data */

};













#endif

