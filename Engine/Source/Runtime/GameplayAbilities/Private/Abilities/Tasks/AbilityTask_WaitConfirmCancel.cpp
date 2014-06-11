
#include "AbilitySystemPrivatePCH.h"
#include "Abilities/Tasks/AbilityTask_WaitConfirmCancel.h"

#include "AbilitySystemComponent.h"

#include "Abilities/GameplayAbility.h"
#include "Abilities/GameplayAbility_Instanced.h"

UAbilityTask_WaitConfirmCancel::UAbilityTask_WaitConfirmCancel(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	ASC = NULL;
}

void UAbilityTask_WaitConfirmCancel::OnConfirmCallback()
{
	OnConfirm.Broadcast(true);
	ASC->ConsumeAbilityConfirm();
}

void UAbilityTask_WaitConfirmCancel::OnCancelCallback()
{
	OnCancel.Broadcast(false);
	ASC->ConsumeAbilityConfirm();
}

UAbilityTask_WaitConfirmCancel* UAbilityTask_WaitConfirmCancel::WaitConfirmCancel(UObject* WorldContextObject)
{
	check(WorldContextObject);
	UGameplayAbility_Instanced* ThisAbility = CastChecked<UGameplayAbility_Instanced>(WorldContextObject);
	if (ThisAbility)
	{
		UAbilityTask_WaitConfirmCancel * MyObj = NULL;
		MyObj = NewObject<UAbilityTask_WaitConfirmCancel>();
		MyObj->Ability = ThisAbility;

		return MyObj;
	}
	return NULL;
}

void UAbilityTask_WaitConfirmCancel::Activate()
{
	if (Ability.IsValid())
	{
		const FGameplayAbilityActorInfo* Info = Ability.Get()->GetCurrentActorInfo();

		ASC = Info->AbilitySystemComponent.Get();

		if (!Info->IsLocallyControlled())
		{
			if (ASC->ReplicatedConfirmAbility)
			{
				// It could have already been confirmed (we may have been behind a client predicting this ability, so it could have got here
				// before we actually executed this task)
				OnConfirmCallback();
				ASC->ConsumeAbilityConfirm();
				return;
			}
		}
		
		// We have to wait for the callback from the AbilitySystemComponent. Which may be instigated locally or through network replication
		ASC->ConfirmCallbacks.AddDynamic(this, &UAbilityTask_WaitConfirmCancel::OnConfirmCallback);
		ASC->CancelCallbacks.AddDynamic(this, &UAbilityTask_WaitConfirmCancel::OnCancelCallback);
	}
}