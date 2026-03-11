#pragma once

#include "NativeGameplayTags.h"

namespace BaseGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LookMouse);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Run);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
	
}
