#include "UI/Base/UIBaseWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UUIBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetRenderOpacity(0.f);
	FadeIn();
}

void UUIBaseWidget::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimer);
	}

	Super::NativeDestruct();
}

void UUIBaseWidget::FadeIn()
{
	PlayFade(0.f, 1.f, false);
	OnWidgetShown();
}

void UUIBaseWidget::FadeOutAndRemove()
{
	PlayFade(1.f, 0.f, true);
	OnWidgetHidden();
}
void UUIBaseWidget::PlayFade(float From, float To, bool bRemoveAfter)
{
	SetRenderOpacity(From);

	if (!GetWorld())
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(FadeTimer);
	const float StepTime = 0.016f;
	const int32 Steps = FMath::Max(1, FadeDuration / StepTime);
	int32* CurrentStep = new int32(0);
	GetWorld()->GetTimerManager().SetTimer(
		FadeTimer,
		FTimerDelegate::CreateLambda(
			[this, From, To, Steps, bRemoveAfter, CurrentStep]()
			{
				float Alpha = FMath::Clamp(
					(float)(*CurrentStep) / (float)Steps,
					0.f,
					1.f
				);

				float Opacity = FMath::Lerp(From, To, Alpha);
				SetRenderOpacity(Opacity);

				(*CurrentStep)++;

				if (*CurrentStep > Steps)
				{
					GetWorld()->GetTimerManager().ClearTimer(FadeTimer);
					delete CurrentStep;

					SetRenderOpacity(To);

					if (bRemoveAfter)
					{
						RemoveFromParent();
					}
				}
			}
		),
		StepTime,
		true
	);
}
