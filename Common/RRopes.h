#pragma once
#include "BaseRender.h"
#include <vector>
#include "OPENGLRENDER_API.h"

class OPENGLRENDER_API RRopes :
    public BaseRender
{
public:
    RRopes();
    virtual ~RRopes();
    void Draw();
    void Prepare(size_t pointsCount);
    void AddStartInd(UINT ind) { starts_.push_back(ind); }
    void AddPoint(const float* p) { points_.push_back(p[0]); points_.push_back(p[1]); points_.push_back(p[2]); }
    bool CreateFromLines(class BLinesDCEL& LinesDCEL);
    void Clear() { points_.clear(); starts_.clear(); }
    size_t GetRopeCount(size_t ind) const;
    void DeleteVBO();
    bool IsEmpty() const { return starts_.empty(); }
    bool GetSegments(class SnapRopes& oSegments) const;
    void Transform(const class BMatr& M);
protected:
    std::vector<float> points_;
    std::vector<int> starts_;
    GLuint VBO;
};

