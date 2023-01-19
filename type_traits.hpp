#ifndef FT_TYPE_TRAITS_HPP
#define FT_TYPE_TRAITS_HPP

namespace ft
{
	template <bool B, class T = void >
	struct enable_if {};

	template <class T>
	struct enable_if<true, T>
	{
		typedef T type;
	};

	template <class T, T v>
	struct integral_constant
	{
		static const T value = v;
		typedef T value_type;
		typedef integral_constant<T, v> type;
		operator T() { return v; }
	};

	typedef integral_constant<bool, false>	false_type;
	typedef integral_constant<bool, true>	true_type;

	template <class _Tp>
	struct _Is_integral {
		typedef false_type		_Integral;
	};

	template<>
	struct _Is_integral<bool> {
		typedef true_type		_Integral;
	};

	template<>
	struct _Is_integral<char> {
		typedef true_type		_Integral;
	};

	template<>
	struct _Is_integral<signed char> {
		typedef true_type		_Integral;
	};

	template<>
	struct _Is_integral<unsigned char> {
		typedef true_type		_Integral;
	};

	template<>
	struct _Is_integral<wchar_t> {
		typedef true_type		_Integral;
	};

	template<>
	struct _Is_integral<short> {
		typedef true_type		_Integral;
	};

	template<>
	struct _Is_integral<unsigned short> {
		typedef true_type		_Integral;
	};

	template<>
	struct _Is_integral<int> {
		typedef true_type		_Integral;
	};

	template<>
	struct _Is_integral<unsigned int> {
		typedef true_type		_Integral;
	};

	template<>
	struct _Is_integral<long> {
		typedef true_type		_Integral;
	};

	template<>
	struct _Is_integral<unsigned long> {
		typedef true_type		_Integral;
	};

} // namespace ft

#endif

