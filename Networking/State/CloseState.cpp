#include "CloseState.hpp"

#include "../Connections/DataConnection.hpp"

CloseState::CloseState (dataConnection *context): State (context) {}

void CloseState::handle (void) {
    context->getClose () = true;
}
