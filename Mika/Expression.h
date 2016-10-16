#pragma once

class Expression
{
protected:
	size_t mRootToken;
	
public:
	Expression(size_t rootToken)
		: mRootToken(rootToken)
	{}
};
