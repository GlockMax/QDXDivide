#include "stdafx.h"
#include "ConstDef.h"
#include "BPoint.h"
#include "RRopes.h"
#include "BLinesDCEL.h"

void BLinesDCEL::Clear()
{
	edges_.clear();
}

void BLinesDCEL::ReplaceEdge(size_t FirstInd, const float* p0, const float* p1, UINT ne0, UINT ne1)
{
	edges_[FirstInd].pP_ = p0;
	edges_[FirstInd].nE1_ = ne1;
	edges_[FirstInd].shift_ = +1;
	edges_[FirstInd + 1].pP_ = p1;
	edges_[FirstInd + 1].nE1_ = ne0;
	edges_[FirstInd + 1].shift_ = -1;
}

size_t BLinesDCEL::NextHEdge(size_t heIndex) const
{
	return edges_[heIndex].nE1_;
}

bool BLinesDCEL::Connect(UINT ind0, UINT ind1)
{
    // Join 2 edges
    // it is assumed that edges have common point and indexes are correct
    UINT shift0 = 0;
    UINT shift1 = 0;
    if (edges_[ind0 + shift0].GetPoint() != edges_[ind1 + shift1].GetPoint())
    {
        shift0 = 1;
        if (edges_[ind0 + shift0].GetPoint() != edges_[ind1 + shift1].GetPoint())
        {
            shift1 = 1;
            if (edges_[ind0 + shift0].GetPoint() != edges_[ind1 + shift1].GetPoint())
            {
                shift0 = 0;
                if (edges_[ind0 + shift0].GetPoint() != edges_[ind1 + shift1].GetPoint())
                {// internal error
                    return false;
                }
            }
        }
    }
    // shift points to the half edges with the same start points now
    edges_[ind0 + 1 - shift0].nE1_ = ind1 + shift1;
    edges_[ind1 + 1 - shift1].nE1_ = ind0 + shift0;
    return true;
}

void BLinesDCEL::Bind(UINT ind0, UINT ind1)
{
    // join 2 edges
    // ind0 and ind1 point to half edges with same start point
    UINT ind0p = PairedHalfEdge(ind0);
    UINT ind1p = PairedHalfEdge(ind1);
    edges_[ind0p].nE1_ = ind1;
    edges_[ind1p].nE1_ = ind0;
}

void BLinesDCEL::RemoveAllMarks()
{
    for (auto m = marks_.begin(); m != marks_.end(); ++m)
        *m = false;
}

size_t BLinesDCEL::PairedHalfEdge(size_t halfEdge) const
{
    // Returns a number of paired half edge or -1 if the edge doesn't exist
    size_t identShift = edges_[halfEdge].shift_;
    return halfEdge + identShift;
}

void BLinesDCEL::DeleteEdge(size_t ind)
{
    // remove start point of paired (to ind) half edge, i.e. end point of ind half edge. Start point of next half edge is same too.
    // ind points to one of half edges
 
    if (ind == UINT_MAX)
        return;
    const auto next = NextHEdge(ind);
    const auto pair = PairedHalfEdge(ind);
    const auto prev = NextHEdge(pair);
    const auto prevpair = prev == UINT_MAX ? UINT_MAX : PairedHalfEdge(prev);
    const auto nextpair = next == UINT_MAX ? UINT_MAX : PairedHalfEdge(next);
    if (nextpair != UINT_MAX)
    {
        edges_.at(nextpair).nE1_ = edges_.at(pair).nE1_;
        edges_.at(next).pP_ = edges_.at(ind).pP_;
    }
    if (prevpair != UINT_MAX)
        edges_.at(prevpair).nE1_ = edges_.at(ind).nE1_;
    Mark(ind);
}

bool BLinesDCEL::TryRemove(size_t ind, double tol)
{
    const auto next = NextHEdge(ind);
    if (next != UINT_MAX && next > ind)
    {
        auto p = edges_.at(ind).GetPoint();
        BPoint q0(p[0], p[1], p[2], 1.);
        p = edges_.at(PairedHalfEdge(next)).GetPoint();
        BPoint q1(p[0], p[1], p[2], 1.);
        p = edges_.at(next).GetPoint();
        BPoint q(p[0], p[1], p[2], 1.);
        double dist2 = q.LineD2(q0, q1);
        if (dist2 < tol * tol)
        {
            DeleteEdge(ind);
            return true;
        }
    }
    return false;
}

void BLinesDCEL::FilterLines()
{
    // delete extra edges
    for (UINT i = 0; i < Size(); i += 2)
    {
        bool removed = false;
        // check next edge
        if (!TryRemove(i, MIND))
            TryRemove(PairedHalfEdge(i), MIND);
    }
}

