#include "UI/Menus/LoadGame/SaveSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"

void USaveSlotWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    BindButtons();
}
void USaveSlotWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (SelectBtn) SelectBtn->SetLabel(SelectBtnLabel);
}
void USaveSlotWidget::BindButtons()
{
    if (SelectBtn) SelectBtn->OnClicked.AddDynamic(this, &USaveSlotWidget::HandleBtnClicked);
}

void USaveSlotWidget::SetupSlot(const FSaveMetadata& Metadata)
{
    CachedSlotName = Metadata.SlotName;

    if (PlayerNameText) PlayerNameText->SetText(FText::FromString(Metadata.PlayerName));
    if (SaveDateText) SaveDateText->SetText(FText::FromString(Metadata.SaveDate.ToString(TEXT("%d.%m.%Y %H:%M"))));

    // Atrodam bildes ceļu: Saved/Screenshots/WindowsEditor/SlotName_Portrait.png
    FString ScreenshotPath = FPaths::ProjectSavedDir() + "Screenshots/WindowsEditor/" + Metadata.SlotName + "_Portrait.png";
    
    UTexture2D* LoadedPortrait = LoadTextureFromFile(ScreenshotPath);
    if (LoadedPortrait && PortraitImage)
    {
        PortraitImage->SetBrushFromTexture(LoadedPortrait);
    }
}

void USaveSlotWidget::HandleBtnClicked()
{
    OnSlotSelected.Broadcast(CachedSlotName);
}

UTexture2D* USaveSlotWidget::LoadTextureFromFile(const FString& FullPath)
{
    if (!FPaths::FileExists(FullPath)) return nullptr;

    TArray<uint8> RawFileData;
    if (!FFileHelper::LoadFileToArray(RawFileData, *FullPath)) return nullptr;

    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
    {
        TArray<uint8> UncompressedBGRA;
        if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
        {
            UTexture2D* LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
            if (!LoadedT2D) return nullptr;

            void* TextureData = LoadedT2D->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
            FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
            LoadedT2D->GetPlatformData()->Mips[0].BulkData.Unlock();

            LoadedT2D->UpdateResource();
            return LoadedT2D;
        }
    }
    return nullptr;
}
