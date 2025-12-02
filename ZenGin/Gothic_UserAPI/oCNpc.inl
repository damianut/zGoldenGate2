// Supported with union (c) 2020 Union team

// User API for oCNpc
// Add your methods here

int EV_AttackForward_Union( oCMsgAttack* );
int EV_PlaySound_Union( oCMsgConversation* );
int FightAttackMelee_Union( int );
int ThinkNextFightAction_Union( );
void OnDamage_Condition_Union( oSDamageDescriptor& );