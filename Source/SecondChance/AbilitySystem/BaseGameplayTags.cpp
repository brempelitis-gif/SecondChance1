#include "BaseGameplayTags.h"

namespace BaseGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LookMouse, "InputTag.LookMouse", "Look Mouse Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Run, "InputTag.Run", "Run Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump Input");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned,
		"InitState.Spawned",
		"1: The actor has been spawned but not yet initialized. This is the default state for all actors when they are created.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable,
		"InitState.DataAvailable",
		"2: The actor has had its data loaded and is ready for initialization. This state indicates that the necessary data for the actor is now available, but the actor has not yet been fully initialized.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized,
		"InitState.DataInitialized",
		"3: The actor has completed its data initialization. This state indicates that the actor has successfully initialized its data and is now ready for gameplay. The actor can now perform any necessary setup based on the initialized data.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady,
		"InitState.GameplayReady",
		"4: The actor is fully initialized and ready for gameplay. This state indicates that the actor has completed all necessary initialization steps and is now ready to participate in the game. The actor can now interact with other actors, respond to player input, and perform any necessary gameplay logic.");
		
}
