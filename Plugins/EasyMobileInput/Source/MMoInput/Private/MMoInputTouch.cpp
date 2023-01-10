// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#include "MMoInputTouch.h"
#include "SMMoInputTouch.h"
#include "Events.h"
#include "Runtime/Engine/Public/Slate/DeferredCleanupSlateBrush.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

#define LOCTEXT_NAMESPACE "UMG"



UMMoInputTouch::UMMoInputTouch()
	: bCircular(false)
	, BackGround(nullptr)
	, ActiveColor(FLinearColor::White)
	, DeActiveColor(FLinearColor::White)
	, Center(FVector2D(0.5f, 0.5f))
	//, VisualCenter (FVector2D::ZeroVector)
	//, CorrectedCenter (FVector2D::ZeroVector)
	, MyTouch(nullptr) {
}


TSharedRef<SWidget> UMMoInputTouch::RebuildWidget() {
	this->Image1.Reset();

	if (BackGround != nullptr) {
		this->Image1 = FDeferredCleanupSlateBrush::CreateBrush(this->BackGround);
	}

	this->MyTouch = SNew(SMMoInputTouch)
					.OnTouchDown(BIND_UOBJECT_DELEGATE(FOnTouchDown, HandleOnTouchDown))
					.OnTouchUp(BIND_UOBJECT_DELEGATE(FOnTouchUp, HandleOnTouchUp))
					.OnTouchMove(BIND_UOBJECT_DELEGATE(FOnTouchMove, HandleOnTouchMove));

	this->MyTouch->SetOwner(this);

	return this->MyTouch.ToSharedRef();
}


void UMMoInputTouch::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);

	this->MyTouch.Reset();
}


void UMMoInputTouch::HandleOnTouchDown(FVector2D Location) {
	this->OnTouchDown.Broadcast(Location);
}

void UMMoInputTouch::HandleOnTouchUp(FVector2D Location) {
	this->OnTouchUp.Broadcast(Location);
}

void UMMoInputTouch::HandleOnTouchMove(FVector2D Location) {
	this->OnTouchMove.Broadcast(Location);
}


#if WITH_EDITOR
const FText UMMoInputTouch::GetPaletteCategory() {
	return LOCTEXT("MMo Input", "MMo Input");
}
#endif

#undef LOCTEXT_NAMESPACE