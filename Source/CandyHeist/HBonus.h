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
#include "HObject.h"
#include "HBonus.generated.h"

class AHCharacter;
class USoundBase;
class UTexture2D;
class UMaterialInterface;

UCLASS( )
class CANDYHEIST_API AHBonus : public AHObject {

	GENERATED_BODY( )

protected:
	UPROPERTY( EditDefaultsOnly, Category = "HFramework|FX" )
	USoundBase* StartSound;

	UPROPERTY( EditDefaultsOnly, Category = "HFramework|FX" )
	USoundBase* StopSound;

	UPROPERTY( EditDefaultsOnly, Category = "HFramework|FX" )
	UTexture2D* Icon;

	UPROPERTY( EditDefaultsOnly, Category = "HFramework|Gameplay" )
	float Duration = 5.f;

	UPROPERTY( EditDefaultsOnly, Category = "HFramework|Gameplay" )
	UMaterialInterface* PostProcess;

public:
	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Bonus" )
	void OnBonusStart( );

	UFUNCTION( BlueprintImplementableEvent, Category = "HFramework|Bonus" )
	void OnBonusIsApplyTo( AHCharacter* player );

	UFUNCTION( BlueprintImplementableEvent, Category = "HFramework|Bonus" )
	void OnBonusIsRemoveOf( AHCharacter* player );

	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Bonus" )
	void OnBonusStop( );

protected:
	virtual void OnBonusStart_Implementation( );

	virtual void OnBonusStop_Implementation( );

public:
	UFUNCTION( BlueprintCallable, Category = "HFramework|Bonus" )
	USoundBase* GetStartSound( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Bonus" )
	USoundBase* GetStopSound( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Bonus" )
	UTexture2D* GetIcon( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Bonus" )
	float GetDuration( ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|Bonus" )
	UMaterialInterface* GetPostProcess( ) const;

};
