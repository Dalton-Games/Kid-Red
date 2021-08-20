// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

static FString InputEventToString(const EInputEvent& InInputEvent)
{
	switch (InInputEvent)
	{
		case IE_Pressed:
		return "Pressed";
		
		case IE_Released:
		return "Released";
		
		case IE_Repeat:
		return "Repeat";
		
		case IE_DoubleClick:
		return "DoubleClick";
		
		case IE_Axis:
		return "Axis";
		
		case IE_MAX:
		return "MAX";
		
		default:
		return "None";
	}
}