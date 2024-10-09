#pragma once
#include "MTMatrFieldAxis.h"
class MTSimpleChainAxis :
    public MTMatrFieldAxis
{
    // 2 equal gears with the chain. VectorX - rotation axis direction. VectorY - axis to axis directon.
    // CenterDist - a distance between gears centers. CycleVal - a value of the Movement parametr in degrees to get full cycle
public:
    MTSimpleChainAxis();
    MTSimpleChainAxis(const MTSimpleChainAxis& Ax);
    const MTSimpleChainAxis& operator= (const MTSimpleChainAxis& Ax);
    ~MTSimpleChainAxis() override;
    MTSimpleChainAxis* Clone() const override;
    void CreateFieldMatr(BMatr& FieldMatr, double T) const override;
    double CalcT(const BPoint& P) const override;
    void SetCycleVal(double Val) { CycleVal = Val; }
    void SetCenterDist(double Val) { CenterDist = Val; }
    double GetCycleVal() const { return CycleVal; }
    double GetCenterDist() const { return CenterDist; }
    AXIS_TYPE GetType() const override;
protected:
    double CenterDist;
    double CycleVal;
};

