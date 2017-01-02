#pragma once

class MikaString
{
protected:
	std::string mValue;
	
public:
	MikaString(const char* initialValue) : mValue(initialValue) {}
	MikaString(const MikaString& other) : mValue(other.mValue) {}
	MikaString(const std::string& other) : mValue(other) {}
	
	const char* GetCharPtr() const { return mValue.c_str(); }
	
	void operator+=(const MikaString& other)
	{
		mValue += other.mValue;
	}

	friend bool operator==(const MikaString& left, const MikaString& right)
	{
		return left.mValue == right.mValue;
	}

	friend bool operator!=(const MikaString& left, const MikaString& right)
	{
		return left.mValue != right.mValue;
	}

	friend bool operator<(const MikaString& left, const MikaString& right)
	{
		return left.mValue < right.mValue;
	}

	friend bool operator<=(const MikaString& left, const MikaString& right)
	{
		return left.mValue <= right.mValue;
	}

	friend bool operator>(const MikaString& left, MikaString& right)
	{
		return left.mValue > right.mValue;
	}

	friend bool operator>=(const MikaString& left, MikaString& right)
	{
		return left.mValue >= right.mValue;
	}

	friend MikaString operator+(const MikaString& left, const MikaString& right)
	{
		return MikaString(left.mValue + right.mValue);
	}
};
