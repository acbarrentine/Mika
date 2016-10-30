#pragma once

class Identifier
{
protected:
	const char* mValue;

public:
	Identifier() : mValue(nullptr) {}
	explicit Identifier(const char* value) : mValue(value) {}

	bool operator ==(const Identifier& rhs) const
	{
		return mValue == rhs.mValue;
	}

	bool operator !=(const Identifier& rhs) const
	{
		return mValue != rhs.mValue;
	}

	bool operator !() const
	{
		return mValue == nullptr;
	}

	operator bool() const
	{
		return mValue != nullptr;
	}

	const char* GetString() const { return mValue; }

	bool operator<(const Identifier other)
	{
		return mValue < other.mValue;
	}
};

class StringTable
{
public:
	Identifier AddValue(std::string value);
	Identifier AddValue(const char* str);
	Identifier AddValue(const char* first, const char* last);
	Identifier ComposeValue(const char* pattern, ...);

	std::set<std::string> mStrings;
};
