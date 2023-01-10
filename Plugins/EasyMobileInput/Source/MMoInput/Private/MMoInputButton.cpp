// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#include "MMoInputButton.h"
#include "Runtime/Engine/Public/Slate/DeferredCleanupSlateBrush.h"
#include "SMMoInputButton.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

#define LOCTEXT_NAMESPACE "UMG"

UMMoInputButton::UMMoInputButton()
	: bWithJoystick(true)
	, bCircular(true)
	, ButtonSize(FVector2D(100.f, 100.f))
	, IconSize(FVector2D(100.f, 100.f))
	, RangeSize(FVector2D(200.f, 200.f))
	, TopSize(FVector2D(200.f, 200.f))
	, Center(FVector2D(0.5f, 0.5f)) {

}


TSharedRef<SWidget> UMMoInputButton::RebuildWidget() {

	// UMMoInputButton::RebuildWidget ==> SMMoInputButton::Construct()
	
	// Obtaining values from UMG and pass to Slate
	this->MyButton = SNew(SMMoInputButton)
					   .OnTouchDown(BIND_UOBJECT_DELEGATE(FOnTouchDown, HandleOnTouchDown))
					   .OnTouchUp(BIND_UOBJECT_DELEGATE(FOnTouchUp, HandleOnTouchUp))
					   .OnTouchMove(BIND_UOBJECT_DELEGATE(FOnTouchMove, HandleOnTouchMove));

	this->MyButton->SetOwner(this);
	return this->MyButton.ToSharedRef();
}


void UMMoInputButton::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);
	MyButton.Reset();
}

void UMMoInputButton::SynchronizeProperties() {
	// This is called after SMMoInputButton::Construct()

	Super::SynchronizeProperties();

	TAttribute<const FSlateBrush*> IconBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Icon, const FSlateBrush*, ConvertIconImage);
	TAttribute<const FSlateBrush*> DefaultBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Default, const FSlateBrush*, ConvertDefaultImage);
	TAttribute<const FSlateBrush*> PressedBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Pressed, const FSlateBrush*, ConvertPressedImage);
	TAttribute<const FSlateBrush*> RangeBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Range, const FSlateBrush*, ConvertRangeImage);
	TAttribute<const FSlateBrush*> TopBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Top, const FSlateBrush*, ConvertTopImage);

	this->MyButton->SetIcon(IconBinding);
	this->MyButton->SetDefault(DefaultBinding);
	this->MyButton->SetPressed(PressedBinding);
	this->MyButton->SetRange(RangeBinding);
	this->MyButton->SetTop(TopBinding);
}

const FSlateBrush* UMMoInputButton::ConvertIconImage(TAttribute<FSlateBrush> InImageAsset) const
{
	UMMoInputButton* MutableThis = const_cast<UMMoInputButton*>(this);
	MutableThis->Icon = InImageAsset.Get();

	return &Icon;
}
const FSlateBrush* UMMoInputButton::ConvertDefaultImage(TAttribute<FSlateBrush> InImageAsset) const
{
	UMMoInputButton* MutableThis = const_cast<UMMoInputButton*>(this);
	MutableThis->Default = InImageAsset.Get();

	return &Default;
}

const FSlateBrush* UMMoInputButton::ConvertPressedImage(TAttribute<FSlateBrush> InImageAsset) const
{
	UMMoInputButton* MutableThis = const_cast<UMMoInputButton*>(this);
	MutableThis->Pressed = InImageAsset.Get();

	return &Pressed;
}

const FSlateBrush* UMMoInputButton::ConvertRangeImage(TAttribute<FSlateBrush> InImageAsset) const
{
	UMMoInputButton* MutableThis = const_cast<UMMoInputButton*>(this);
	MutableThis->Range = InImageAsset.Get();

	return &Range;
}

const FSlateBrush* UMMoInputButton::ConvertTopImage(TAttribute<FSlateBrush> InImageAsset) const
{
	UMMoInputButton* MutableThis = const_cast<UMMoInputButton*>(this);
	MutableThis->Top = InImageAsset.Get();

	return &Top;
}


#if WITH_EDITOR
const FText UMMoInputButton::GetPaletteCategory() {
	return LOCTEXT("MMo Input", "MMo Input");
}
#endif


void UMMoInputButton::HandleOnTouchDown(FVector2D Location) {
	this->OnTouchDown.Broadcast(Location);
}

void UMMoInputButton::HandleOnTouchUp(FVector2D Location) {
	this->OnTouchUp.Broadcast(Location);
}

void UMMoInputButton::HandleOnTouchMove(FVector2D Location) {
	this->OnTouchMove.Broadcast(Location);
}


UMaterialInstanceDynamic* UMMoInputButton::GetDynamicMaterialOfIcon()
{
	UMaterialInterface* Material = nullptr;

	UObject* Resource = Icon.GetResourceObject();
	Material = Cast<UMaterialInterface>(Resource);

	if (Material)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Icon.SetResourceObject(DynamicMaterial);

			if (MyButton.IsValid())
			{
				MyButton->SetIcon(&Icon);
			}
		}

		return DynamicMaterial;
	}

	//TODO UMG can we do something for textures?  General purpose dynamic material for them?
	return nullptr;
}


UMaterialInstanceDynamic* UMMoInputButton::GetDynamicMaterialOfDefault()
{
	UMaterialInterface* Material = nullptr;

	UObject* Resource = Default.GetResourceObject();
	Material = Cast<UMaterialInterface>(Resource);

	if (Material)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Default.SetResourceObject(DynamicMaterial);

			if (MyButton.IsValid())
			{
				MyButton->SetDefault(&Default);
			}
		}

		return DynamicMaterial;
	}

	//TODO UMG can we do something for textures?  General purpose dynamic material for them?
	return nullptr;
}

UMaterialInstanceDynamic* UMMoInputButton::GetDynamicMaterialOfPressed()
{
	UMaterialInterface* Material = nullptr;

	UObject* Resource = Pressed.GetResourceObject();
	Material = Cast<UMaterialInterface>(Resource);

	if (Material)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Pressed.SetResourceObject(DynamicMaterial);

			if (MyButton.IsValid())
			{
				MyButton->SetPressed(&Pressed);
			}
		}

		return DynamicMaterial;
	}

	//TODO UMG can we do something for textures?  General purpose dynamic material for them?
	return nullptr;
}


#undef LOCTEXT_NAMESPACE