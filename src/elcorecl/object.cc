// Copyright 2018 RnD Center "ELVEES", JSC

#include "elcore-cl.h"

void intrusive_ptr_add_ref(_ecl_object *p) { p->add_ref(); }

void intrusive_ptr_release(_ecl_object *p) { p->release(); }
