#pragma once
#include "BSP_MODEL_API.h"
#include "QDivideBase.h"
#include "DXSubModel5X.h"

class BSP_MODEL_API QDXDivide : public QDivideBase
{
public:
	QDXDivide(MBody* body);
	virtual ~QDXDivide();
	void SetPoint(BPoint point, BPoint view) override;
	void Revert() override;

private:
	DXSubModel5X* pb;
	void MarkBodies();

    enum DexelSectionType {
        Z_D,
        Y_D,
        X_D
    };
    // Член очереди, хранящий указатель на точку отрезка и тип отрезка декселя
    struct QueueMember {
        DXMemID p;
        DexelSectionType t;
        int i_n0;
        int i_n1;

        QueueMember(DXMemID pp, DexelSectionType tt, int i_n00, int i_n11) :
            p(pp), t(tt), i_n0(i_n00), i_n1(i_n11) {};

        bool operator==(const QueueMember& In) const {
            return ((p == In.p) && (t == In.t) && (i_n0 == In.i_n0) && (i_n1 == In.i_n1));
        }
        bool operator!=(const QueueMember& In) const {
            return ((p != In.p) && (t != In.t) && (i_n0 != In.i_n0) && (i_n1 != In.i_n1));
        }
    };

};

