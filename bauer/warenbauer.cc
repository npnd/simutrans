/*
 * warenbauer.cc
 *
 * Copyright (c) 1997 - 2002 Hansj�rg Malthaner
 *
 * This file is part of the Simutrans project and may not be used
 * in other projects without written permission of the author.
 */

#include "../simdebug.h"
#include "../besch/ware_besch.h"
#include "../besch/spezial_obj_tpl.h"
#include "warenbauer.h"

stringhashtable_tpl<const ware_besch_t *> warenbauer_t::besch_names;

slist_tpl<const ware_besch_t *> warenbauer_t::waren;

const ware_besch_t *warenbauer_t::passagiere = NULL;
const ware_besch_t *warenbauer_t::post = NULL;
const ware_besch_t *warenbauer_t::nichts = NULL;

static spezial_obj_tpl<ware_besch_t> spezial_objekte[] = {
    { &warenbauer_t::passagiere,    "Passagiere" },
    { &warenbauer_t::post,	    "Post" },
    { &warenbauer_t::nichts,	    "None" },
    { NULL, NULL }
};


bool warenbauer_t::alles_geladen()
{
	if(!::alles_geladen(spezial_objekte)) {
		return false;
	}
	/**
	* Put special items in front:
	* Volker Meyer
	*/
	waren.insert(post);
	waren.insert(passagiere);
	waren.insert(nichts);
	return true;
}


bool warenbauer_t::register_besch(ware_besch_t *besch)
{
	::register_besch(spezial_objekte, besch);
	besch_names.put(besch->gib_name(), const_cast<ware_besch_t *>(besch));

	if(besch==passagiere) {
		besch->ware_index = 0;
	} else if(besch==post) {
		besch->ware_index = 1;
	} else if(besch != nichts) {
		besch->ware_index = waren.count()+2;
		waren.append(besch);
	}
	return true;
}





const ware_besch_t *warenbauer_t::gib_info(const char* name)
{
    const ware_besch_t *t = (const ware_besch_t *)besch_names.get(name);

    if(t == NULL) {
        dbg->error("warenbauer_t::gib_info()", "No info for good '%s' available", name);
    }

    return t;
}



const ware_besch_t *warenbauer_t::gib_info_catg(const sint8 catg)
{
	if(catg>0) {
		for(int i=0;  i<gib_waren_anzahl();  i++  ) {
			if(waren.at(i)->catg==catg) {
				return (const ware_besch_t *)waren.at(i);
			}
		}
	}
	dbg->warning("warenbauer_t::gib_info()", "No info for good catg %d available, set to passengers", catg);
	return waren.at(0);
}
