#pragma once
#include <format>
#include <string>
#include <sstream>

namespace leo
{
	class Assertion
	{
	public:
		enum class Consequence
		{
			Log,
			Terminate
		};
	public:
		Assertion(const std::string& expression, const char* file, const char* functionName, int line, Consequence con);
		~Assertion();
	public:
		Assertion& note(const std::string& message);

		template<typename T>
		Assertion& watch(T&& val, const char* name)
		{
			m_stream << name << " => " << std::forward<T>(val) << "\n";
			return *this;
		}
	private:
		const char* m_sourceFile;
		const char* m_sourceFunctionName;
		int m_sourceLine;
		Consequence m_consequence;

		std::ostringstream m_stream;
	};

}


#define ZZ_STR_(x) #x  
#define ZL_STR(x) ZZ_STR_(x)

#if !PRODUCTION_BUILD
	#define LEOASSERT(expr, msg) (bool(expr)) ? void(0) : (void)leo::Assertion{ ZL_STR(expr), __FILE__, __FUNCTION__, __LINE__, leo::Assertion::Consequence::Terminate }.note(msg)
	#define LEOCHECK(expr, msg)  (bool(expr)) ? void(0) : (void)leo::Assertion{ ZL_STR(expr), __FILE__, __FUNCTION__, __LINE__, leo::Assertion::Consequence::Log }.note(msg)

	#define LEOASSERTF(expr, msg, ...) LEOASSERT(expr, std::format(msg, __VA_ARGS__))
	#define LEOCHECKF(expr, msg, ...)  LEOCHECK(expr,  std::format(msg, __VA_ARGS__))

	#define LEOWATCH(var) watch((var), ZL_STR(var))
#else
	#define LEOASSERTF(expr, msg, ...)
	#define LEOCHECKF(expr, msg, ...)

	#define LEOASSERT(expr, msg, ...)
	#define LEOCHECK(expr, msg, ...)

	#define LEOWATCH(var)
#endif
