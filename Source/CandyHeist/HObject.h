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
#include "HObject.generated.h"

class AHCharacter;
class AHPickup;
class UStaticMesh;

UCLASS( )
class CANDYHEIST_API AHObject : public AActor {

	GENERATED_BODY( )

protected:
	UPROPERTY( EditAnywhere, Category = "HFramework|FX" )
	UStaticMesh* Mesh;

	UPROPERTY( EditAnywhere, Category = "HFramework|Gameplay" )
	TSubclassOf<AHPickup> Pickup;

public:
	AHObject( );

	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Object" )
	void OnPickObject( AHCharacter* player );

	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Object" )
	void OnDropObject( AHCharacter* player );

protected:
	virtual void OnPickObject_Implementation( AHCharacter* player );

	virtual void OnDropObject_Implementation( AHCharacter* player );

public:
	UFUNCTION( BlueprintCallable, Category = "HFramework|Object" )
	UStaticMesh* GetMesh( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Object" )
	TSubclassOf<AHPickup> GetPickup( ) const;

};
