// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "UMGEditorPrivatePCH.h"

#include "SUMGEditorTreeItem.h"
#include "UMGEditor.h"
#include "UMGEditorViewportClient.h"
#include "UMGEditorActions.h"
#include "WidgetTemplateDragDropOp.h"

#include "PreviewScene.h"
#include "SceneViewport.h"

#include "BlueprintEditor.h"
#include "SKismetInspector.h"
#include "BlueprintEditorUtils.h"

void SUMGEditorTreeItem::Construct(const FArguments& InArgs, TSharedPtr<FBlueprintEditor> InBlueprintEditor, USlateWrapperComponent* InItem)
{
	BlueprintEditor = InBlueprintEditor;
	Item = InItem;

	ChildSlot
	[
		SNew(STextBlock)
		.Text(this, &SUMGEditorTreeItem::GetItemText)
	];
}

FText SUMGEditorTreeItem::GetItemText() const
{
	return FText::FromString(Item->GetName());
}

void SUMGEditorTreeItem::OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	//TSharedPtr<FDragDropOperation> Operation = DragDropEvent.GetOperation();
	//if ( !Operation.IsValid() )
	//{
	//	return;
	//}

	//if ( Operation->IsOfType<FWidgetTemplateDragDropOp>() )
	//{
	//	TSharedPtr<FWidgetTemplateDragDropOp> DragDropOp = StaticCastSharedPtr<FWidgetTemplateDragDropOp>(Operation);
	//	bDraggedOver = true;
	//}
}

void SUMGEditorTreeItem::OnDragLeave(const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FWidgetTemplateDragDropOp> DragDropOp = DragDropEvent.GetOperationAs<FWidgetTemplateDragDropOp>();
	if ( DragDropOp.IsValid() )
	{
		DragDropOp->ResetToDefaultToolTip();
	}
}

FReply SUMGEditorTreeItem::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FWidgetTemplateDragDropOp> DragDropOp = DragDropEvent.GetOperationAs<FWidgetTemplateDragDropOp>();
	if ( DragDropOp.IsValid() )
	{
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SUMGEditorTreeItem::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FWidgetTemplateDragDropOp> DragDropOp = DragDropEvent.GetOperationAs<FWidgetTemplateDragDropOp>();
	if ( DragDropOp.IsValid() )
	{
		return FReply::Handled();
	}

	return FReply::Unhandled();
}
