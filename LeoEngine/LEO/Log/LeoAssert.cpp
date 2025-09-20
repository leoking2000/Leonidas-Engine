#include "Log.h"

namespace LEO
{
	Assertion::Assertion(const std::string& expression, const char* file, const char* functionName, int line, Consequence con)
		:
		m_sourceFile(file),
		m_sourceFunctionName(functionName),
		m_sourceLine(line),
		m_consequence(con)
	{
		m_stream << "Assertion Failed! " << expression << "\n";
	}

	Assertion::~Assertion()
	{
		EntryBuilder{ m_sourceFile, m_sourceFunctionName, m_sourceLine }
			.SetChannel(&GetDefaultLogChannel())
			.SetLogLevel(m_consequence == Consequence::Terminate ? LogLevel::FATAL : LogLevel::ERROR)
			.SetMessage(m_stream.str());

		if (m_consequence == Consequence::Terminate)
		{
			std::terminate();
		}
	}

	Assertion& Assertion::note(const std::string& message)
	{
		m_stream << message << "\n";
		return *this;
	}

}

