// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: TriggerList.cpp
// Purpose: Handles list of triggers in an event
 
#ifndef TRIGGERLIST_CPP__
#define TRIGGERLIST_CPP__

#include "TriggerList.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
TriggerList::TriggerList() {
  Reset();
}

/*************************************************************************/
//                             Destructor
/*************************************************************************/
TriggerList::~TriggerList() {
}

/*************************************************************************/
//                                Reset
/*************************************************************************/
void TriggerList::Reset() {
  triggerlist.resize(0);
  trigorderedlist.resize(0);
  ordered = false;
  tottrig = 0;
  check = NULL;
}

/*************************************************************************/
//                             AddTriggers
/*************************************************************************/
void TriggerList::AddTriggers(vector<trigger_t> addtrigs) {
  triggerlist.insert(triggerlist.end(), addtrigs.begin(), addtrigs.end());
}

/*************************************************************************/
//                            OrderTriggers
/*************************************************************************/
void TriggerList::OrderTriggers() {
  tottrig = triggerlist.size();
  trigorderedlist.resize(tottrig);
  check = NULL;
  if (tottrig == 0) {
    ordered = true;
    return;
  }
  for (int cnt=0;cnt<tottrig;cnt++) {
    trigorderedlist[cnt].prev = NULL;
    trigorderedlist[cnt].next = NULL;
    trigorderedlist[cnt].mytrig = &triggerlist[cnt];
    if (cnt > 0) {
      check = &trigorderedlist[0];
      if (trigorderedlist[cnt].mytrig->T > check->mytrig->T) {
	while (trigorderedlist[cnt].mytrig->T > check->mytrig->T && check->next != NULL) {
	  check = check->next;
	}
	if (trigorderedlist[cnt].mytrig->T <= check->mytrig->T) {
	  trigorderedlist[cnt].next = check;
	  trigorderedlist[cnt].prev = check->prev;
	  check->prev = &trigorderedlist[cnt];
	  if (trigorderedlist[cnt].prev != NULL)
	    trigorderedlist[cnt].prev->next = &trigorderedlist[cnt];
	}
	else{
	  trigorderedlist[cnt].prev = check;
	  trigorderedlist[cnt].next = check->next;
	  check->next = &trigorderedlist[cnt];
	  if (trigorderedlist[cnt].next != NULL)
	    trigorderedlist[cnt].next->prev = &trigorderedlist[cnt];
	}
      }
      else {
	while (trigorderedlist[cnt].mytrig->T < check->mytrig->T && check->prev != NULL) {
	  check = check->prev;
	}
	if (trigorderedlist[cnt].mytrig->T >= check->mytrig->T) {
	  trigorderedlist[cnt].prev = check;
	  trigorderedlist[cnt].next = check->next;
	  check->next = &trigorderedlist[cnt];
	  if (trigorderedlist[cnt].next != NULL)
	    trigorderedlist[cnt].next->prev = &trigorderedlist[cnt];
	}
	else {
	  trigorderedlist[cnt].next = check;
	  trigorderedlist[cnt].prev = check->prev;
	  check->prev = &trigorderedlist[cnt];
	  if (trigorderedlist[cnt].prev != NULL)
	    trigorderedlist[cnt].prev->next = &trigorderedlist[cnt];
	}
      }// if !(trigorderedlist[cnt].mytrig->T > check->mytrig->T)
    }// if (cnt > 0)
  }//for cnt < tottrig
  check = &trigorderedlist[0];
  while (check->prev != NULL)
    check = check->prev;
  ordered = true;
}

/*************************************************************************/
//                            GetMaxTrigger
/*************************************************************************/
bool TriggerList::GetMaxTrigger(trigger_t &trig) {
  if (triggerlist.size() ==0 ) return false;
  double max = -1;
  int maxi = -1;
  for (int i=0;i<triggerlist.size();i++) {
    if (max < TMath::Abs(triggerlist[i].E)) {
      max = TMath::Abs(triggerlist[i].E);
      maxi = i;
    }
  }
  trig = triggerlist[maxi];
  return true;
}


/*************************************************************************/
//                            GetNextTrigger
/*************************************************************************/
bool TriggerList::GetTrigger(trigger_t &trig) {
  if (!ordered) {cout << "Call OrderTriggerList first" << endl; return false;}
  if (check == NULL) return false;
  trig = *check->mytrig;
  /*
  trig.ch = check->mytrig->ch;
  trig.E = check->mytrig->E;
  trig.T = check->mytrig->T;
  trig.Shaping = check->mytrig->Shaping;
  trig.Integration = check->mytrig->Integration;
  trig.Chi2 = check->mytrig->Chi2;
  trig.TrapE = check->mytrig->TrapE;
  trig.TrapT = check->mytrig->TrapT;
  */
  check = check->next;
  return true;
}

#endif // TRIGGERLIST_CPP__
