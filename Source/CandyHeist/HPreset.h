// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HPreset.generated.h"

UCLASS( )
class CANDYHEIST_API AHPreset : public AActor {

	GENERATED_BODY( )
	
public:	
	AHPreset( );

protected:
	virtual void BeginPlay( ) override;

};
