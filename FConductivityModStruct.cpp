// Fill out your copyright notice in the Description page of Project Settings.


#include "LongLostLightning/Public/FConductivityModStruct.h"

FConductivityModStruct::FConductivityModStruct(): Modifier(EModifier::Fire) {
}

FConductivityModStruct::FConductivityModStruct(EModifier InputModifier)  {
	Modifier = InputModifier;
}
