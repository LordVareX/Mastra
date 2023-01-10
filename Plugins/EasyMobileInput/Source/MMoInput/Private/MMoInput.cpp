// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#include "MMoInput.h"

#define LOCTEXT_NAMESPACE "FMMoInputModule"

void FMMoInputModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FMMoInputModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMMoInputModule, MMoInput)