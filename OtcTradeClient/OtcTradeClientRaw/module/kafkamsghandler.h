#pragma once
#include "iinternalSenderReceiver.h"
#ifdef MYLIBDLL
#define MYLIBDLL extern "C" __declspec(dllimport)
#else
#define MYLIBDLL extern "C" __declspec(dllexport)
#endif

MYLIBDLL  IInternalSenderReceiver* __stdcall CreateInternalSenderReceiver();
