#include "EventInitiator.h"


// Add default functionality here for any IEventInitiator functions that are not pure virtual.

// void IEventInitiator::OnInteract() {
// }


void IEventInitiator::ChanceModifier(int PercentChance, TEnumAsByte<EOutcomePins>& Outcome) {
	Outcome = PercentChance >= FMath::RandRange(0, 100) ? Success : Failure;
}

void IEventInitiator::RepeatingModifier(int TimesToRepeat, float TimeInBetween) {
}

void IEventInitiator::SendToObject() {
}
