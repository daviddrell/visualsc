#include "fvstate.h"

FVState::FVState(SCState* state, CustomTreeWidgetItem* stateTreeItem): FVItem(stateTreeItem),
    _state(state)
{
}

FVState::~FVState()
{

}

/*
FVState& FVState::operator=(const SCState &state)
{
    setState(state);
}
FVState& FVState::operator=(const FVState &state)
{
    setState(state.getState());
}
*/

SCState* FVState::getState()
{
    return _state;
}

void FVState::setState(SCState* state)
{
    _state = state;
}

