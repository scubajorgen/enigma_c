typedef struct
{
    char    ngram[6];
    int     score;
} NGram;

char ioctrigrams1[][5]=
{
    "EIN",
    "INS",
    "FUE",
    "ZWO",
    "ULL",
    "IER",
    "NUL",
    "UNG",
    "ENF",
    "VIE"
};

char ioctrigrams2[][4]=
{
    "SCH",
    "DIE",
    "NDE",
    "CHE",
    "UND",
    "ICH",
    "TEN",
    "DEN",
    "EIN",
    "END"
};

NGram ioctrigrams3[]=
{
    {"DER", 9420599},
    {"EIN", 7489063},
    {"SCH", 6899233},
    {"ICH", 6760120},
    {"NDE", 6482747},
    {"DIE", 5635370},
    {"CHE", 5254952},
    {"DEN", 5033246},
    {"TEN", 4574812},
    {"UND", 4368411},
    {"INE", 4345996},
    {"TER", 4025969},
    {"GEN", 4013211},
    {"END", 3979534},
    {"ERS", 3833032},
    {"STE", 3775633},
    {"CHT", 3699593},
    {"UNG", 3555443},
    {"DAS", 3469167},
    {"ERE", 3405234},
    {"BER", 3286740},
    {"ENS", 3286666},
    {"NGE", 3210783},
    {"RDE", 3151853},
    {"VER", 3071930},
    {"EIT", 3018115},
    {"HEN", 2796252},
    {"ERD", 2738482},
    {"REI", 2734464},
    {"IND", 2658516},
    {"AUF", 2630884},
    {"NEN", 2600809},
    {"ENT", 2565432},
    {"AND", 2560449},
    {"ERN", 2465524},
    {"ERT", 2442564},
    {"MIT", 2378804},
    {"IST", 2378235},
    {"NTE", 2324682},
    {"BEI", 2248560},
    {"ACH", 2228148},
    {"REN", 2210668},
    {"ENE", 2202487},
    {"AUS", 2163073},
    {"NER", 2158847},
    {"IER", 2112165},
    {"ERA", 2091956},
    {"LLE", 2065617},
    {"ENA", 2004039},
    {"NDI", 1998657},
    {"STA", 1987440},
    {"ABE", 1953063},
    {"DES", 1950829},
    {"SEI", 1947209}
};

NGram iocBiGrams[]=
{
    {"ER",  35707750},
    {"EN",  33080619},
    {"CH",  21636992},
    {"DE",  21124885},
    {"EI",  18144092},
    {"TE",  18126613},
    {"IN",  15690942},
    {"ND",  15344323},
    {"IE",  13578842},
    {"GE",  13317143},
    {"ST",  11118572},
    {"NE",  10917953},
    {"BE",  10756008},
    {"ES",  10703963},
    {"UN",  10388445},
    {"RE",  10140670},
    {"AN",  9756253 },
    {"HE",  8184104 },
    {"AU",  8136116 },
    {"NG",  7970442 },
    {"SE",  7600035 },
    {"IT",  7436421 },
    {"DI",  7384338 },
    {"IC",  7324071 },
    {"SC",  7068205 },
    {"LE",  6638858 },
    {"DA",  6627804 },
    {"NS",  6468472 },
    {"IS",  6389470 },
    {"RA",  6281503 },
    {"EL",  6238102 },
    {"RD",  5889352 },
    {"ON",  5791625 },
    {"NT",  5732665 },
    {"AL",  5659150 },
    {"AS",  5500246 },
    {"LI",  5484579 },
    {"NA",  5463682 },
    {"SI",  5446900 },
    {"HA",  5385452 },
    {"SS",  5159304 },
    {"RS",  5158230 },
    {"RI",  5046663 },
    {"TI",  5022031 },
    {"AR",  4935805 },
    {"RT",  4914671 },
    {"NI",  4836856 },
    {"EM",  4816801 },
    {"ME",  4792737 },
    {"TA",  4711549 },
    {"LL",  4572904 },
    {"WE",  4405901 },
    {"ET",  4399720 },
    {"OR",  4312069 },
    {"MI", 4304636},
    {"US", 4251806},
    {"TS", 4219950},
    {"HT", 4169217},
    {"AT", 4052430},
    {"NN", 4034812},
    {"HR", 3922137},
    {"ED", 3920989},
    {"ZU", 3658739},
    {"LA", 3649982},
    {"EH", 3630189},
    {"VE", 3626090},
    {"MA", 3598054},
    {"EG", 3577454},
    {"UR", 3547966},
    {"VO", 3525613},
    {"IG", 3497890},
    {"WI", 3449060},
    {"EU", 3401634},
    {"SA", 3374227},
    {"AM", 3217895},
    {"AB", 3171003},
    {"AG", 3161970},
    {"RO", 3160632},
    {"UF", 3105709},
    {"EB", 3065012},
    {"ZE", 2980732},
    {"RN", 2965436},
    {"RU", 2899129},
    {"TR", 2835856},
    {"NK", 2664279},
    {"LT", 2647387},
    {"TD", 2645479},
    {"RG", 2640899},
    {"NU", 2624679},
    {"IM", 2621567},
    {"TZ", 2591974},
    {"OL", 2570680},
    {"FE", 2548396},
    {"SO", 2537620},
    {"AC", 2513914},
    {"IL", 2503793},
    {"SP", 2477871},
    {"RB", 2447166},
    {"EA", 2389014},
    {"TT", 2383344},
    {"RK", 2361533},
    {"KE", 2322635},
    {"NZ", 2316240},
    {"UT", 2310920},
    {"UM", 2308646},
    {"PR", 2296623},
    {"WA", 2262926},
    {"TU", 2255379},
    {"AH", 2243224},
    {"NW", 2161930},
    {"NB", 2083040},
    {"MM", 2071035},
    {"EF", 2059409},
    {"LS", 2029806}

};
