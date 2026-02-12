#include "UCharacterSkillTreeWidget.h"
#include "Kismet/GameplayStatics.h"


void UCharacterSkillTreeWidget::NativePreConstruct()
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

void UCharacterSkillTreeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (BackBtn)
	{
		BackBtn->OnClicked.AddDynamic(this, &UCharacterSkillTreeWidget::HandleBackClicked);
	}

	if (PlayBtn)
	{
		PlayBtn->OnClicked.AddDynamic(this, &UCharacterSkillTreeWidget::HandlePlayClicked);
	}
}

void UCharacterSkillTreeWidget::HandleBackClicked()
{
	// Izsaucam delegātu, ko klausās UCharacterCreationMain
	OnBackStepRequested.Broadcast();
}

void UCharacterSkillTreeWidget::HandlePlayClicked()
{
	// Šeit mēs ielādējam tavu pirmo spēles karti
	// Aizstāj "FirstLevelName" ar savu reālo kartes nosaukumu
	UGameplayStatics::OpenLevel(this, FName("FirstLevelName"));
}
