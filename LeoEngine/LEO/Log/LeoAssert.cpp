#include "Log.h"

namespace LEO::LOG
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
			.Channel(&GetDefaultLogChannel())
			.LogLevel(m_consequence == Consequence::Terminate ? Level::FATAL : Level::ERROR)
			.Note(m_stream.str());

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

