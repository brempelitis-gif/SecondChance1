#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMenuEditableTextWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuTextChanged, const FText&, Text);

UCLASS()
class SECONDCHANCE_API UMenuEditableTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetLabel(FText InText);
	void SetText(FText InText);
	FText GetText() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMenuTextChanged OnTextChanged;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LabelText;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditableInput;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FText LabelValue = FText::FromString("Label");

	UFUNCTION()
	void HandleOnTextChanged(const FText& Text);
};