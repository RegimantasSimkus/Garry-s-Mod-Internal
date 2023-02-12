#pragma once

class IAppSystem
{
public:
	// Here's where the app systems get to learn about each other 
	virtual bool Connect(void* factory) = 0;
	virtual void Disconnect() = 0;

	// Here's where systems can access other interfaces implemented by this object
	// Returns NULL if it doesn't implement the requested interface
	virtual void* QueryInterface(const char* pInterfaceName) = 0;

	// Init, shutdown
	virtual void* Init() = 0;
	virtual void Shutdown() = 0;
};