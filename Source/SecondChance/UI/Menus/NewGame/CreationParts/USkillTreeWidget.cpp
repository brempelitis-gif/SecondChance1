#include "UI/Menus/NewGame/CreationParts//USkillTreeWidget.h"
#include "Kismet/GameplayStatics.h"


void USkillTreeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Šis izpildīsies gan Editorā, gan spēles sākumā
	if (BackBtn)
	{
		BackBtn->SetLabel(BackButtonText);
	}

	if (PlayBtn)
	{
		PlayBtn->SetLabel(PlayButtonText);
	}
}

void USkillTreeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackBtn)
	{
		BackBtn->OnClicked.AddDynamic(this, &USkillTreeWidget::HandleBackClicked);
	}

	if (PlayBtn)
	{
		PlayBtn->OnClicked.AddDynamic(this, &USkillTreeWidget::HandlePlayClicked);
	}
}

void USkillTreeWidget::HandleBackClicked()
{
	// Izsaucam delegātu, ko klausās UCharacterCreationMain
	OnBackStepRequested.Broadcast();
}

void USkillTreeWidget::HandlePlayClicked()
{
	// Šeit mēs ielādējam tavu pirmo spēles karti
	// Aizstāj "FirstLevelName" ar savu reālo kartes nosaukumu
	UGameplayStatics::OpenLevel(this, FName("FirstLevelName"));
}
