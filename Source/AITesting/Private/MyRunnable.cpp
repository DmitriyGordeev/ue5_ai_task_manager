#include "MyRunnable.h"

MyRunnable::MyRunnable() {}

bool MyRunnable::Init()
{
	UE_LOG(LogTemp, Log, TEXT("MyRunnable::Init() [start] "));

	// Blocking!
	FPlatformProcess::Sleep(2.0f);

	UE_LOG(LogTemp, Log, TEXT("MyRunnable::Init() [ends]"));
	return FRunnable::Init();
}

uint32 MyRunnable::Run()
{
	UE_LOG(LogTemp, Log, TEXT("MyRunnable::Run() [starts]"));

	// nonblocking
	FPlatformProcess::Sleep(1.0f);
	
	UE_LOG(LogTemp, Log, TEXT("MyRunnable::Run() [ends]"));
	return 0;
}

void MyRunnable::Exit()
{
	UE_LOG(LogTemp, Log, TEXT("MyRunnable::Exit() [starts]"));

	// nonblocking
	FPlatformProcess::Sleep(1.0f);
	
	UE_LOG(LogTemp, Log, TEXT("MyRunnable::Exit() [ends]"));
	FRunnable::Exit();
}

void MyRunnable::Stop()
{
	UE_LOG(LogTemp, Log, TEXT("MyRunnable::Stop()"));
	FRunnable::Stop();
}
