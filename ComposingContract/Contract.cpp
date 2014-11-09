#include "Contract.h"
#include "Visitors.h"
void Zero::accept(Visitor & visitor){ visitor.visit(*this); }
void One::accept(Visitor & visitor){ visitor.visit(*this); }
void Scale::accept(Visitor & visitor){ visitor.visit(*this); }
void When::accept(Visitor & visitor){ visitor.visit(*this); }
void Or::accept(Visitor & visitor){ visitor.visit(*this); }
void And::accept(Visitor & visitor){ visitor.visit(*this); }
