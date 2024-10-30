#pragma once

#include "./align.h"
#include "./environment.h"

NAGISA_BUILD_LIB_DETAIL_BEGIN

template<align auto Align>
	requires (details::is_valid_align(Align))
constexpr ::std::size_t offset_of(::std::ranges::input_range auto const& sizes, ::std::size_t i)
	requires ::std::convertible_to<::std::ranges::range_value_t<decltype(sizes)>, ::std::size_t>
{
	if (i == 0)
		return 0;

	if constexpr (Align == 1) {
		return details::offset_of<Align>(sizes, i - 1) + static_cast<::std::size_t>(*::std::ranges::next(::std::ranges::begin(sizes), i - 1));
	}
	else {
		auto start = details::offset_of<Align>(sizes, i - 1) + static_cast<::std::size_t>(*::std::ranges::next(::std::ranges::begin(sizes), i - 1));
		auto size_i = static_cast<::std::size_t>(*::std::ranges::next(::std::ranges::begin(sizes), i));
		auto align_size = (size_i > Align ? Align : size_i);
		auto model = start % align_size;
		if (!model) return start;
		return start + align_size - model;
	}
}

template<align auto Align>
	requires (details::is_valid_align(Align))
constexpr auto offset(const ::std::ranges::input_range auto& sizes)
	requires ::std::convertible_to<::std::ranges::range_value_t<decltype(sizes)>, ::std::size_t>&& ::std::ranges::sized_range<decltype(sizes)>
{
	return ::std::views::iota(0, ::std::ranges::size(sizes)) | ::std::views::transform([sizes = ::std::views::all(sizes)](auto i)
		{
			return details::offset_of<Align>(sizes, i);
		});
}

template<align auto Align = 1>
	requires (details::is_valid_align(Align))
constexpr auto size_of(const ::std::ranges::input_range auto& sizes)
	requires ::std::convertible_to<::std::ranges::range_value_t<decltype(sizes)>, ::std::size_t>&& ::std::ranges::sized_range<decltype(sizes)>
{
	auto index = ::std::ranges::size(sizes) - 1;
	auto size = details::offset_of<Align>(sizes, index) + static_cast<::std::size_t>(*::std::ranges::next(::std::ranges::begin(sizes), index));
	auto model = size % Align;

	if (!model) return size;
	return size + Align - model;
}

template<align auto Align>
	requires (details::is_valid_align(Align))
constexpr auto layout(const ::std::ranges::input_range auto& sizes)
	requires ::std::convertible_to<::std::ranges::range_value_t<decltype(sizes)>, ::std::size_t>&& ::std::ranges::sized_range<decltype(sizes)>
{
	return ::std::views::iota(0, ::std::ranges::size(sizes)) | ::std::views::transform([sizes = ::std::views::all(sizes)](auto i)
		{
			return ::std::make_pair(
				static_cast<::std::size_t>(*::std::ranges::next(::std::ranges::begin(sizes), i)),
				details::offset_of<Align>(sizes, i)
			);
		});
}

NAGISA_BUILD_LIB_DETAIL_END