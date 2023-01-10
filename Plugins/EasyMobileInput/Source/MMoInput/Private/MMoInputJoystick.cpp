// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#include "MMoInputJoystick.h"
#include "SMMoInputJoystick.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelSlot.h"
#include "Engine/Canvas.h"

#define LOCTEXT_NAMESPACE "UMG"

UMMoInputJoystick::UMMoInputJoystick()
	: RangeSize(FVector2D(200.f, 200.f))
	, TopSize(FVector2D(200.f, 200.f))
	, bNeedFadeOut(false)
	, FadeOutDelay(1.f)
	, FadeOutSpeed(4.f)
	, FadeOutColor(FLinearColor(1.f,1.f,1.f,0.3f))
	, bNeedRepositioning(false)
	, bRepositioningWithinCanvasPanelSlot(false)
	, bNeedResetPosition(false)
	, ResetPositionDelay(1.f) {
}

TSharedRef<SWidget> UMMoInputJoystick::RebuildWidget() {
	// call the SMMoInputJoystick::Construct first
	
	FVector2D RepositioningSize = FVector2D::ZeroVector;
#if (ENGINE_MAJOR_VERSION == 4)
		RepositioningSize = this->GetRepositioningSize();
#endif
	
	this->MyJoystick = SNew(SMMoInputJoystick)
						.bNeedFadeOut(bNeedFadeOut)
						.FadeOutColor(FadeOutColor)
						.bNeedRepositioning(bNeedRepositioning)
						.bNeedReset(bNeedResetPosition)
						.ResetDelay(ResetPositionDelay)
						.RepositioningSize(RepositioningSize)
						.bRepositioningWithinSlot(bRepositioningWithinCanvasPanelSlot);

	this->MyJoystick->SetOwner(this);
	
	return this->MyJoystick.ToSharedRef();
}

#if WITH_EDITOR
const FText UMMoInputJoystick::GetPaletteCategory() {
	return LOCTEXT("MMo Input", "MMo Input");
}
#endif


void UMMoInputJoystick::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);
	this->MyJoystick.Reset();
}

void UMMoInputJoystick::SynchronizeProperties() {
	Super::SynchronizeProperties();


	TAttribute<const FSlateBrush*> RangeBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Range, const FSlateBrush*, ConvertRangeImage);
	TAttribute<const FSlateBrush*> TopBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Top, const FSlateBrush*, ConvertTopImage);


	this->MyJoystick->SetRange(RangeBinding);
	this->MyJoystick->SetTop(TopBinding);
}

const FSlateBrush* UMMoInputJoystick::ConvertRangeImage(TAttribute<FSlateBrush> InImageAsset) const {
	UMMoInputJoystick* MutableThis = const_cast<UMMoInputJoystick*>(this);
	MutableThis->Range = InImageAsset.Get();

	return &Range;
}

const FSlateBrush* UMMoInputJoystick::ConvertTopImage(TAttribute<FSlateBrush> InImageAsset) const {
	UMMoInputJoystick* MutableThis = const_cast<UMMoInputJoystick*>(this);
	MutableThis->Top = InImageAsset.Get();

	return &Top;
}

UMaterialInstanceDynamic* UMMoInputJoystick::GetDynamicMaterialOfRange() {
	UMaterialInterface* Material = nullptr;

	UObject* Resource = Range.GetResourceObject();
	Material = Cast<UMaterialInterface>(Resource);

	if (Material)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Range.SetResourceObject(DynamicMaterial);

			if (MyJoystick.IsValid())
			{
				MyJoystick->SetRange(&Range);
			}
		}

		return DynamicMaterial;
	}

	//TODO UMG can we do something for textures?  General purpose dynamic material for them?
	return nullptr;
}

UMaterialInstanceDynamic* UMMoInputJoystick::GetDynamicMaterialOfTop() {
	UMaterialInterface* Material = nullptr;

	UObject* Resource = Top.GetResourceObject();
	Material = Cast<UMaterialInterface>(Resource);

	if (Material)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Top.SetResourceObject(DynamicMaterial);

			if (MyJoystick.IsValid())
			{
				MyJoystick->SetTop(&Top);
			}
		}

		return DynamicMaterial;
	}

	//TODO UMG can we do something for textures?  General purpose dynamic material for them?
	return nullptr;
}

FVector2D UMMoInputJoystick::GetRepositioningSize() const {
	const UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot);
	FVector2D RepositioningSize;
	
	if (CanvasSlot != nullptr ) {
		RepositioningSize = CanvasSlot->GetSize();
	}
	return RepositioningSize;
}


#undef LOCTEXT_NAMESPACE