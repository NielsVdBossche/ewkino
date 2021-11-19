#include "../interface/mt2.h"
#include "../interface/lester_mt2_bisect.h"
#include "../interface/mt2Alt.h"


double mt2::mt2( const PhysicsObject& lhs, const PhysicsObject& rhs, const Met& met ){

    //invisible particles are assumed to have mass 0 (i.e. neutrinos)
    return asymm_mt2_lester_bisect::get_mT2( lhs.mass(), lhs.px(), lhs.py(), rhs.mass(), rhs.px(), rhs.py(), met.px(), met.py(), 0, 0);
}

double mt2::mt2Alt(const PhysicsObject& lhs, const PhysicsObject& rhs, const Met& met) {
    mt2_bisect::mt2 mt2_event;
    
    double pa[] = {0., lhs.px(), lhs.py()};
    double pb[] = {0., rhs.px(), rhs.py()};
    double pmiss[] = {0., met.px(), met.py()};

    mt2_event.set_momenta(pa, pb, pmiss);
    mt2_event.set_mn(0.);

    return mt2_event.get_mt2();
}

double mt2::mt2bb(const PhysicsObject& b1, const PhysicsObject& b2, const PhysicsObject& l1, const PhysicsObject& l2, const Met& met) {
    mt2_bisect::mt2 mt2_event;

    PhysicsObject metVec = met + l1 + l2;

    double pa[] = {0., b1.px(), b1.py()};
    double pb[] = {0., b2.px(), b2.py()};
    double pmiss[] = {0., metVec.px(), metVec.py()};

    mt2_event.set_momenta(pa, pb, pmiss);
    mt2_event.set_mn(80.4);

    return mt2_event.get_mt2();
}


double mt2::mt2lblb(const PhysicsObject& b1, const PhysicsObject& b2, const PhysicsObject& l1, const PhysicsObject& l2, const Met& met) {
    mt2_bisect::mt2 mt2_event;

    PhysicsObject bl1;
    PhysicsObject bl2;

    double max1 = std::max((l1 + b1).mass(), (l2 + b2).mass());
    double max2 = std::max((l1 + b2).mass(), (l2 + b1).mass());
    if (max1 < max2) {
        bl1 = l1 + b1;
        bl2 = l2 + b2;
    } else {
        bl1 = l1 + b2;
        bl2 = l2 + b1;
    }

    double pa[] = {0., bl1.px(), bl1.py()};
    double pb[] = {0., bl2.px(), bl2.py()};
    double pmiss[] = {0., met.px(), met.py()};

    mt2_event.set_momenta(pa, pb, pmiss);
    mt2_event.set_mn(0.);

    return mt2_event.get_mt2();
}