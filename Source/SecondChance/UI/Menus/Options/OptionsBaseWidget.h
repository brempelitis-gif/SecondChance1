#pragma once

#include "CoreMinimal.h"
#include "Core/Enums/ESettingsCategory.h"
#include "UI/Base/UIBaseWidget.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "OptionsBaseWidget.generated.h"

UCLASS(Abstract)
class SECONDCHANCE_API UOptionsBaseWidget : public UUIBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:

	// =========================
	// UI Manager
	// =========================
	UPROPERTY()
	UUIManagerSubsystem* UIManager = nullptr;

	// =========================
	// Category
	// =========================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Options")
	ESettingsCategory Category = ESettingsCategory::None;

	// =========================
	// Buttons (common)
	// =========================
	UPROPERTY(meta = (BindWidgetOptional))
	class UMenuButtonWidget* ApplyButton;

	UPROPERTY(meta = (BindWidgetOptional))
	class UMenuButtonWidget* CancelButton;

	// =========================
	// Button handlers
	// =========================
	UFUNCTION()
	void HandleApplyClicked();

	UFUNCTION()
	void HandleCancelClicked();

	// =========================
	// UI Refresh
	// =========================
	UFUNCTION()
	void OnSettingsChanged(ESettingsCategory ChangedCategory);

	virtual void RefreshUI();
};
