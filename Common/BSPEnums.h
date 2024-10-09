#pragma once

enum M_POSITION
{
	M_NEG = -1,// - all points are in negative s/s
	M_POS = +1,// - all points are in positive s/s
	M_INT =  0,// - intersection found
	M_EMP = -2,// - area is empty
	M_IDN = -3,// - planes are identical
	M_ERR = -100// error
};
enum M_PTR
{
	M_IN_PTR = NULL,
	M_OUT_PTR = NULL+1,
	M_NDEF_PTR = NULL+2
};
