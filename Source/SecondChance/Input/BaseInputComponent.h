#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "BaseInputConfig.h"
#include "BaseInputComponent.generated.h"

UCLASS()
class SECONDCHANCE_API UBaseInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserClass, typename FuncType>
	void BindNativeAction(
		const UBaseInputConfig* InputConfig,
		const FGameplayTag& InputTag,
		ETriggerEvent TriggerEvent,
		UserClass* Object,
		FuncType Func
	);
	template<class UserClass, typename PressFuncType, typename ReleaseFuncType>
	void BindAbilityActions(
		const UBaseInputConfig* InputConfig,
		UserClass* Object,
		PressFuncType PressedFunc,
		ReleaseFuncType ReleasedFunc
	);
};

template<class UserClass, typename FuncType>
void UBaseInputComponent::BindNativeAction(
	const UBaseInputConfig* InputConfig,
	const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent,
	UserClass* Object,
	FuncType Func
)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionByTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressFuncType, typename ReleaseFuncType>
void UBaseInputComponent::BindAbilityActions(
	const UBaseInputConfig* InputConfig,
	UserClass* Object,
	PressFuncType PressedFunc,
	ReleaseFuncType ReleasedFunc
)
{
	check(InputConfig);
	for (const FBaseInputAction& Action:InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc) BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag);
			if (ReleasedFunc) BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
		}
	}
}
		