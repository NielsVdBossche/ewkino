#include "../interface/mt2.h"
#include "../interface/lester_mt2_bisect.h"
#include "../interface/mt2Alt.h"


double mt2::mt2( const PhysicsObject& lhs, const PhysicsObject& rhs, const Met& met ){

    //invisible particles are assumed to have mass 0 (i.e. neutrinos)
    return asymm_mt2_lester_bisect::get_mT2( lhs.mass(), lhs.px(), lhs.py(), rhs.mass(), rhs.px(), rhs.py(), met.px(), met.py(), 0, 0);
}

double mt2::mt2Alt(const PhysicsObject& lhs, const PhysicsObject& rhs, const Met& met) {
    mt2_bisect::mt2 mt2_event;
    
    double pa[] = {lhs.mass(), lhs.px(), lhs.py()};
    double pb[] = {rhs.mass(), rhs.px(), rhs.py()};
    double pmiss[] = {0., met.px(), met.py()};

    mt2_event.set_momenta(pa, pb, pmiss);
    mt2_event.set_mn(0.);

    return mt2_event.get_mt2();
}
