#include "stdafx.h"

using namespace dml;

LTDouble LTDouble::Cos(bool bConvertToRadians) const
{
	LTDouble val(loose, tight);

	if (bConvertToRadians)
	{
		val.loose *= (M_PI / 180);
		val.tight *= (M_PI / 180);
	}

	val.loose = cos(val.loose);
	val.tight = cos(val.tight);

	return val;
}