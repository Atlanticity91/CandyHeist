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
#include "HExtraction.generated.h"

class UDecalComponent;
class USphereComponent;

UCLASS( )
class CANDYHEIST_API AHExtraction : public AActor {

	GENERATED_BODY( )
	
protected:
	UPROPERTY( VisibleAnywhere )
	UDecalComponent* DecalComp;

	UPROPERTY( VisibleAnywhere )
	USphereComponent* TriggerComp;

public:	
	AHExtraction( );

protected:
	virtual void BeginPlay( ) override;

	UFUNCTION( )
	void HandleOverlap( UPrimitiveComponent* comp, AActor* other, UPrimitiveComponent* other_comp, int32 other_id, bool from_sweep, const FHitResult& sweep_result );

};
