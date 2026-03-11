#include "BaseInputConfig.h"

const UInputAction* UBaseInputConfig::FindNativeInputActionByTag(const FGameplayTag& InputTag) const
{
	for (const FBaseInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	return nullptr;
}
