#pragma once

#include "./detail/align.h"
#include "./detail/size.h"
#include "./environment.h"

NAGISA_BUILD_LIB_BEGIN

using details::align;
//using details::basic_policy;
using details::align_policy;
namespace policy
{
	using details::deduce;
	using details::recommend;
	using details::force;
}
using details::align_of;
using details::is_valid_align;

using details::offset_of;
using details::offset;
using details::size_of;
using details::layout;

NAGISA_BUILD_LIB_END