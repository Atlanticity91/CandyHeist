/**
 *
 * MIT License
 *
 * Copyright (c) 2020 Alves Quentin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HCharacter.generated.h"

class AHObject;
class UCameraComponent;
class UHInventory;
class UInputComponent;
class USpringArmComponent;
class UStaticMeshComponent;

UCLASS( )
class CANDYHEIST_API AHCharacter : public ACharacter {

	GENERATED_BODY( )

protected:
	UPROPERTY( VisibleAnywhere )
	USpringArmComponent* SpringArm;

	UPROPERTY( VisibleAnywhere )
	UCameraComponent* Camera;

	UPROPERTY( EditAnywhere )
	UHInventory* Inventory;

	UPROPERTY( VisibleAnywhere )
	UStaticMeshComponent* Decal;

	UPROPERTY( VisibleAnywhere )
	UStaticMeshComponent* ItemHolder;

	UPROPERTY( EditAnywhere, Category = "HFramework|FX" )
	FName ItemSocket = "Hand";

	UPROPERTY( EditAnywhere, Category = "HFramework|FX" )
	FName PlayerColorLabel = "Color";

	UPROPERTY( EditAnywhere, Category = "HFramework|Gameplay" )
	float RotationSpeed = 10.f;

	UPROPERTY( ReplicatedUsing=OnRep_Item, BlueprintReadOnly, Category = "HFramework|Gameplay" )
	AHObject* Item;

	UPROPERTY( ReplicatedUsing=OnRep_Direction, BlueprintReadOnly, Category = "HFramework|Gameplay" )
	FVector Direction;

	UPROPERTY( ReplicatedUsing=OnRep_Inventory, BlueprintReadOnly, Category = "HFramework|Gameplay" )
	bool IsInventoryVisible;

	UPROPERTY( ReplicatedUsing=OnRep_Dash, BlueprintReadOnly, Category = "HFramework|Gameplay" )
	bool IsDashActivate;

public:
	AHCharacter( );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	void SetColor( FColor value );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	void SetItem( int slot_id );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	void EnableDash( );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	void DisableDash( );
		
	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	void ToggleDash( );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "HFramework|Player" )
	void DropItem( );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "HFramework|Player" )
	void UseItem( );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "HFramework|Player" )
	void UseInventory( );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "HFramework|Player" )
	void UseDash( );

	virtual void Tick( float delta_time ) override;

	virtual void SetupPlayerInputComponent( UInputComponent* input_comp ) override;

protected:
	virtual void BeginPlay( ) override;

	void MoveForward( float value );

	void MoveRight( float value );

	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Player" )
	void OnOpenInventory( );

	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Player" )
	void OnCloseInventory( );

	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Player" )
	void OnCharacterDash( );

protected:
	UFUNCTION( )
	void OnRep_Item( AHObject* object );

	UFUNCTION( )
	void OnRep_Direction( FVector target );

	UFUNCTION( )
	void OnRep_Inventory( bool state );

	UFUNCTION( )
	void OnRep_Dash( bool state );

protected:
	virtual void DropItem_Implementation( );

	virtual void UseItem_Implementation( );

	virtual void UseInventory_Implementation( );

	virtual void UseDash_Implementation( );

	virtual void OnOpenInventory_Implementation( );

	virtual void OnCloseInventory_Implementation( );

	virtual void OnCharacterDash_Implementation( );

public:
	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	FVector GetDirection( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	UHInventory* GetInventory( ) const;
	
	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	bool GetIsHoldingItem( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	AHObject* GetItem( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	bool GetIsInventoryVisible( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Player" )
	bool GetIsDashActivate( ) const;

};
