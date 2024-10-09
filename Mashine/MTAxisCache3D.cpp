#include "stdafx.h"
#include "MTAxisCache3D.h"

void* EMPTY = (void*)(NULL + 1);

MTAxisCache3D::MTAxisCache3D()
{
    Init();
}

MTAxisCache3D::~MTAxisCache3D()
{
    Clear();
}

void MTAxisCache3D::Reset()
{
    Clear();
    Init();
}

void MTAxisCache3D::Clear()
{
    for each (auto & d2 in storage_)
    {
        for each (auto & d3 in *d2)
            delete d3;
        delete d2;
    }
}

void MTAxisCache3D::Init()
{
    storage_.resize(1);
    storage_.at(0) = new Data2(1);
    storage_.at(0)->at(0) = new Data3(1);
    storage_.at(0)->at(0)->at(0) = EMPTY;
}

void MTAxisCache3D::Set(int i, int j, int k, void* newValue)
{
    (*(*storage_[i])[j])[k] = newValue;
}

void* MTAxisCache3D::Get(int i, int j, int k, bool& needNewValue)
{
    needNewValue = false;
    if (i >= storage_.size())
    {
        needNewValue = true;
        const auto oldSize = storage_.size();
        storage_.resize(i + 1);
        for (auto l = oldSize; l < storage_.size(); ++l)
        {
            storage_[l] = new Data2(storage_[0]->size());
            for (auto m = 0; m < storage_[0]->size(); ++m)
            {
                (*storage_[l])[m] = new Data3((*storage_[0])[0]->size());
                for (auto n = 0; n < (*storage_[0])[0]->size(); ++n)
                    (*(*storage_[l])[m])[n] = EMPTY;
            }
        }
    }
    if (j >= storage_[i]->size())
    {
        needNewValue = true;
        const auto oldSize = storage_[i]->size();
        storage_[i]->resize(j + 1);
        for (auto m = oldSize; m < storage_[i]->size(); ++m)
        {
            (*storage_[i])[m] = new Data3((*storage_[i])[0]->size());
            for (auto n = 0; n < (*storage_[i])[0]->size(); ++n)
                (*(*storage_[i])[m])[n] = EMPTY;
        }
    }
    if (k >= (*storage_[i])[j]->size())
    {
        needNewValue = true;
        const auto oldSize = (*storage_[i])[j]->size();
        (*storage_[i])[j]->resize(k + 1);
        for (auto n = oldSize; n < (*storage_[i])[j]->size(); ++n)
            (*(*storage_[i])[j])[n] = EMPTY;
    }
    void* ret = (*(*storage_[i])[j])[k];
    if (ret == EMPTY)
        needNewValue = true;
    return ret;
}
