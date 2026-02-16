#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UBaseLabeledWidget.generated.h"

UCLASS(Abstract) // Abstract nozīmē, ka mēs neizveidosim šo widgetu pašu par sevi
class SECONDCHANCE_API UBaseLabeledWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Kopīgs Label uzstādīšanas rīks
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetLabel(FText InText);

protected:
	// BindWidget nozīmē, ka visos bērnu Blueprintos ir jābūt TextBlock ar šādu nosaukumu
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LabelText;

	// Kopīgs mainīgais, ko redzēsi Editorā katram widgetam
	UPROPERTY(EditAnywhere, Category = "UI")
	FText LabelDefaultValue;

	// Kopīga NativePreConstruct loģika
	virtual void NativePreConstruct() override;
};
