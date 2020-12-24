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
#include "Components/SceneComponent.h"
#include "HInventory.generated.h"

#define HINV_SIZE 4 

class AHObject;
class UStaticMeshComponent;

UCLASS( ClassGroup = ( "HFramework" ), meta = ( BlueprintSpawnableComponent ) )
class CANDYHEIST_API UHInventory : public USceneComponent {

	GENERATED_BODY( )

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HFramework|Inventory" )
	TArray<FVector> SlotsPositions;

protected:
	UStaticMeshComponent* Slots[ HINV_SIZE ];
	AHObject* Content[ HINV_SIZE ];

public:
	UHInventory( );

	void Prepare( );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	int Append( AHObject* object );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	AHObject* Swap( AHObject* object, int slot_id );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	AHObject* RetriveFirst( );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	AHObject* Retrive( int slot_id );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	void Show( );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	void Hide( );

private:
	void SetSlot( int slot_id, AHObject* object );

public:
	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	int GetLength( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	int GetSize( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	bool GetIsFull( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	bool Contain( TSubclassOf<AHObject> type, int& count );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Inventory" )
	bool Next( TSubclassOf<AHObject> type, int previous, int& slot_id );

public:
	template< typename Type = AHObject >
	bool Contain( int& count ) {
		return this->Contain( AHObject::StaticClass( ), count );
	}

	template< typename Type = AHObject >
	bool Next( int previous, int& slot_id ) {
		return this->Next( AHObject::StaticClass( ), previous, slot_id );
	}

};
