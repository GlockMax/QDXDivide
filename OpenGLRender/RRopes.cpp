#include "stdafx.h"
#include <array>
#include <vector>
#include "BLinesDCEL.h"
#include "BPoint.h"
#include "RRopes.h"

RRopes::RRopes()
{
	VBO = 0;
}

RRopes::~RRopes()
{
	DeleteVBO();
}

void RRopes::Draw()
{
	if (starts_.empty())
		return;
    if (VBO == 0)
    {
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        if (!glIsBuffer(VBO))
            return;

        glBufferData(GL_ARRAY_BUFFER, sizeof(points_[0]) * points_.size(), points_.data(), GL_STATIC_DRAW);
    }
    else
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, NULL);
    for (int i = 0; i < starts_.size(); ++i)
    {
        const int nextStart = int((starts_.size() <= i + 1) ? points_.size() / 3 : starts_[i + 1]);
        glDrawArrays(GL_LINE_STRIP, starts_.at(i), nextStart - starts_.at(i));
    }
    glPopClientAttrib();
}

void RRopes::Prepare(size_t pointsCount)
{
	points_.resize(pointsCount * 3);
}

bool RRopes::CreateFromLines(BLinesDCEL& LinesDCEL)
{
    if (LinesDCEL.Size() == 0)
        return false;
    LinesDCEL.FilterLines();// delete extra edges. Mark all deleted
    auto markBuf = LinesDCEL.GetMarks();// store marks
    UINT curStart = 0;
    UINT outCount = 0;
    while (curStart != LinesDCEL.Size())
    {
        // find start
        for (; curStart < LinesDCEL.Size(); curStart += 2)
            if (!(LinesDCEL.IsMarked(curStart) || LinesDCEL.IsMarked(curStart + 1)))
                break;
        if (curStart == LinesDCEL.Size())
            break;
        // END:find start
        AddStartInd(outCount);
        // traverse to rope end
        UINT ropeCount = 0;// a number of edges in this rope
        bool CheckCycle = false;
        size_t cur = curStart;
        for (; cur != UINT_MAX && !(CheckCycle && cur == curStart); cur = LinesDCEL.NextHEdge(cur))
        {
            CheckCycle = true;
            if (++ropeCount >= LinesDCEL.Size())
                return false;// loop protection
            LinesDCEL.Mark(cur);
        }
        if (cur != curStart)// cycle was not detected
        {
            LinesDCEL.Unmark(curStart);
            // traverse to rope start
            for (size_t fcur = curStart + 1; fcur != UINT_MAX; fcur = LinesDCEL.NextHEdge(fcur))
            {
                if (++ropeCount >= LinesDCEL.Size())
                    return false;// loop protection
                LinesDCEL.Mark(fcur);
            }
            --ropeCount; // start edge was counted twice
        }
        else
        {
            // mark cycle
            starts_.back() = -1 * (starts_.back() + 1);
        }
        outCount += ropeCount;
        if (outCount > LinesDCEL.Size())
            return false;// loop protection
    }
    curStart = 0;// must be even
    LinesDCEL.RestoreMarks(markBuf);// restore original marks
    Prepare(outCount + starts_.size());// one extra point for each rope
    size_t mainIndex = 0;
    for (size_t i = 0; i < starts_.size(); ++i)
    {
        // find next start
        for (; curStart < LinesDCEL.Size(); curStart += 2)
            if (!(LinesDCEL.IsMarked(curStart) || LinesDCEL.IsMarked(curStart + 1)))
                break;
        bool cycle = false;
        if (starts_.at(i) < 0)// cycle detected
        {
            // unmark cycle
            starts_.at(i) = -1 * (starts_.at(i) + 1);
            cycle = true;
        }
        // store one rope and mark stored
        size_t cur = curStart + 1;
        if (!cycle)
        {
            // traverse to rope start
            for (size_t in = cur, cnt = 0; in != UINT_MAX; in = LinesDCEL.NextHEdge(cur))
            {
                if (++cnt >= LinesDCEL.Size())
                    return false;// loop protection
                cur = in;
            }
        }
        // traverse to rope end
        size_t count = starts_.at(i);
        const size_t countMax = count + GetRopeCount(i);
        // change traverse direction
        cur = LinesDCEL.PairedHalfEdge(cur);
        for (; ; ++count)
        {
            LinesDCEL.Mark(cur);
            points_[mainIndex++] = LinesDCEL[cur].GetPoint()[0];
            points_[mainIndex++] = LinesDCEL[cur].GetPoint()[1];
            points_[mainIndex++] = LinesDCEL[cur].GetPoint()[2];
            auto buf = LinesDCEL.NextHEdge(cur);
            if (buf == UINT_MAX || count == countMax - 1)
            {
                // store last point
                auto last = LinesDCEL.PairedHalfEdge(cur);
                points_[mainIndex++] = LinesDCEL[last].GetPoint()[0];
                points_[mainIndex++] = LinesDCEL[last].GetPoint()[1];
                points_[mainIndex++] = LinesDCEL[last].GetPoint()[2];
                break;
            }
            cur = buf;
        }
    }
    for (int l = 0; l < starts_.size(); ++l)
        starts_[l] += l;// each rope have one point more than lines count
    return true;
}

size_t RRopes::GetRopeCount(size_t ind) const
{
    int nextStart = (starts_.size() <= ind + 1) ? int(points_.size() / 3 - starts_.size()) : starts_[ind + 1];
    nextStart = nextStart < 0 ? -nextStart - 1 : nextStart;
    return nextStart - starts_[ind];
}

void RRopes::DeleteVBO()
{
    if (VBO != 0)
    {
        VBO2Delete.push(VBO);// Needed to ensure deleting of a VBO in the main thread only
        VBO = 0;
    }
}

bool RRopes::GetSegments(SnapRopes& oSegments) const
{
    if (IsEmpty())
        return true;
    const auto oldSize = oSegments.size();
    oSegments.reserve(oldSize + starts_.size());
    for (int i = 0; i < starts_.size(); ++i)
    {
        oSegments.emplace_back();
        const int nextStart = (starts_.size() <= i + 1) ? int(points_.size() / 3) : starts_[i + 1];
        oSegments.back().reserve(nextStart - starts_.at(i));
        for (int k = starts_.at(i), j = 0; k < nextStart; ++k, ++j)
        {
            oSegments.at(i + oldSize).emplace_back(points_.at(k * 3), points_.at(k * 3 + 1), points_.at(k * 3 + 2), 1.);
        }
    }
    return true;
}

void RRopes::Transform(const BMatr& M)
{
    for (size_t i = 0; i < points_.size(); i += 3)
    {
        BPoint p(points_[i], points_[i + 1], points_[i + 2], 1.);
        p *= M;
        points_[i] = float(p.GetX());
        points_[i + 1] = float(p.GetY());
        points_[i + 2] = float(p.GetZ());
    }
}

