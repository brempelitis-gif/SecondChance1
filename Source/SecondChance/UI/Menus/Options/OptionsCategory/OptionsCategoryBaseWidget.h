#pragma once

#include "CoreMinimal.h"
#include "UI/Base/UIBaseWidget.h"
#include "Core/Enums/ESettingsCategory.h"
#include "OptionsCategoryBaseWidget.generated.h"

class UUIManagerSubsystem;
class UOptionsBaseWidget; // Forward declaration uz jauno bāzi

UCLASS(Abstract)
class SECONDCHANCE_API UOptionsCategoryBaseWidget : public UUIBaseWidget
{
	GENERATED_BODY()

public:
	/** Kategorija (Audio, Graphics, etc.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Options")
	ESettingsCategory Category = ESettingsCategory::None;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	/** Reakcija uz pending izmaiņām */
	UFUNCTION()
	virtual void HandleSettingsChanged(ESettingsCategory ChangedCategory);

	/** Vai šī kategorija šobrīd ir pending */
	UFUNCTION(BlueprintPure, Category="Options")
	bool IsPending() const;

	/** Kešota norāde uz UIManager */
	UPROPERTY()
	UUIManagerSubsystem* UIManager = nullptr;
};
