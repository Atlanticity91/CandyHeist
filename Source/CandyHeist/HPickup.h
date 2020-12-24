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
#include "GameFramework/Actor.h"
#include "HPickup.generated.h"

class AHObject;
class UBoxComponent;
class USoundBase;
class UStaticMesh;
class UStaticMeshComponent;
class UMaterialInterface;

UCLASS( )
class CANDYHEIST_API AHPickup : public AActor {

	GENERATED_BODY( )

protected:
	UPROPERTY( EditAnywhere )
	UStaticMeshComponent* MeshComp;

	UPROPERTY( EditAnywhere )
	UStaticMeshComponent* DecalComp;

	UPROPERTY( EditAnywhere )
	UBoxComponent* TriggerComp;

	UPROPERTY( EditAnywhere, Category = "HFramework|FX" )
	UMaterialInterface* Decal;

	UPROPERTY( EditAnywhere, Category = "HFramework|FX" )
	UStaticMesh* DecalMesh;

	UPROPERTY( EditAnywhere, Category = "HFramework|FX" )
	FColor DecalColor;

	UPROPERTY( EditAnywhere, Category = "HFramework|FX" )
	UStaticMesh* Mesh;

	UPROPERTY( EditAnywhere, Category = "HFramework|Gameplay" )
	TSubclassOf<AHObject> Object;

private:
	bool IsActive;
	AHObject* Item;

public:
	AHPickup( );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Pickup" )
	void SetItem( AHObject* item );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Pickup" )
	void Enable( );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Pickup" )
	void Disable( );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Pickup" )
	void Toggle( );

	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Pickup" )
	void OnEnable( );

	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Pickup" )
	void OnDisable( );

protected:
	virtual void BeginPlay( ) override;

	UFUNCTION( )
	void HandleOverlap( UPrimitiveComponent* comp, AActor* other, UPrimitiveComponent* other_comp, int32 other_id, bool from_sweep, const FHitResult& sweep_result );

	virtual void OnEnable_Implementation( );

	virtual void OnDisable_Implementation( );

public:
	UFUNCTION( BlueprintCallable, Category = "HFramework|Pickup" )
	bool GetIsActive( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Pickup" )
	AHObject* GetItem( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Pickup" )
	TSubclassOf<AHObject> GetType( ) const;

};
