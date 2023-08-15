#pragma once
#include "CoreMinimal.h"

class MyRunnable: public FRunnable
{
public:
	MyRunnable();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;
	
};
