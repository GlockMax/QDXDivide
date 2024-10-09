#pragma once
#include "NCMB_API.h"
#include <vector>
#include "concurrent_vector.h"

using namespace concurrency;

class NCMB_API BLinesDCEL
{
	class HalfEdge
	{
		friend class BLinesDCEL;
	public:
		const float* GetPoint() const { return pP_; }
		void SetPoint(const float* p) { pP_ = p; }
		void ShiftCopy(const HalfEdge& src, UINT shift) { *this = src; if(nE1_ != UINT_MAX) nE1_ += shift; }
	protected:
		const float* pP_; //pointer to point
		UINT nE1_; // next edge index
		int shift_;// -1 - correspondent halfedge is previous/0 - is absent/1 - is next/ -2 - this half edge is dummy
	};

protected:
	concurrent_vector<HalfEdge> edges_;
	std::vector<bool> marks_;
protected:
	bool TryRemove(size_t ind, double tol);
public:
	void Clear();
	void ReplaceEdge(size_t firstInd, const float* p0, const float* p1, UINT ne0, UINT ne1);
	size_t NextHEdge(size_t heIndex) const;
	size_t Size() const { return edges_.size(); }
	void Reserve(size_t cap) { edges_.reserve(cap); marks_.reserve(cap);}
	void Resize(size_t cap) { edges_.resize(cap); marks_.resize(cap); }
	const HalfEdge& operator[](size_t i) const { return edges_[i]; }
	HalfEdge& operator[](size_t i) { return edges_[i]; }
	bool Connect(UINT ind0, UINT ind1);
	void Bind(UINT ind0, UINT ind1);
	void RemoveAllMarks();
	size_t PairedHalfEdge(size_t halfEdge) const;
	void Mark(size_t ind) { marks_[ind + edges_[ind].shift_] = true; marks_[ind] = true; }
	void Unmark(size_t ind) { marks_[ind] = false; marks_[ind + edges_[ind].shift_] = false; }
	bool IsMarked(size_t ind) { return marks_[ind]; }
	void FilterLines();
	void DeleteEdge(size_t ind);
	const auto& GetMarks() const { return marks_; }
	void RestoreMarks(const std::vector<bool>& marks) { marks_ = marks; }
};

