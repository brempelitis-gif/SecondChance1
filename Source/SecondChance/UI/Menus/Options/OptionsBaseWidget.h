
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Enums/ESettingsCategory.h"
#include "Core/Subsystems/AudioManagerSubsystem.h" // Iekļaujam, lai zinātu Enum un Struct
#include "OptionsBaseWidget.generated.h"

// Forward Declarations (lai paātrinātu kompilāciju)
class UUIManagerSubsystem;
class UAudioManagerSubsystem;
class UMenuButtonWidget;
class UWidgetSwitcher;

UCLASS()
class SECONDCHANCE_API UOptionsBaseWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // --- Lifecycle ---
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

    // --- Audio Control (Called from UI Sliders) ---
    UFUNCTION(BlueprintCallable, Category = "Options | Audio")
    void SetAudioOption(EAudioOption Option, float Value);

    // --- Graphics Control (Called from UI Dropdowns) ---
    UFUNCTION(BlueprintCallable, Category = "Options | Graphics")
    void ApplyGraphicsResolution(FIntPoint Resolution);

    // --- Main Button Handlers ---
    UFUNCTION(BlueprintCallable, Category = "Options | Actions")
    void HandleApplyClicked();

    UFUNCTION(BlueprintCallable, Category = "Options | Actions")
    void HandleCancelClicked();

    UFUNCTION(BlueprintCallable, Category = "Options | Actions")
    void HandleBackClicked();

	// Pievieno šo rindiņu pie publiskajām funkcijām
	UFUNCTION(BlueprintPure, Category = "Options | State")
	bool IsCategoryPending(ESettingsCategory Category) const;

	// Tev obligāti vajag šo rindiņu, citādi bērni to neatradīs!
	UPROPERTY(BlueprintAssignable, Category = "Options | Events")
	FOnSettingsChanged OnSettingsChanged;

protected:
    // --- Events for Blueprint ---
    
    // Šo implementē Blueprintā: Saņem datus un uzstāda slaideru vērtības
    // (Izsauc pie Construct un pie Cancel)
    UFUNCTION(BlueprintImplementableEvent, Category = "Options | UI Updates")
    void UpdateAudioSliders(const FAudioSettingsData& AudioData);

    // --- Subsystem Cache (Optimization) ---
    UPROPERTY()
    UUIManagerSubsystem* UIManager;

    UPROPERTY()
    UAudioManagerSubsystem* AudioManager;

    // --- Internal State ---
    UPROPERTY(BlueprintReadOnly, Category = "Options | State")
    TSet<ESettingsCategory> PendingCategories;

    // --- Helper Functions ---
    void MarkCategoryPending(ESettingsCategory Category);
    void ClearCategoryPending(ESettingsCategory Category);
    void UpdateActionButtonsVisibility(); // Rāda/Slēpj Apply pogu

    // --- Internal Apply/Cancel Logic ---
    void ApplyAudioChanges();
    void CancelAudioChanges();

    void ApplyGraphicsChanges();
    void CancelGraphicsChanges(); // Reverts resolution/window mode
    
    // Callbacks for Confirmation Popup
    UFUNCTION()
    void OnGraphicsConfirmed();
    
    UFUNCTION()
    void OnGraphicsReverted();

    // --- UI Elements (Optional, if you bind in C++) ---
    UPROPERTY(meta = (BindWidgetOptional))
    UMenuButtonWidget* ApplyButton;

    UPROPERTY(meta = (BindWidgetOptional))
    UMenuButtonWidget* CancelButton;
};
